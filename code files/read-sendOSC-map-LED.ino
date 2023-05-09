//this code is a mesh of example library and tutorials of this article: https://dronebotworkshop.com/esp32-intro/#Simple_WiFi_Server

#define PIN_RED    23 // GIOP23
#define PIN_GREEN  22 // GIOP22
#define PIN_BLUE   21 // GIOP21
#define FSR_PIN    36 // GIOP36

#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

// Set up the WiFi and OSC connection
const char* ssid = "modul";
const char* password = "pyxx31d1060";
const IPAddress serverIP (172, 20, 10, 3);//(192, 168, 1, 146); // IP address of the receiving device
const unsigned int serverPort = 6574;
WiFiUDP udp;

void setup() {
   Serial.begin(115200);
   pinMode(PIN_RED,   OUTPUT);
   pinMode(PIN_GREEN, OUTPUT);
   pinMode(PIN_BLUE,  OUTPUT);

  // This initializes the sensor
   pinMode(FSR_PIN, INPUT);

  // This connects to wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

   // This starts the OSC connection
  udp.begin(WiFi.localIP(), 8888);
  Serial.print("Local IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // This reads the sensor value and maps it onto a 0-100 scale
  int sensorValue = analogRead(FSR_PIN);
  int mapVal = map(sensorValue, 0, 4095, 0, 100);


    // This packages and sends the sensor data via OSCS
  OSCMessage msg("/sensorValue");
  Serial.println(sensorValue); // print the mapped analog reading
  msg.add(mapVal);
  udp.beginPacket(serverIP, serverPort);
  msg.send(udp);
  udp.endPacket();
  msg.empty();

  //This makes the RGB led shine in proportion to the squeeze
  // Map the analog value to a range of 0-255
  int brightness = map(sensorValue, 0, 4095, 0, 255);

  // Set the RGB LED to the mapped brightness value
  analogWrite(PIN_RED,   brightness);
  analogWrite(PIN_GREEN, brightness);
  analogWrite(PIN_BLUE,  brightness);

  //I attatched lables to cluster the squeeze force in 4 groups
  if (sensorValue < 10)       // from 0 to 9
    Serial.println(" -> no pressure");
  else if (sensorValue < 200) // from 10 to 199
    Serial.println(" -> light touch");
  else if (sensorValue < 500) // from 200 to 499
    Serial.println(" -> light squeeze");
  else if (sensorValue < 800) // from 500 to 799
    Serial.println(" -> medium squeeze");
  else // from 800 to 1023
    Serial.println(" -> big squeeze");
  // // delay for 200 milliseconds before reading the sensor value value again
  delay(100);
}
