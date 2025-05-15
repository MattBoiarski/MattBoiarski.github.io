document.addEventListener('DOMContentLoaded', () => {
    ScrollReveal().reveal('#hero', {
        delay: 200,
        distance: '50px',
        origin: 'bottom'
    });

    ScrollReveal().reveal('#about', {
        delay: 300,
        distance: '50px',
        origin: 'left'
    });

    ScrollReveal().reveal('#projects article', {
        delay: 400,
        distance: '50px',
        origin: 'right',
        interval: 200
    });

    ScrollReveal().reveal('#skills .skills-section', {
        delay: 300,
        distance: '50px',
        origin: 'bottom',
        interval: 200
    });

    ScrollReveal().reveal('#contact', {
        delay: 400,
        distance: '50px',
        origin: 'bottom'
    });
});
