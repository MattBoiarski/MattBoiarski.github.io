document.addEventListener('DOMContentLoaded', function () {
    const form = document.getElementById('contact-form');
    const thankYouModal = document.getElementById('thank-you-modal');
    const closeThankYouBtn = document.getElementById('close-thank-you');

    form.addEventListener('submit', function(event) {
        event.preventDefault(); // Prevent default form submission[10][13]

        const name = document.getElementById('user_name').value;
        const email = document.getElementById('user_email').value;
        const message = document.getElementById('message').value;

        const subject = encodeURIComponent(`Message from ${name}`);
        const body = encodeURIComponent(`Sender Email: ${email}\n\n${message}`);

        // Gmail compose URL
        const gmailUrl = `https://mail.google.com/mail/?view=cm&fs=1&to=21boiarskim@gmail.com&su=${subject}&body=${body}`;

        // Open Gmail compose in a new tab
        window.open(gmailUrl, '_blank');

        // Show thank you modal
        thankYouModal.style.display = 'flex';
    });

    closeThankYouBtn.addEventListener('click', function() {
        thankYouModal.style.display = 'none';
        form.reset();
    });
});
