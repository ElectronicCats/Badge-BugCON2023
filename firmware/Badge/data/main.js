function updateValue(sliderId, displayId) {
    // Get the current value of the slider
    var slider = document.getElementById(sliderId);
    var display = document.getElementById(displayId);

    // Show the current value of the slider next to it
    display.textContent = slider.value;
}

function play() {
    var redValue = document.getElementById('red').value;
    var greenValue = document.getElementById('green').value;
    var blueValue = document.getElementById('blue').value;

    console.log("Valores RGB seleccionados: " + redValue + ", " + greenValue + ", " + blueValue);
}
