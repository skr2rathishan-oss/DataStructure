import { apiLogin } from './api.js';

document.getElementById('loginForm')?.addEventListener('submit', async (e) => {
    e.preventDefault();
    const u = document.getElementById('username').value;
    const p = document.getElementById('password').value;
    const errorMsg = document.getElementById('errorMsg');
    
    try {
        const res = await apiLogin(u, p);
        if (res.success) {
            localStorage.setItem('currentUser', JSON.stringify(res.user));
            window.location.href = 'files.html';
        } else {
            errorMsg.textContent = res.error || 'Login failed';
        }
    } catch (err) {
        errorMsg.textContent = 'Server connection error';
    }
});
