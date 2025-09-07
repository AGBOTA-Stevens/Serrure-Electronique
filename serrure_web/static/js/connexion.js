const identifiant_label = document.getElementById('em');
const password_label = document.getElementById('pwd');
const identifiant_input = document.getElementById('identifiant');
const password_input = document.getElementById('password');
const submit_button = document.getElementById('submit-button');
const togglepass = document.querySelector('#voirMotdePasse');
const foot = document.querySelector('#foot');
identifiant_input.addEventListener('focus', function() {
    identifiant_label.style.transition = 'all 0.3s ease';
    identifiant_label.style.transform = 'translateY(98%)';
    identifiant_label.style.fontSize = '0.9rem';
    identifiant_label.style.width = '105px';
    identifiant_label.style.backgroundColor = '#ffff'
    identifiant_label.style.color = '#656fd7';
});
identifiant_input.addEventListener('blur', function() {
    if (identifiant_input.value === '') {
        identifiant_label.style.transition = 'all 0.3s ease';
        identifiant_label.style.transform = 'translateY(190%)';
        identifiant_label.style.fontSize = '1.2rem'
        identifiant_label.style.width = '90%';
        identifiant_label.style.color = '#02000080';
    }
});
password_input.addEventListener('focus', function() {
    password_label.style.transition = 'all 0.3s ease';
    password_label.style.transform = 'translateY(98%)';
    password_label.style.color = '#656fd7';
    password_label.style.backgroundColor = '#ffff';
    password_label.style.width = '120px';
    password_label.style.fontSize = '0.9rem';
    togglepass.style.display = 'inline';
});
password_input.addEventListener('blur', function() {
    if (password_input.value === '') {
        //password_label.style.transition = 'all 0.3s ease';
        password_label.style.transform = 'translateY(190%)';
        password_label.style.color = '#02000080';
        password_label.style.fontSize = '1.2rem';
        password_label.style.width = '90%'
        password_input.setAttribute('type', 'password');
        togglepass.style.display = 'none';
        togglepass.classList.remove('bi-eye');
    }
});

document.addEventListener('DOMContentLoaded', function(e){
    if (identifiant_input.value !== '') {
        identifiant_label.style.transform = 'translateY(100%)';
        identifiant_label.style.fontSize = '0.9rem';
        identifiant_label.style.backgroundColor = '#ffffff'
        identifiant_label.style.color = '#656fd7';
        identifiant_input.style.borderColor = '#656fd7';
    }
    if (password_input.value !== '') {
        password_label.style.transition = 'all 0.3s ease';
        password_label.style.transform = 'translateY(100%)';
        password_label.style.color = '#656fd7';
        password_label.style.backgroundColor = '#ffffff';
        password_label.style.fontSize = '0.9rem';
        password_input.style.borderColor = "#656fd7";
    } else {
        togglepass.style.display = 'none';
    }
});
document.addEventListener('load', (e) => {
    if (password_input.value === '') {
        togglepass.style.display = 'none';
    }
})
togglepass.addEventListener('click', function(e) {
    e.preventDefault();
    const isPasswordVisible = password_input.getAttribute('type') === 'password' ? 'text' : 'password';
    password_input.setAttribute('type', isPasswordVisible);
    togglepass.classList.toggle('bi-eye');
});


const old = new Date().getFullYear();
foot.innerHTML = `&copy; ${old} Serrure Electronique. Tout droits réservés.`
