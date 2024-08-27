#include <WiFiNINA.h>
#include <Wire.h>
#include <BH1750.h>

char ssid [] = "realme 9 5G Speed Edition" ;
char pass [] = "12345678";

WiFiClient client;

BH1750 lightMeter; // Create an instance of the BH1750 light sensor library const 
int ledCheck = 2;
String queryString = "?value1=57&value2=25"; 
char HOST_NAME[] = "maker.ifttt.com"; // Website
String PATH_NAME = "https://maker.ifttt.com/trigger/LightDetect/with/key/gGi5lX65RsckYaoBo3yI3L7bTpujJWBOdDY4lrp0eLF"; // Key



void setup() 
{
  WiFi.begin(ssid, pass);
  Serial.begin(9600); 
  pinMode(ledCheck, OUTPUT);

  Wire.begin();
  lightMeter.begin();

  Serial.println(F("BH1750 Test begin"));

  // Attempt to connect to WiFi
  connectWiFi();
}

void loop() {
  float lux = lightMeter.readLightLevel();
  Serial.print("Lux: ");
  Serial.println(lux);
  
  if (lux >= 500) {
    triggerWebhook("Sunlight");
  } else if (lux < 10) {
    triggerWebhook("Sundown");
  }
  delay(1000); // Wait for 1  before reading light level again
}

void connectWiFi() {
  Serial.println("Attempting to connect to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, pass);
    delay(5000); // Wait 5 seconds
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void triggerWebhook(String eventName) 
{
  if (WiFi.status() != WL_CONNECTED) 
  {
    connectWiFi();
  }
  
  if (client.connect("maker.ifttt.com", 80)) {
    client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println();
    delay(500);
    client.stop();
    Serial.println("Webhook triggered successfully");
  } else {
    Serial.println("Failed to connect to webhook server");
  }
}