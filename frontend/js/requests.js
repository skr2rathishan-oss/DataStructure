import { apiCreateAccessRequest, apiGetNextRequest, apiProcessRequest } from './api.js';

const user = JSON.parse(localStorage.getItem('currentUser'));
if (!user) window.location.href = 'index.html';

document.getElementById('logoutBtn').addEventListener('click', () => {
    localStorage.removeItem('currentUser');
    window.location.href = 'index.html';
});

document.getElementById('requestForm').addEventListener('submit', async (e) => {
    e.preventDefault();
    const ownerName = document.getElementById('ownerName').value;
    const fileId = document.getElementById('fileId').value;
    
    try {
        await apiCreateAccessRequest(user.username, ownerName, fileId);
        alert('Access request added to queue!');
        loadNextRequest();
    } catch (err) {
        alert('Error: ' + err.message);
    }
});

let currentPendingRequest = null;

async function loadNextRequest() {
    const infoDiv = document.getElementById('nextRequestInfo');
    const btnA = document.getElementById('btnApprove');
    const btnR = document.getElementById('btnReject');
    
    try {
        const res = await apiGetNextRequest();
        if (res.hasRequest) {
            currentPendingRequest = res.request;
            infoDiv.innerHTML = `
                <strong>Request ID:</strong> ${res.request.requestId} <br>
                <strong>Requester:</strong> ${res.request.requesterUsername} <br>
                <strong>File ID:</strong> ${res.request.fileId}
            `;
            btnA.disabled = false;
            btnR.disabled = false;
        } else {
            currentPendingRequest = null;
            infoDiv.innerHTML = 'No pending requests in queue.';
            btnA.disabled = true;
            btnR.disabled = true;
        }
    } catch (err) {
        infoDiv.innerHTML = `<span style="color:var(--danger-red)">Failed to load request: ${err.message}</span>`;
    }
}

document.getElementById('btnApprove').addEventListener('click', () => process(true));
document.getElementById('btnReject').addEventListener('click', () => process(false));

async function process(approved) {
    if (!currentPendingRequest) return;
    try {
        await apiProcessRequest(user.username, approved);
        alert(approved ? 'Request Approved' : 'Request Rejected');
        loadNextRequest();
    } catch (err) {
        alert('Process error: ' + err.message);
    }
}

loadNextRequest();
