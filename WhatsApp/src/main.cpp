#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <UrlEncode.h>

//Your WiFi credentials
const char* ssid = "MY WIFI";
const char *password = "MY PASSWORD";

String phoneNumber = "MY PHONE NUMBER"; //Your international phone number, for example +48123456789 for Poland
String apiKey = "MY API KEY"; //Your WhatsApp apikey

void sendMessage(String msg)
{
  // Data to send with HTTP POST
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(msg); 
  HTTPClient http;
  http.begin(url);

  //HTTP content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  //Send HTTP POST request
  int httpResponseCode = http.POST(url);
  Serial.print("HTTP response code: ");
  Serial.println(httpResponseCode);
  
  //End of data transmission
  http.end();
}

void setup() 
{
  Serial.begin(115200);
  //Connecting with WiFi
  WiFi.begin(ssid, password); 
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print('.');
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A3, INPUT);
}

void loop() 
{
  float sensor_voltage;
  float RS_gas;
  float R0 = 0.7; //Calculated after the calibration
  float ratio;
  float sensor_read = analogRead(A0); //Reading from the sensor
  int alarm = 0;

  sensor_voltage = (sensor_read / 1024) * 5.0; //From the documentation
  RS_gas = (5.0 - sensor_voltage) / sensor_voltage;
  ratio = RS_gas / R0;

  Serial.print("sensor_voltage = "); 
  Serial.println(sensor_voltage); 
  Serial.print("RS_ratio = "); 
  Serial.println(RS_gas); 
  Serial.print("Rs/R0 = "); 
  Serial.println(ratio); 
  Serial.print("\n\n");
  alarm = digitalRead(A3);
  if (alarm == 1)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    sendMessage("Warning: combustible gas concentration too high!"); //If the gas concentration is too high the LED turns on and the message is send
  }
  else if (alarm == 0)
    digitalWrite(LED_BUILTIN, LOW);
  delay(5000); 
}