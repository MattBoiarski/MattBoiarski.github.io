function redirectToEmail(event) {
    event.preventDefault();
    const name = document.getElementById('user_name').value;
    const email = document.getElementById('user_email').value;
    const message = document.getElementById('message').value;
    const subject = encodeURIComponent(name);
    const body = encodeURIComponent(`From: ${name}\nEmail: ${email}\n\n${message}`);
    window.location.href = `mailto:your-email@example.com?subject=${subject}&body=${body}`;
}
