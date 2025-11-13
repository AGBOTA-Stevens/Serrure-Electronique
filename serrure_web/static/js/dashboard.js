const badge = document.getElementById('badge');
const submit_badge = document.getElementById('submit_badge');
const color = ["green", "red"]
const text = ['Activer le badge', 'Désactiver le badge']

let state = badge.innerText === 'Désactiver le badge' ? 1 : 0;
badge.style.backgroundColor = color[state];
if (state === 1) {
    badge.style.fontSize = "0.9rem";
} else {
    badge.style.fontSize = "1rem";
}
document.getElementById('badge_input').checked = state === 1;

badge.addEventListener('click', () => {
    const submit = confirm("Voulez-vous vraiment changer l'état du badge ?");
    if (submit) {
        state = (state + 1) % 2;
        badge.style.transition = "all 0.3s ease";
        badge.style.backgroundColor = color[state];
        if (state === 1) {
            badge.style.fontSize = "0.9rem";
        } else {
            badge.style.fontSize = "1rem";
        }
        badge.innerText = text[state];
        document.getElementById('badge_input').checked = state === 1;
        submit_badge.click();
    } else {
        return;
    }
});