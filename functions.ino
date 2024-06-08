void bitcoinParse()
{
  Serial.print("Connecting to "); 
    Serial.println(url);
 
  http.begin(url);
  int httpCode = http.GET();                                                            //Get crypto price from API
  StaticJsonDocument<2000> doc;
  DeserializationError error = deserializeJson(doc, http.getString());
 
  if (error)                                                                            //Display error message if unsuccessful
  {
    Serial.print(F("deserializeJson Failed"));
    Serial.println(error.f_str());
    delay(2500);
    return;
  }
 
  Serial.print("HTTP Status Code: ");
  Serial.println(httpCode);
 
  String BTCUSDPrice = doc["bpi"]["USD"]["rate_float"].as<String>();                    //Store crypto price and update date in local variables
  String lastUpdated = doc["time"]["updated"].as<String>();
  http.end();
}
void historyParse()
{
    Serial.print("Getting history...");
  StaticJsonDocument<2000> historyDoc;
  http.begin(historyURL);                                                               //Get historical crypto price from API
  int historyHttpCode = http.GET();
  DeserializationError historyError = deserializeJson(historyDoc, http.getString());
 
  if (historyError) {                                                                   //Display error message if unsuccessful
    Serial.print(F("deserializeJson(History) failed"));
    Serial.println(historyError.f_str());
    delay(2500);
    return;
  }
 
  Serial.print("History HTTP Status Code: ");
  Serial.println(historyHttpCode);
  JsonObject bpi = historyDoc["bpi"].as<JsonObject>();
  double yesterdayPrice;
  for (JsonPair kv : bpi) {
    yesterdayPrice = kv.value().as<double>();                                           //Store yesterday's crypto price
  }

    Serial.print("BTCUSD Price: ");                                                       //Display current price on serial monitor
  Serial.println(BTCUSDPrice.toDouble());
 
  Serial.print("Yesterday's Price: ");                                                  //Display yesterday's price on serial monitor
  Serial.println(yesterdayPrice);
 

}

void changeAlert(){ 
  if (isUp)                                                                             //If price has increased from yesterday
  {
    percentChange = ((BTCUSDPrice.toDouble() - yesterdayPrice) / yesterdayPrice) * 100;
    digitalWrite(upLED, HIGH);
    digitalWrite(downLED, LOW);
  } 
  else                                                                                  //If price has decreased from yesterday
  {
    percentChange = ((yesterdayPrice - BTCUSDPrice.toDouble()) / yesterdayPrice) * 100;
    dayChangeString = dayChangeString + "-";
    digitalWrite(downLED, HIGH);
    digitalWrite(upLED, LOW);
  }
 
  Serial.print("Percent Change: ");                                                     //Display the percentage change on the serial monitor
  Serial.println(percentChange);
}
void oledDisplay(){
   display.clearDisplay();                                                               //Clear the OLED display
  display.setTextSize(1);
  printCenter("BTC/USD", 0, 0);                                                         //Display the comparison header
 
  display.setTextSize(1.5);
  printCenter("$" + BTCUSDPrice, 0, 20);                                                //Display the current price
                                           
  display.setTextSize(1);                                                               //Display the change percentage
  dayChangeString = dayChangeString + percentChange + "%";
  printCenter(dayChangeString, 0, 55);
  display.display();                                                                    //Execute the new display
 
  //http.end();                                                                           //End the WiFi connection
  //esp_sleep_enable_timer_wakeup(900000000);                                             //Sleep for 15 minutes
}
void printCenter(const String buf, int x, int y)                          //Function to centre the current price in the display width
{
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(buf, x, y, &x1, &y1, &w, &h);                     //Calculate string width
  display.setCursor((x - w / 2) + (128 / 2), y);                          //Set cursor to print string in centre
  display.print(buf);                                                     //Display string
}
