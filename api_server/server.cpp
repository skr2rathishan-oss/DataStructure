#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#ifdef WINVER
#undef WINVER
#endif
#define _WIN32_WINNT 0x0A00
#define WINVER 0x0A00
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif

#include "httplib.h"
#include "json.hpp"

#include "../backend/services/AuthService.h"
#include "../backend/services/FileService.h"
#include "../backend/services/AccessControlService.h"
#include "../backend/services/RequestService.h"
#include "../backend/sample_data/sample_data.h"

using json = nlohmann::json;
using namespace std;

void set_cors_headers(httplib::Response &res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
}

int main() {
    httplib::Server svr;
    AuthService auth;
    FileService fileService;
    AccessControlService accessControl;
    RequestService requestService;

    // Load initial data
    loadSampleData(auth, accessControl);
    fileService.loadStoredFiles();
    fileService.syncPermissionGraph(accessControl);

    // CORS preflight
    svr.Options(R"(.*)", [](const httplib::Request &req, httplib::Response &res) {
        (void)req;
        set_cors_headers(res);
        res.status = 200;
    });

    // POST /api/login
    svr.Post("/api/login", [&](const httplib::Request &req, httplib::Response &res) {
        set_cors_headers(res);
        try {
            auto body = json::parse(req.body);
            string username = body["username"];
            string password = body["password"];

            if (auth.login(username, password)) {
                User* u = auth.getCurrentUser();
                json response = {
                    {"success", true},
                    {"user", {
                        {"username", u->username},
                        {"role", u->role},
                        {"userId", u->userId}
                    }}
                };
                res.set_content(response.dump(), "application/json");
            } else {
                json response = {{"success", false}, {"error", "Invalid credentials"}};
                res.set_content(response.dump(), "application/json");
            }
        } catch (...) {
            res.status = 400;
            res.set_content(json({{"error", "Bad Request"}}).dump(), "application/json");
        }
    });

    // GET /api/files?username=admin
    svr.Get("/api/files", [&](const httplib::Request &req, httplib::Response &res) {
        set_cors_headers(res);
        string username = req.has_param("username") ? req.get_param_value("username") : "";
        User* user = auth.getUser(username);
        if (!user) {
            res.status = 401;
            res.set_content(json({{"error", "Unauthorized"}}).dump(), "application/json");
            return;
        }

        auto accessibleFiles = fileService.getAccessibleFilesList(username, &accessControl, user->role);
        json filesJson = json::array();
        for (const auto& file : accessibleFiles) {
            filesJson.push_back({
                {"fileId", file.fileId},
                {"fileName", file.fileName},
                {"filePath", file.filePath},
                {"fileType", file.fileType},
                {"fileSize", file.fileSize},
                {"owner", file.owner},
                {"visibility", file.visibility}
            });
        }
        res.set_content(filesJson.dump(), "application/json");
    });

    // POST /api/files/upload-metadata
    svr.Post("/api/files/upload-metadata", [&](const httplib::Request &req, httplib::Response &res) {
        set_cors_headers(res);
        try {
            auto body = json::parse(req.body);
            string username = body["username"];
            string fileName = body["fileName"];
            string filePath = body["filePath"];
            string fileType = body["fileType"];
            int fileSize = body["fileSize"];

            User* user = auth.getUser(username);
            if (!user) {
                res.status = 401;
                res.set_content(json({{"error", "Unauthorized"}}).dump(), "application/json");
                return;
            }

            FileRecord uploadedFile = fileService.uploadFile(fileName, filePath, username, fileType, fileSize);
            accessControl.addFileNode(uploadedFile.fileId);
            accessControl.grantPermission(username, uploadedFile.fileId);

            json response = {
                {"success", true},
                {"file", {
                    {"fileId", uploadedFile.fileId},
                    {"fileName", uploadedFile.fileName},
                    {"owner", uploadedFile.owner},
                    {"visibility", uploadedFile.visibility}
                }}
            };
            res.set_content(response.dump(), "application/json");
        } catch (...) {
            res.status = 400;
            res.set_content(json({{"error", "Bad Request"}}).dump(), "application/json");
        }
    });

    // POST /api/files/upload
    svr.Post("/api/files/upload", [&](const httplib::Request &req, httplib::Response &res) {
        set_cors_headers(res);
        try {
            auto body = json::parse(req.body);
            string username = body["username"];
            string sourcePath = body.contains("filePath")
                ? body["filePath"].get<string>()
                : body["sourcePath"].get<string>();

            User* user = auth.getUser(username);
            if (!user) {
                res.status = 401;
                res.set_content(json({{"error", "Unauthorized"}}).dump(), "application/json");
                return;
            }

            if (sourcePath.empty()) {
                res.status = 400;
                res.set_content(json({{"error", "File path is required"}}).dump(), "application/json");
                return;
            }

            FileRecord uploadedFile = fileService.uploadRealFile(sourcePath, username);
            if (uploadedFile.fileId <= 0) {
                res.status = 400;
                res.set_content(json({{"error", "Upload failed. Check that the file path exists on this computer."}}).dump(), "application/json");
                return;
            }

            accessControl.addFileNode(uploadedFile.fileId);
            accessControl.grantPermission(username, uploadedFile.fileId);

            json response = {
                {"success", true},
                {"file", {
                    {"fileId", uploadedFile.fileId},
                    {"fileName", uploadedFile.fileName},
                    {"filePath", uploadedFile.filePath},
                    {"fileType", uploadedFile.fileType},
                    {"fileSize", uploadedFile.fileSize},
                    {"owner", uploadedFile.owner},
                    {"visibility", uploadedFile.visibility}
                }}
            };
            res.set_content(response.dump(), "application/json");
        } catch (...) {
            res.status = 400;
            res.set_content(json({{"error", "Bad Request"}}).dump(), "application/json");
        }
    });

    // GET /api/files/search?username=admin&q=report
    svr.Get("/api/files/search", [&](const httplib::Request &req, httplib::Response &res) {
        set_cors_headers(res);
        string username = req.has_param("username") ? req.get_param_value("username") : "";
        string q = req.has_param("q") ? req.get_param_value("q") : "";
        User* user = auth.getUser(username);
        if (!user) {
            res.status = 401;
            res.set_content(json({{"error", "Unauthorized"}}).dump(), "application/json");
            return;
        }

        auto accessibleFiles = fileService.searchAccessibleFilesList(q, username, &accessControl, user->role);
        json filesJson = json::array();
        for (const auto& file : accessibleFiles) {
            filesJson.push_back({
                {"fileId", file.fileId},
                {"fileName", file.fileName},
                {"filePath", file.filePath},
                {"owner", file.owner},
                {"visibility", file.visibility}
            });
        }
        res.set_content(filesJson.dump(), "application/json");
    });

    // GET /api/files/:fileId/download?username=admin
    svr.Get(R"(/api/files/(\d+)/download)", [&](const httplib::Request &req, httplib::Response &res) {
        set_cors_headers(res);
        int fileId = stoi(req.matches[1]);
        string username = req.has_param("username") ? req.get_param_value("username") : "";
        User* user = auth.getUser(username);
        if (!user) {
            res.status = 401;
            res.set_content(json({{"error", "Unauthorized"}}).dump(), "application/json");
            return;
        }

        FileRecord* file = fileService.getFileById(fileId);
        if (file == nullptr || file->isDeleted) {
            res.status = 404;
            res.set_content(json({{"error", "File not found"}}).dump(), "application/json");
            return;
        }

        if (!accessControl.canAccess(username, user->role, *file)) {
            res.status = 403;
            res.set_content(json({{"error", "Access denied"}}).dump(), "application/json");
            return;
        }

        json response = {
            {"success", true},
            {"message", "Download requested. Note: Real file download requires real storage support."},
            {"file", {
                {"fileId", file->fileId},
                {"fileName", file->fileName},
                {"filePath", file->filePath},
                {"fileSize", file->fileSize}
            }}
        };
        res.set_content(response.dump(), "application/json");
    });

    // DELETE /api/files/:fileId?username=admin
    svr.Delete(R"(/api/files/(\d+))", [&](const httplib::Request &req, httplib::Response &res) {
        set_cors_headers(res);
        int fileId = stoi(req.matches[1]);
        string username = req.has_param("username") ? req.get_param_value("username") : "";
        User* user = auth.getUser(username);
        if (!user) {
            res.status = 401;
            res.set_content(json({{"error", "Unauthorized"}}).dump(), "application/json");
            return;
        }

        FileRecord* file = fileService.getFileById(fileId);
        if (file == nullptr || file->isDeleted) {
            res.status = 404;
            res.set_content(json({{"error", "File not found"}}).dump(), "application/json");
            return;
        }

        if (user->role != "Admin" && file->owner != username) {
            res.status = 403;
            res.set_content(json({{"error", "Access denied"}}).dump(), "application/json");
            return;
        }

        bool deleted = false;
        if (file->filePath.find("storage") != string::npos) {
            deleted = fileService.deleteRealFile(fileId);
        } else {
            deleted = fileService.deleteFile(fileId);
        }

        if (deleted) {
            accessControl.makePrivate(fileId);
            res.set_content(json({{"success", true}}).dump(), "application/json");
        } else {
            res.status = 500;
            res.set_content(json({{"error", "Failed to delete file"}}).dump(), "application/json");
        }
    });

    // POST /api/files/delete
    svr.Post("/api/files/delete", [&](const httplib::Request &req, httplib::Response &res) {
        set_cors_headers(res);
        try {
            auto body = json::parse(req.body);
            string username = body["username"];
            int fileId = body["fileId"];

            User* user = auth.getUser(username);
            if (!user) {
                res.status = 401;
                res.set_content(json({{"error", "Unauthorized"}}).dump(), "application/json");
                return;
            }

            FileRecord* file = fileService.getFileById(fileId);
            if (file == nullptr || file->isDeleted) {
                res.status = 404;
                res.set_content(json({{"error", "File not found"}}).dump(), "application/json");
                return;
            }

            if (user->role != "Admin" && file->owner != username) {
                res.status = 403;
                res.set_content(json({{"error", "Access denied"}}).dump(), "application/json");
                return;
            }

            bool deleted = false;
            if (file->filePath.find("storage") != string::npos) {
                deleted = fileService.deleteRealFile(fileId);
            } else {
                deleted = fileService.deleteFile(fileId);
            }

            if (deleted) {
                accessControl.makePrivate(fileId);
                res.set_content(json({{"success", true}}).dump(), "application/json");
            } else {
                res.status = 500;
                res.set_content(json({{"error", "Failed to delete file"}}).dump(), "application/json");
            }
        } catch (...) {
            res.status = 400;
            res.set_content(json({{"error", "Bad Request"}}).dump(), "application/json");
        }
    });

    // GET /api/permissions
    svr.Get("/api/permissions", [&](const httplib::Request &req, httplib::Response &res) {
        (void)req;
        set_cors_headers(res);
        res.set_content(json({{"message", "Permission graph available in console."}}).dump(), "application/json");
    });

    // POST /api/permissions/grant
    svr.Post("/api/permissions/grant", [&](const httplib::Request &req, httplib::Response &res) {
        set_cors_headers(res);
        try {
            auto body = json::parse(req.body);
            string adminUsername = body["adminUsername"];
            string targetUsername = body["targetUsername"];
            int fileId = body["fileId"];

            User* adminUser = auth.getUser(adminUsername);
            if (!adminUser) {
                res.status = 401;
                res.set_content(json({{"error", "Unauthorized"}}).dump(), "application/json");
                return;
            }

            FileRecord* file = fileService.getFileById(fileId);
            if (file == nullptr || file->isDeleted) {
                res.status = 404;
                res.set_content(json({{"error", "File not found"}}).dump(), "application/json");
                return;
            }

            if (adminUser->role != "Admin" && file->owner != adminUsername) {
                res.status = 403;
                res.set_content(json({{"error", "Only Admin or owner can share"}}).dump(), "application/json");
                return;
            }

            if (!auth.userExists(targetUsername)) {
                res.status = 404;
                res.set_content(json({{"error", "Target user not found"}}).dump(), "application/json");
                return;
            }

            accessControl.grantPermission(targetUsername, fileId);
            if (file->visibility == "PRIVATE") {
                fileService.setFileVisibility(fileId, "RESTRICTED");
            }

            res.set_content(json({{"success", true}}).dump(), "application/json");
        } catch (...) {
            res.status = 400;
            res.set_content(json({{"error", "Bad Request"}}).dump(), "application/json");
        }
    });

    // POST /api/permissions/revoke
    svr.Post("/api/permissions/revoke", [&](const httplib::Request &req, httplib::Response &res) {
        set_cors_headers(res);
        try {
            auto body = json::parse(req.body);
            string adminUsername = body["adminUsername"];
            string targetUsername = body["targetUsername"];
            int fileId = body["fileId"];

            User* adminUser = auth.getUser(adminUsername);
            if (!adminUser) {
                res.status = 401;
                return;
            }

            FileRecord* file = fileService.getFileById(fileId);
            if (file == nullptr || file->isDeleted) {
                res.status = 404;
                res.set_content(json({{"error", "File not found"}}).dump(), "application/json");
                return;
            }

            if (adminUser->role != "Admin" && file->owner != adminUsername) {
                res.status = 403;
                res.set_content(json({{"error", "Only Admin or owner can revoke"}}).dump(), "application/json");
                return;
            }

            if (targetUsername == file->owner) {
                res.status = 400;
                res.set_content(json({{"error", "Owner access cannot be revoked"}}).dump(), "application/json");
                return;
            }

            accessControl.revokePermission(targetUsername, fileId);
            res.set_content(json({{"success", true}}).dump(), "application/json");
        } catch (...) {
            res.status = 400;
            res.set_content(json({{"error", "Bad Request"}}).dump(), "application/json");
        }
    });

    // POST /api/permissions/check
    svr.Post("/api/permissions/check", [&](const httplib::Request &req, httplib::Response &res) {
        set_cors_headers(res);
        try {
            auto body = json::parse(req.body);
            string username = body["username"];
            int fileId = body["fileId"];

            User* user = auth.getUser(username);
            FileRecord* file = fileService.getFileById(fileId);

            if (!file || file->isDeleted) {
                res.status = 404;
                res.set_content(json({{"error", "File not found"}}).dump(), "application/json");
                return;
            }

            string role = user ? user->role : "";
            bool allowed = accessControl.canAccess(username, role, *file);

            res.set_content(json({{"allowed", allowed}, {"reason", allowed ? "Has permission" : "Access denied"}}).dump(), "application/json");
        } catch (...) {
            res.status = 400;
            res.set_content(json({{"error", "Bad Request"}}).dump(), "application/json");
        }
    });

    // POST /api/requests/access
    int requestIdCounter = 1000;
    svr.Post("/api/requests/access", [&](const httplib::Request &req, httplib::Response &res) {
        set_cors_headers(res);
        try {
            auto body = json::parse(req.body);
            string requesterUsername = body["requesterUsername"];
            string ownerUsername = body["ownerUsername"];
            int fileId = body["fileId"];

            User* requester = auth.getUser(requesterUsername);
            if (!requester) {
                res.status = 401;
                return;
            }

            FileRecord* file = fileService.getFileById(fileId);
            if (!file || file->isDeleted || file->owner != ownerUsername) {
                res.status = 404;
                res.set_content(json({{"error", "Invalid file or owner"}}).dump(), "application/json");
                return;
            }

            requestService.addAccessRequest(requestIdCounter++, requester->userId, requesterUsername, ownerUsername, fileId);
            res.set_content(json({{"success", true}}).dump(), "application/json");
        } catch (...) {
            res.status = 400;
            res.set_content(json({{"error", "Bad Request"}}).dump(), "application/json");
        }
    });

    // GET /api/requests/next
    svr.Get("/api/requests/next", [&](const httplib::Request &req, httplib::Response &res) {
        (void)req;
        set_cors_headers(res);
        ::Request* nextReq = requestService.getNextRequestData();
        if (nextReq) {
            json response = {
                {"hasRequest", true},
                {"request", {
                    {"requestId", nextReq->requestId},
                    {"requesterUsername", nextReq->requesterUsername},
                    {"ownerUsername", nextReq->ownerUsername},
                    {"fileId", nextReq->fileId},
                    {"status", nextReq->status}
                }}
            };
            delete nextReq;
            res.set_content(response.dump(), "application/json");
        } else {
            res.set_content(json({{"hasRequest", false}}).dump(), "application/json");
        }
    });

    // POST /api/requests/process
    svr.Post("/api/requests/process", [&](const httplib::Request &req, httplib::Response &res) {
        set_cors_headers(res);
        try {
            auto body = json::parse(req.body);
            string username = body["username"];
            bool approved = body["approved"];

            User* user = auth.getUser(username);
            if (!user) {
                res.status = 401;
                return;
            }

            bool processed = requestService.processNextRequestApi(username, user->role, approved, fileService, accessControl);
            if (processed) {
                res.set_content(json({{"success", true}}).dump(), "application/json");
            } else {
                res.status = 400;
                res.set_content(json({{"error", "Failed to process request or unauthorized"}}).dump(), "application/json");
            }
        } catch (...) {
            res.status = 400;
            res.set_content(json({{"error", "Bad Request"}}).dump(), "application/json");
        }
    });

    // GET /api/trash?username=admin
    svr.Get("/api/trash", [&](const httplib::Request &req, httplib::Response &res) {
        set_cors_headers(res);
        string username = req.has_param("username") ? req.get_param_value("username") : "";
        User* user = auth.getUser(username);
        if (!user) {
            res.status = 401;
            res.set_content(json({{"error", "Unauthorized"}}).dump(), "application/json");
            return;
        }

        auto trashFiles = fileService.getTrashFilesList(username, user->role);
        json filesJson = json::array();
        for (const auto& file : trashFiles) {
            filesJson.push_back({
                {"fileId", file.fileId},
                {"fileName", file.fileName},
                {"filePath", file.filePath},
                {"fileType", file.fileType},
                {"fileSize", file.fileSize},
                {"owner", file.owner},
                {"visibility", file.visibility}
            });
        }
        res.set_content(filesJson.dump(), "application/json");
    });

    // POST /api/trash/restore
    svr.Post("/api/trash/restore", [&](const httplib::Request &req, httplib::Response &res) {
        set_cors_headers(res);
        try {
            auto body = json::parse(req.body);
            string username = body["username"];
            User* user = auth.getUser(username);
            if (!user) {
                res.status = 401;
                return;
            }

            auto trashFiles = fileService.getTrashFilesList(username, user->role);
            if (trashFiles.empty()) {
                res.status = 404;
                res.set_content(json({{"error", "No files in trash"}}).dump(), "application/json");
                return;
            }

            int fileId = trashFiles.back().fileId;
            bool restored = false;
            
            if (trashFiles.back().filePath.find("storage") != string::npos) {
                 restored = fileService.restoreRealFile(fileId);
            } else {
                 FileRecord* file = fileService.getFileById(fileId);
                 if (file) {
                     file->isDeleted = false;
                     restored = true;
                 }
            }

            if (restored) {
                FileRecord* file = fileService.getFileById(fileId);
                accessControl.addFileNode(fileId);
                if (file->owner != "unknown") {
                    accessControl.grantPermission(file->owner, fileId);
                }
                if (file->visibility == "PUBLIC") {
                    accessControl.makePublic(fileId);
                }
                res.set_content(json({{"success", true}, {"fileId", fileId}}).dump(), "application/json");
            } else {
                res.status = 500;
                res.set_content(json({{"error", "Failed to restore file"}}).dump(), "application/json");
            }
        } catch (...) {
            res.status = 400;
            res.set_content(json({{"error", "Bad Request"}}).dump(), "application/json");
        }
    });

    cout << "API Server running on http://localhost:8080..." << endl;
    svr.listen("0.0.0.0", 8080);

    return 0;
}
