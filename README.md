# SmartCloudStorage

SmartCloudStorage is a C++ Data Structures project with a console backend, a separate HTTP API adapter, and a plain HTML/CSS/JavaScript frontend.

The core backend demonstrates:

- Hash tables for user and file lookup
- Linked list for file listing/search traversal
- BST for file ID organization
- Graph for file permissions
- Queue for access requests
- Stack for trash/restore behavior

## Project Structure

```text
SmartCloudStorage/
├── backend/                 # Main C++ console application
│   ├── main.cpp
│   ├── data_structures/
│   ├── models/
│   ├── services/
│   └── sample_data/
├── api_server/              # Separate API server for frontend connection
│   ├── server.cpp
│   └── download_deps.bat
├── frontend/                # Plain HTML/CSS/JS frontend
│   ├── index.html
│   ├── files.html
│   ├── permissions.html
│   ├── requests.html
│   ├── trash.html
│   ├── css/
│   └── js/
└── docs/
```

## Requirements

- C++ compiler with C++17 support, such as MinGW `g++`
- `curl`, only needed once to download API headers
- Python, optional but recommended for serving the frontend locally

## 1. Run The Console Backend

Use this when you want to test the original backend menu directly.

```powershell
cd backend
g++ main.cpp -std=c++17 -Wall -Wextra -g -o main.exe
.\main.exe
```

Demo users:

```text
admin / admin123
user1 / user123
viewer1 / viewer123
```

## 2. Set Up API Server Dependencies

The frontend connects through `api_server/server.cpp`. This API server needs two single-header libraries:

- `httplib.h`
- `json.hpp`

Download them once:

```powershell
cd api_server
.\download_deps.bat
```

After this, `api_server/` should contain:

```text
httplib.h
json.hpp
```

## 3. Run The API Server

Compile the API server:

```powershell
cd api_server
.\build_server.bat
```

Then start it from the project root so it uses `backend/storage` cleanly:

```powershell
cd ..
.\api_server\server.exe
```

Expected output:

```text
API Server running on http://localhost:8080...
```

Keep this terminal open while using the frontend.

## 4. Run The Frontend

Open a second terminal from the project root and serve the frontend with a local web server:

```powershell
python -m http.server 5500
```

Then open:

```text
http://localhost:5500/frontend/index.html
```

Do not open the HTML files directly with `file://`, because the frontend uses JavaScript modules and API calls.

## Normal Run Order

Use this order for the full project:

```text
1. Start API server on localhost:8080
2. Start frontend server on localhost:5500
3. Open frontend/index.html in browser
4. Login and test files, permissions, requests, and trash
```

## API Notes

The frontend should call the API server for project data. It should not use dummy frontend data for files, permissions, requests, or trash.

Main API base URL:

```text
http://localhost:8080/api
```

Important endpoints include:

```text
POST   /api/login
GET    /api/files?username=admin
POST   /api/files/upload
POST   /api/files/upload-metadata
GET    /api/files/search?username=admin&q=report
GET    /api/files/:fileId/download?username=admin
DELETE /api/files/:fileId?username=admin
POST   /api/files/delete
POST   /api/permissions/grant
POST   /api/permissions/revoke
POST   /api/permissions/check
POST   /api/requests/access
GET    /api/requests/next
POST   /api/requests/process
GET    /api/trash?username=admin
POST   /api/trash/restore
```

## Testing Checklist

After starting the API server and frontend:

1. Login as `admin`.
2. Upload a real file by entering its full file path on the Files page.
3. Search for the uploaded file.
4. Download/check the file metadata.
5. Grant permission to `user1`.
6. Login as `user1`.
7. Confirm `user1` can see/access the shared file.
8. Create an access request.
9. Process the request as admin or owner.
10. Delete a file.
11. Restore from trash.

## Git Ignore Notes

The project ignores local build/runtime files:

```text
backend/main.exe
backend/storage/
```

Do not commit generated executables or uploaded runtime files.

## Current Limitation

The API server connects the frontend to backend services, but some file operations may still be metadata-based unless real file storage support is fully enabled in `FileService` and `StorageService`.
