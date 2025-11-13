const uno = document.querySelector('#uno');
const esp = document.querySelector('#esp');
const clavier = document.querySelector('#clavier');
const lcd = document.querySelector('#lcd');
const relais = document.querySelector('#relais');
const foot = document.querySelector('#foot');
const jumper = document.querySelector('#jumper');
const uno_list = [
    "/static/img/arduino.png",
    "/static/img/arduino.jpg",
    "/static/img/arduino1.png",
    "/static/img/uno.png"
]; // Liste des images pour l'Arduino Uno

const clavier_list = [
    "/static/img/matrice.png",
    "/static/img/clavier.jpg",
    "/static/img/matrice2.png",
    "/static/img/clavier2.jpg"
]; // Liste des images pour le clavier matriciel

const lcd_list = [
    "/static/img/lcd.png", 
    "/static/img/lcd2.png", 
    "/static/img/lcd3.png", 
    "/static/img/lcd4.png"
]; // Liste des images pour l'écran LCD

const esp_list = [
    "/static/img/esp8266.png",
    "/static/img/esp8266_2.png",
    "/static/img/esp8266_3.png",
    "/static/img/esp8266_4.png"
]; // Liste des images pour l'ESP8266

const relais_list = [
    "/static/img/relais.jpg",
    "/static/img/relais2.jpg",
    "/static/img/relais3.png",
    "/static/img/relais4.png"
]; // Liste des images pour le module relais

const jumper_list = [
    "/static/img/jumpers1.jpg",
    "/static/img/jumpers2.jpg",
    "/static/img/jumpers3.jpg"
]; // Liste des images pour les jumpers

var j = 0;
const uno_change = async () => {
    if (j===uno_list.length)
        j =0;
    uno.src = uno_list[j];
    j++;
}
var k = 0;
const esp_change = async () => {
    if (k===esp_list.length)
        k =0;
    esp.src = esp_list[k];
    k++;
}


var i = 0;
const lcd_change = async () => {
    if (i===lcd_list.length)
        i =0;
    lcd.src = lcd_list[i];
    i++;
}
var l = 0;
const clavier_change = async () => {
    if (l===clavier_list.length)
        l =0;
    clavier.src = clavier_list[l];
    l++;
}
var m = 0;
const relais_change = async () => {
    if (m===relais_list.length)
        m =0;
    relais.src = relais_list[m];
    m++;
}
var n = 0;
const jumper_change = async () => {
    if (n===jumper_list.length)
        n =0;
    jumper.src = jumper_list[n];
    n++;
}

setInterval(jumper_change, 5300);
setInterval(relais_change, 5200);
setInterval(clavier_change, 5100);
setInterval(lcd_change, 5000);
setInterval(uno_change, 5090);
setInterval(esp_change, 5900);

const old = new Date().getFullYear();
foot.innerHTML = `&copy; ${old} Serrure Electronique. Tout droits réservés.`