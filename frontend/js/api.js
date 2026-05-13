const API_BASE = 'http://localhost:8080/api';

async function fetchJson(endpoint, options = {}) {
    try {
        const response = await fetch(`${API_BASE}${endpoint}`, options);
        const data = await response.json();
        if (!response.ok) {
            throw new Error(data.error || 'API Error');
        }
        return data;
    } catch (e) {
        console.error("API call failed:", e);
        throw e;
    }
}

export async function apiLogin(username, password) {
    return await fetchJson('/login', {
        method: 'POST',
        body: JSON.stringify({ username, password })
    });
}

export async function apiGetFiles(username) {
    return await fetchJson(`/files?username=${encodeURIComponent(username)}`);
}

export async function apiUploadMetadata(payload) {
    return await fetchJson('/files/upload-metadata', {
        method: 'POST',
        body: JSON.stringify(payload)
    });
}

export async function apiUploadFilePath(username, filePath) {
    return await fetchJson('/files/upload', {
        method: 'POST',
        body: JSON.stringify({ username, filePath })
    });
}

export async function apiSearchFiles(username, query) {
    return await fetchJson(`/files/search?username=${encodeURIComponent(username)}&q=${encodeURIComponent(query)}`);
}

export async function apiDownloadFile(username, fileId) {
    return await fetchJson(`/files/${fileId}/download?username=${encodeURIComponent(username)}`);
}

export async function apiDeleteFile(username, fileId) {
    return await fetchJson('/files/delete', {
        method: 'POST',
        body: JSON.stringify({ username, fileId: parseInt(fileId) })
    });
}

export async function apiGrantPermission(adminUsername, targetUsername, fileId) {
    return await fetchJson('/permissions/grant', {
        method: 'POST',
        body: JSON.stringify({ adminUsername, targetUsername, fileId: parseInt(fileId) })
    });
}

export async function apiRevokePermission(adminUsername, targetUsername, fileId) {
    return await fetchJson('/permissions/revoke', {
        method: 'POST',
        body: JSON.stringify({ adminUsername, targetUsername, fileId: parseInt(fileId) })
    });
}

export async function apiCheckPermission(username, fileId) {
    return await fetchJson('/permissions/check', {
        method: 'POST',
        body: JSON.stringify({ username, fileId: parseInt(fileId) })
    });
}

export async function apiCreateAccessRequest(requesterUsername, ownerUsername, fileId) {
    return await fetchJson('/requests/access', {
        method: 'POST',
        body: JSON.stringify({ requesterUsername, ownerUsername, fileId: parseInt(fileId) })
    });
}

export async function apiGetNextRequest() {
    return await fetchJson('/requests/next');
}

export async function apiProcessRequest(username, approved) {
    return await fetchJson('/requests/process', {
        method: 'POST',
        body: JSON.stringify({ username, approved })
    });
}

export async function apiRestoreTrash(username) {
    return await fetchJson('/trash/restore', {
        method: 'POST',
        body: JSON.stringify({ username })
    });
}

export async function apiGetTrashFiles(username) {
    return await fetchJson(`/trash?username=${encodeURIComponent(username)}`);
}
