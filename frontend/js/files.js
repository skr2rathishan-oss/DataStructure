import { apiGetFiles, apiUploadFilePath, apiSearchFiles, apiDownloadFile, apiDeleteFile } from './api.js';

const user = JSON.parse(localStorage.getItem('currentUser'));
if (!user) window.location.href = 'index.html';

document.getElementById('logoutBtn').addEventListener('click', () => {
    localStorage.removeItem('currentUser');
    window.location.href = 'index.html';
});

async function loadFiles(query = '') {
    const tbody = document.getElementById('filesTableBody');
    tbody.innerHTML = '<tr><td colspan="5">Loading...</td></tr>';
    try {
        const files = query ? await apiSearchFiles(user.username, query) : await apiGetFiles(user.username);
        tbody.innerHTML = '';
        if (files.length === 0) {
            tbody.innerHTML = '<tr><td colspan="5">No files found.</td></tr>';
            return;
        }
        
        files.forEach(f => {
            const tr = document.createElement('tr');
            tr.innerHTML = `
                <td>${f.fileId}</td>
                <td>${f.fileName}</td>
                <td>${f.owner}</td>
                <td><span class="badge">${f.visibility}</span></td>
                <td>
                    <button class="btn btn-primary" style="padding: 0.25rem 0.75rem; font-size: 0.875rem; margin-right: 0.5rem;" onclick="downloadFile(${f.fileId})">Download</button>
                    ${(user.role === 'Admin' || f.owner === user.username) ? `<button class="btn btn-danger" style="padding: 0.25rem 0.75rem; font-size: 0.875rem;" onclick="deleteFile(${f.fileId})">Delete</button>` : ''}
                </td>
            `;
            tbody.appendChild(tr);
        });
    } catch (e) {
        tbody.innerHTML = `<tr><td colspan="5" style="color:red">Error loading files</td></tr>`;
    }
}

document.getElementById('uploadForm').addEventListener('submit', async (e) => {
    e.preventDefault();
    const filePath = document.getElementById('uploadPath').value.trim();
    try {
        await apiUploadFilePath(user.username, filePath);
        e.target.reset();
        loadFiles();
    } catch (err) {
        alert("Upload failed: " + err.message);
    }
});

document.getElementById('searchBtn').addEventListener('click', () => {
    const q = document.getElementById('searchInput').value;
    loadFiles(q);
});

window.downloadFile = async (id) => {
    try {
        const res = await apiDownloadFile(user.username, id);
        alert(res.message + "\nFile: " + res.file.fileName);
    } catch (err) {
        alert("Download failed: " + err.message);
    }
};

window.deleteFile = async (id) => {
    if(!confirm("Are you sure you want to delete this file?")) return;
    try {
        await apiDeleteFile(user.username, id);
        loadFiles();
    } catch (err) {
        alert("Delete failed: " + err.message);
    }
};

loadFiles();
