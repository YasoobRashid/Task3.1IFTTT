// WiFi credentials
char ssid [] = "realme 9 5G Speed Edition"; // SSID of the WiFi network
char pass [] = "12345678"; // Password for the WiFi network

WiFiClient client; // Create a WiFi client object

BH1750 lightMeter; // Create an instance of the BH1750 light sensor
const int ledCheck = 2; // Pin number for an LED (if used)
String queryString = "?value1=57&value2=25"; // Query string for the webhook
char HOST_NAME[] = "maker.ifttt.com"; // Host name for the webhook server
String PATH_NAME = "https://maker.ifttt.com/trigger/LightDetect/with/key/gGi5lX65RsckYaoBo3yI3L7bTpujJWBOdDY4lrp0eLF"; // Path name for the webhook including the key

void setup() 
{
  WiFi.begin(ssid, pass); // Start the WiFi connection using the provided credentials
  Serial.begin(9600); // Initialize serial communication for debugging
  pinMode(ledCheck, OUTPUT); // Set the LED pin as an output 

  Wire.begin(); // Initialize the I2C communication
  lightMeter.begin(); // Start the BH1750 light sensor

  Serial.println(F("BH1750 Test begin")); // Print a message indicating the test has begun

  // Attempt to connect to WiFi
  connectWiFi();
}

void loop() {
  float lux = lightMeter.readLightLevel(); // Read the light level in lux from the sensor
  Serial.print("Lux: "); // Print the label for the lux value
  Serial.println(lux); // Print the measured lux value
  
  // Check the lux value and trigger the appropriate webhook
  if (lux >= 500) {
    triggerWebhook("Sunlight"); // Trigger webhook for sunlight detection
  } else if (lux < 10) {
    triggerWebhook("Sundown"); // Trigger webhook for sundown detection
  }
  delay(1000); // Wait for 1 second before reading the light level again
}

void connectWiFi() {
  Serial.println("Attempting to connect to WiFi..."); // Print a message indicating the connection attempt
  while (WiFi.status() != WL_CONNECTED) { // Loop until connected to WiFi
    WiFi.begin(ssid, pass); // Re-attempt to connect to WiFi
    delay(5000); // Wait 5 seconds before trying again
    Serial.print("."); // Print a dot to indicate the connection is still in progress
  }
  Serial.println("\nConnected to WiFi"); // Print a message once connected
}

void triggerWebhook(String eventName) 
{
  if (WiFi.status() != WL_CONNECTED) 
  {
    connectWiFi(); // Ensure WiFi is connected before sending the webhook
  }
  
  // Attempt to connect to the webhook server
  if (client.connect("maker.ifttt.com", 80)) {
    // Construct and send the GET request to the webhook
    client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME)); // Specify the host in the request
    client.println("Connection: close"); // Indicate that the connection will be closed after the request
    client.println(); // Send a blank line to indicate the end of the headers
    delay(500); // Wait for a short period to allow the server to respond
    client.stop(); // Close the connection to the server
    Serial.println("Webhook triggered successfully"); // Print success message
  } else {
    Serial.println("Failed to connect to webhook server"); // Print error message if connection fails
  }
}
