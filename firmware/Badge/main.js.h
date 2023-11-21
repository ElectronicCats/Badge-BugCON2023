const char* main_js = R"=====(function updateValue(sliderId, displayId) {
    // Obtener el valor del slider
    var slider = document.getElementById(sliderId);
    var display = document.getElementById(displayId);

    // Mostrar el valor actual del slider junto a la etiqueta
    display.textContent = slider.value;
}

function aceptar() {
    // Obtener los valores finales de los sliders
    var redValue = document.getElementById('red').value;
    var greenValue = document.getElementById('green').value;
    var blueValue = document.getElementById('blue').value;

    // Hacer algo con los valores, por ejemplo, enviarlos al servidor para controlar los LEDs
    console.log("Valores RGB seleccionados: " + redValue + ", " + greenValue + ", " + blueValue);
    // Aquí podrías realizar una llamada AJAX para enviar los valores al servidor
}
)=====";