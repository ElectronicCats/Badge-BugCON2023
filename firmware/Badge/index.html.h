const char* index_html = R"=====(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="styles.css">
    <title>Control de LEDs RGB</title>
</head>
<body>

    <div class="container">
        <label for="red">Rojo: <span id="redValue">0</span></label>
        <input type="range" id="red" min="0" max="255" oninput="updateValue('red', 'redValue')">

        <label for="green">Verde: <span id="greenValue">0</span></label>
        <input type="range" id="green" min="0" max="255" oninput="updateValue('green', 'greenValue')">

        <label for="blue">Azul: <span id="blueValue">0</span></label>
        <input type="range" id="blue" min="0" max="255" oninput="updateValue('blue', 'blueValue')">

        <button onclick="play()">Aceptar</button>
    </div>

    <script src="main.js"></script>
</body>
</html>
)=====";