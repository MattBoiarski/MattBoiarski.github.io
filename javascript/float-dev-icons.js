document.addEventListener("DOMContentLoaded", function() {
    const skillIcons = Array.from(document.querySelectorAll(".skills-icons .skill-icon img"));
    const svgContainer = document.querySelector(".container .svg-icons");

    if (!skillIcons.length || !svgContainer) {
        console.error("Skill icons or SVG container not found!");
        return;
    }
    const randomSkillCount = Math.floor(Math.random() * 6) + 5; // Random count between 5 and 10
    svgContainer.innerHTML = "";
    skillIcons.sort(() => Math.random() - 0.5);
    for (let i = 0; i < randomSkillCount && i < skillIcons.length; i++) {
        const skill = skillIcons[i].cloneNode(true);
        skill.classList.add("svg");
        skill.addEventListener("mouseenter", () => { skill.classList.add("wiggle"); });
        skill.addEventListener("mouseleave", () => { skill.classList.remove("wiggle"); });
        svgContainer.appendChild(skill);
    }
});
