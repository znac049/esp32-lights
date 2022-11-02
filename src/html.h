const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>CLC - Chippers Light Controller</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://unpkg.com/@blaze/css@x.x.x/dist/blaze/blaze.css">
  <link rel="icon" href="data:,">
</head>
<body>
<div class="o-container o-container--small c-card u-high">
  <h2>Device Settings - %DEVICEIP%</h2>

<form class="c-card u-high">
  <div class="o-form-element">
    <label class="c-label">
      Device name
      <input class="c-field c-field--label" type="text" placeholder="%DEVICENAME%" />
    </label>
    <label class="c-label">
      Number of LEDs
      <input class="c-field c-field--label" type="text" placeholder="%NUMLEDS%" />
    </label>
    <label class="c-label">
      LED Order
      <select class="c-field c-field--label">
        %LEDORDER%
      </select>
    </label>
    <label class="c-label">
      Pattern
      <input class="c-field c-field--label" type="text" placeholder="" />
    </label>
  </div>
    <footer class="c-card__footer">
    <button type="button" class="c-button c-button--ghost c-button--info">
      Save
    </button>
  </footer>

</form


    <div class="card">
      <h2>Output - GPIO 2</h2>
      <p class="state">state: <span id="state">%STATE%</span></p>
      <p><button id="button" class="c-button">Toggle</button></p>
    </div>


<script>
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {
    var state;
    if (event.data == "1"){
      state = "ON";
    }
    else{
      state = "OFF";
    }
    document.getElementById('state').innerHTML = state;
  }
  function onLoad(event) {
    initWebSocket();
    initButton();
  }
  function initButton() {
    document.getElementById('button').addEventListener('click', toggle);
  }
  function toggle(){
    websocket.send('toggle');
  }
</script>
</body>
</html>)rawliteral";
