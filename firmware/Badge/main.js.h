const char* main_js = R"=====(let currentLocation = localStorage.getItem("location");
let reload = localStorage.getItem("reload");
let reloaded = localStorage.getItem("reloaded");
console.log(`Current location: ${currentLocation}`);

// Update current page location
function updateLocation(location) {
    localStorage.setItem("location", location);
    localStorage.setItem("reload", true);
    localStorage.setItem("reloaded", false);
    window.location.reload();
}

function reloadPageListener(page, delay) {
    if (reload == "true") {
        // Update location and reload page
        if (reloaded == "false") {
            localStorage.setItem("reloaded", true);
            window.location.href = currentLocation;
        }

        // Reload page again with the new location
        if (reloaded == "true") {
            setTimeout(() => {
                // alert("Second reload");
                localStorage.setItem("reload", false);
                window.location.replace(page);
            }, delay);
        }
    }
}

function updateValue(sliderId, displayId) {
    // Get the current value of the slider
    let slider = document.getElementById(sliderId);
    let display = document.getElementById(displayId);

    // Show the current value of the slider next to it
    display.textContent = slider.value;
}

function play() {
    let redValue = document.getElementById('red').value;
    let greenValue = document.getElementById('green').value;
    let blueValue = document.getElementById('blue').value;

    localStorage.setItem("red", redValue);
    localStorage.setItem("green", greenValue);
    localStorage.setItem("blue", blueValue);

    console.log("Valores RGB seleccionados: " + redValue + ", " + greenValue + ", " + blueValue);
    updateLocation(`index.html?red=${redValue}&green=${greenValue}&blue=${blueValue}`);
}

reloadPageListener("index.html", 500);

window.addEventListener("load", function () {
    // Update the current value of the sliders by the one stored in the local storage
    let redValue = localStorage.getItem("red");
    let greenValue = localStorage.getItem("green");
    let blueValue = localStorage.getItem("blue");

    document.getElementById("red").value = redValue;
    document.getElementById("green").value = greenValue;
    document.getElementById("blue").value = blueValue;

    // Update the current value of the sliders
    updateValue("red", "redValue");
    updateValue("green", "greenValue");
    updateValue("blue", "blueValue");
});
)=====";