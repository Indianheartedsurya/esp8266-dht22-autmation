/**
   --Tarun Grover project
   ESP32 + DHT22 Example for Wokwi
   https://wokwi.com/arduino/projects/322410731508073042
*/

#define BLYNK_TEMPLATE_ID "----------"
#define BLYNK_TEMPLATE_NAME "-------"
#define BLYNK_AUTH_TOKEN "-------------------"
#define BLYNK_PRINT Serial

#include "DHTesp.h"
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <BlynkSimpleEsp32.h>
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

BlynkTimer timer;
DHTesp dhtSensor;
LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 16, 2);

const int DHT_PIN = 15;
const int PIR_PIN = 14;
const int RELAY_PIN = 4;
const int RELAY_PIN_1 =5;
int PIR_VALUE;
float temperature, humidity;




// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, 16, 2);

BLYNK_WRITE(V3){
if(param.asInt()==1){
  digitalWrite(RELAY_PIN_1, HIGH);
}else{
     digitalWrite(RELAY_PIN_1, LOW);
}
}





// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V3); 
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
   
  Blynk.virtualWrite(V0, PIR_VALUE);
  Blynk.virtualWrite(V1, temperature);
  Blynk.virtualWrite(V2, humidity);
  
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  // Blynk.virtualWrite(V1, millis() / 1000);
}

void setup() {
  Serial.begin(115200);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(RELAY_PIN_1, OUTPUT);

    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
  
 


}

void loop() {

   Blynk.run();
  timer.run();
 
  // TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  lcd.setCursor(0, 0);
  temperature = dhtSensor.getTemperature();
   humidity = dhtSensor.getHumidity();

  lcd.print( "Temp: " );
  lcd.print(temperature);
  lcd.print("C");

  lcd.setCursor(0, 1);  
  lcd.print( "Humidity: ");
  lcd.print( humidity);
  lcd.print( "%");
  
digitalWrite(RELAY_PIN, HIGH);
  PIR_VALUE=digitalRead(PIR_PIN);
  if(PIR_VALUE == LOW){
    digitalWrite(RELAY_PIN, LOW);
  }
  Serial.println(PIR_VALUE);
 
}
