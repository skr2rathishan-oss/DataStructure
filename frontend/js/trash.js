import { apiGetTrashFiles, apiRestoreTrash } from './api.js';

const user = JSON.parse(localStorage.getItem('currentUser'));
if (!user) window.location.href = 'index.html';

document.getElementById('logoutBtn').addEventListener('click', () => {
    localStorage.removeItem('currentUser');
    window.location.href = 'index.html';
});

async function loadTrashFiles() {
    const tbody = document.getElementById('trashTableBody');
    tbody.innerHTML = '<tr><td colspan="5">Loading...</td></tr>';

    try {
        const files = await apiGetTrashFiles(user.username);

        if (files.length === 0) {
            tbody.innerHTML = '<tr><td colspan="5">Trash is empty.</td></tr>';
            return;
        }

        tbody.innerHTML = '';
        files.forEach((file) => {
            const tr = document.createElement('tr');
            tr.innerHTML = `
                <td>${file.fileId}</td>
                <td>${file.fileName}</td>
                <td>${file.owner}</td>
                <td><span class="badge">${file.visibility}</span></td>
                <td>${file.fileSize} bytes</td>
            `;
            tbody.appendChild(tr);
        });
    } catch (err) {
        tbody.innerHTML = '<tr><td colspan="5" style="color:red">Error loading trash</td></tr>';
    }
}

document.getElementById('btnRestore').addEventListener('click', async () => {
    try {
        const res = await apiRestoreTrash(user.username);
        alert('Restored file ID: ' + res.fileId);
        await loadTrashFiles();
    } catch (err) {
        alert('Restore error: ' + err.message);
    }
});

loadTrashFiles();
