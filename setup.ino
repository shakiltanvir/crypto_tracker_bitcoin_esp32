void setup() 
{
  Serial.begin(115200);                                                       //Start the serial monitor
  
  pinMode(upLED, OUTPUT);                                                     //Define the LED pin outputs
  pinMode(downLED, OUTPUT);
 
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))                   //Connect to the display
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
 
  display.clearDisplay();                                                     //Clear the display
  display.setTextColor(SSD1306_WHITE);                                        //Set the text colour to white
  display.drawBitmap(0, 0, bitcoinLogo, 128, 64, WHITE);                             //Display bitmap from array
  display.display();
  delay(2000);
 
  display.clearDisplay();                                                     //Clear the display
  display.setTextSize(1);                                                     //Set display parameters
  display.setTextColor(WHITE);
  display.println("Connecting to WiFi...");
  display.display();
 
  WiFi.begin(ssid, password);
 
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED)                                        //Connect to the WiFi network
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
 
  display.println("Connected to: ");                                           //Display message once connected
  display.print(ssid);
  display.display();
  delay(1500);
  display.clearDisplay();
  display.display();
}
 
