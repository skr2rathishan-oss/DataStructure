import { apiGrantPermission, apiRevokePermission, apiCheckPermission } from './api.js';

const user = JSON.parse(localStorage.getItem('currentUser'));
if (!user) window.location.href = 'index.html';

document.getElementById('logoutBtn').addEventListener('click', () => {
    localStorage.removeItem('currentUser');
    window.location.href = 'index.html';
});

document.getElementById('btnGrant').addEventListener('click', async () => {
    const targetUser = document.getElementById('targetUser').value;
    const fileId = document.getElementById('fileId').value;
    try {
        await apiGrantPermission(user.username, targetUser, fileId);
        alert('Permission granted');
    } catch (err) {
        alert('Error: ' + err.message);
    }
});

document.getElementById('btnRevoke').addEventListener('click', async () => {
    const targetUser = document.getElementById('targetUser').value;
    const fileId = document.getElementById('fileId').value;
    try {
        await apiRevokePermission(user.username, targetUser, fileId);
        alert('Permission revoked');
    } catch (err) {
        alert('Error: ' + err.message);
    }
});

document.getElementById('checkForm').addEventListener('submit', async (e) => {
    e.preventDefault();
    const fileId = document.getElementById('checkFileId').value;
    const resultDiv = document.getElementById('checkResult');
    try {
        const res = await apiCheckPermission(user.username, fileId);
        if (res.allowed) {
            resultDiv.innerHTML = `<span style="color: green;">Allowed: ${res.reason}</span>`;
        } else {
            resultDiv.innerHTML = `<span style="color: var(--danger-red);">Denied: ${res.reason}</span>`;
        }
    } catch (err) {
        resultDiv.innerHTML = `<span style="color: var(--danger-red);">Error: ${err.message}</span>`;
    }
});
