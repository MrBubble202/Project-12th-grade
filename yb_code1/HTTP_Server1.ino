/* -------- HTTP Server Response -------- */
void sendResponse() {
  float hum = currentHum;
  float temp = currentTemp;

  // Validate sensor readings
  if (isnan(hum) || isnan(temp)) {
    server.send(500, "text/plain", "Failed to read from DHT sensor!");
    return;
  }

  // Build dynamic HTML page with sensor data
  String htmlPage = R"rawliteral(
<!DOCTYPE html><html><head><meta charset='UTF-8'>
<meta http-equiv='refresh' content='3'>
<title>Greenhouse Data</title>
<style>
  body { font-family: Arial, sans-serif; margin: 20px; background-color: #f0f0f0; }
  .container { display: flex; flex-wrap: wrap; gap: 20px; }
  .sensor-section { background-color: white; padding: 20px; border-radius: 10px;
                    box-shadow: 0 2px 5px rgba(0,0,0,0.1); flex: 1; min-width: 250px; }
  .top-row { display: flex; gap: 20px; margin-bottom: 20px; }
  .left-column { flex: 1; }
  .center-column { flex: 1; text-align: center; }
  h1 { color: #333; margin-top: 0; font-size: 1.5em; }
  h2 { color: #666; font-size: 1.1em; }
  img { margin: 10px 0; }
</style>
</head><body>

<div class='top-row'>
  <div class='sensor-section left-column'>
    <h1>Raindrops Module:</h1>
    <img src='https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcQpNspr1antNGAkaaCb07VY_C3RYMGWF50YAQ&s' width='150'>
    <h2>If water spilled: )rawliteral" + String(!waterValue == 1 ? "Yes" : "No") + R"rawliteral(</h2>
  </div>

  <div class='sensor-section center-column'>
    <h1>Light Sensor:</h1>
    <img src='https://see-sys.co.il/wp-content/uploads/2025/03/ldr_sensor_module_1_.jpg' width='150'>
    <h2>Light Level: )rawliteral" + String(lightValue < 2000 ? "Good" : "Not Good") + R"rawliteral(</h2>
  </div>
</div>

<div class='top-row'>
  <div class='sensor-section left-column'>
    <h1>DHT22:</h1>
    <img src='https://o.lnwfile.com/_/o/_raw/zp/f9/qc.png' width='150'>
    <h2>Temperature: )rawliteral" + String(temp, 1) + R"rawliteral( °C</h2>
    <h2>Humidity: )rawliteral" + String(hum, 1) + R"rawliteral( %</h2>
    <h2>Motor Status: )rawliteral" + String(temp > 25 ? "ON" : "OFF") + R"rawliteral(</h2>
  </div>

  <div class='sensor-section center-column'>
    <h1>Water Level Sensor:</h1>
    <img src='https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcQqiwcOUQKCWqRZcUqFUMTOnlqUz95j7Vn3eQ&s' width='150'>
    <h2>Status: )rawliteral" + String(waterLevelValue > 2500 ? "Good" : "Bad") + R"rawliteral(</h2>
  </div>
</div>

</body></html>
)rawliteral";

  // Send HTML page to client
  server.send(200, "text/html", htmlPage);
}

/* -------- Web Server Setup -------- */
void setupServer() {
  server.on("/", HTTP_GET, []() {
    sendResponse();
  });
}
//=================================================