const identifiant_label = document.getElementById('em');
const password_label = document.getElementById('pwd');
const identifiant_input = document.getElementById('identifiant');
const password_input = document.getElementById('password');
const submit_button = document.getElementById('submit-button');
const togglepass = document.querySelector('#voirMotdePasse');
document.addEventListener('click', function(e){
    if (e.target && (e.target.id === 'em' || e.target.id === 'identifiant')) {
        identifiant_label.style.transition = 'transform 0.3s ease';
        identifiant_label.style.transform = 'translateY(-3px)';
        identifiant_label.style.color = '#020000ff';
    } else if (e.target && identifiant_input.value === '') {
        identifiant_label.style.transition = 'transform 0.3s ease';
        identifiant_label.style.transform = 'translateY(192%)';
        identifiant_label.style.color = '#12111166';
    }
    if (e.target && (e.target.id === 'pwd' || e.target.id === 'password' || e.target.id === 'voirMotdePasse')) {
        togglepass.style.display = 'inline';
        password_label.style.transition = 'transform 0.3s ease';
        password_label.style.transform = 'translateY(-3px)';
        password_label.style.color = '#020000ff';
    } else if (e.target && password_input.value === '') {
        password_label.style.transition = 'transform 0.3s ease';
        password_label.style.transform = 'translateY(192%)';
        password_label.style.color = '#12111166';
    }
});

document.addEventListener('DOMContentLoaded', function(e){
    if (identifiant_input.value !== '') {
        identifiant_label.style.transform = 'translateY(-3px)';
        identifiant_label.style.color = '#020000ff';
    }
    if (password_input.value !== '') {
        password_label.style.transform = 'translateY(-3px)';
        password_label.style.color = '#020000ff';
        togglepass.style.display = 'none';
    }
});

togglepass.addEventListener('click', function() {
    const isPasswordVisible = password_input.getAttribute('type') === 'password' ? 'text' : 'password';
    password_input.setAttribute('type', isPasswordVisible);
    togglepass.classList.toggle('bi-eye');
});