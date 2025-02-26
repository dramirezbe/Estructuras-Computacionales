// Capturamos el slider y el valor
const slider = document.getElementById('slider-lumens');
const valorSlider = document.getElementById('valor-slider');

// Actualizamos el valor mostrado cuando el usuario mueve el slider
slider.addEventListener('input', function() {
    valorSlider.textContent = slider.value;
    enviarLumens(slider.value);
});

// Función para simular el envío del valor al backend
function enviarLumens(lumens) {
    console.log(`Enviando lumens: ${lumens}`);
    // Aquí más adelante haremos una petición al backend
}


