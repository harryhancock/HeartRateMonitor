#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
// Use ESP32_Heltec_V2 with platformio project

const int proxPin = 37; // Proximity sensor is connected to GPIO 36 (Analog ADC1_0*) 
const int filterPin = 36; //
const int redPin = 3; //red LED at pin 3
const int greenPin = 0; //green LED at pin 0
double bpm = 0;
double voltage = 0;
double proxValue = 0; // variable for storing the proximity sensor value
double prev = 0;
double beats = 0;
unsigned long timediff = 0;
double filter = 0;
unsigned long timer;
unsigned long start;
boolean first;

//Your Domain name with URL path or IP address with path
const char* serverName = "http://35.188.168.116/main/heart_rate/";
const char* ssid = "honor10";
const char* password = "kA971@KsN";

/*Key to sensor operation -------------------------------------------
Make sure to press little LED, then position index finger parallel to sensor
*/

void PostRequest(String bpm){
  //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverName);

      // Specify content-type header
      //http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Data to send with HTTP POST
      //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&value1=24.25&value2=49.54&value3=1005.14";           
      // Send HTTP POST request
      //int httpResponseCode = http.POST(httpRequestData);
      
      // If you need an HTTP request with a content type: application/json, use the following:
      //http.addHeader("Content-Type", "application/json");
      //int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

      // If you need an HTTP reque
      //post with a content type: text/plain
      http.addHeader("Content-Type", "text/plain");
      int httpResponseCode = http.POST("hboy"+bpm);
     
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
        
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
}

void setup() {
Serial.begin(115200);

// server setup part
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");

//heart rate part
  Serial.begin(9600); //set baud rate of serial output from esp32
  delay(1000); //1 second delay
  pinMode(redPin, OUTPUT); //set the LED pins to output for red, yellow and green
  pinMode(greenPin, OUTPUT);

  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  first = true; //set to true, so we capture first time value
}

void loop() { //loop continuously
  //Reading proximity sensor value
  proxValue = analogRead(proxPin);
  filter = analogRead(filterPin);
  prev = voltage;
  voltage = (filter * 3.3)/ 1024;
  //Serial.println("prox: " + String(proxValue));
  //Serial.println("filter: " + String(voltage));

  //Serial.println(millis()/1000);
  
  if ((voltage > 0) && (voltage <= 1) && (voltage != prev)) {
    beats++; //if correct voltage level, record a heartbeat
    Serial.println(voltage);
    if (first == true) { //if first timestamp, 
      start = millis()/1000; //capture time at first beat measurement
      first = false; //next loop don't rewrite variable first
      Serial.println("Started");
      timer = 0; //reset timer and timediff
      timediff = 0;
    } else {
      timer = millis()/1000; //measures time since start in seconds, millis is in milliseconds
      timediff = timer - start; //get time difference between start and now
      if (timediff >= 60){ //if time difference is roughly minute
        bpm = beats; //beats in a minute, so set to bpm
        // /3 because every beat is 3 voltage changes
        Serial.println("bpm: " + String(bpm));
        first = true; //restart counting time so get start time again in first if
        PostRequest(String(bpm)); //send bpm POST request to server
      }
    }
  }
  
  delay(100);
  
  if (proxValue < 1000){
    //Serial.println("touching"); //value of 1000, object is touching the proximity sensor
    //digitalWrite(yellowPin, LOW);
    digitalWrite(redPin, LOW);
    delay(10);
    digitalWrite(greenPin, HIGH); //set greenLED on
  } else if (proxValue > 3000){ //value of 3000, object is far away
    //Serial.println("far away");
    digitalWrite(greenPin, LOW);
    delay(10);
    digitalWrite(redPin, HIGH); //set red LED on
  }
}
