/* -------- WiFi Connection Function -------- */
void connectToWifi() {

  // Display WiFi connection status on screen
  lcd.print(5, 5, "Connecting to WiFi", 2, WHITE, BLACK);
  lcd.print(5, 25, ".", 2, WHITE, BLACK);

  // Start WiFi connection
  WiFi.begin(ssid, password);

  // Wait until WiFi is connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.print(".");
  }

  // Clear screen after successful connection
  lcd.fillScreen(BLACK);
  lcd.print(10, 20, "Connected to WiFi", 2, GREEN, BLACK);
  delay(1000);

  // Display initial screen content (IP address by default)
  updateDisplay();
}
//=================================================