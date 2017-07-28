//This example needs https://github.com/esp8266/arduino-esp8266fs-plugin

#include "FS.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "Wifi_ssid";
const char* password = "Wifi_password";

const char* mqtt_server = "aaaaaaaaaaaaaa.iot.us-west-2.amazonaws.com"; //MQTT broker ip 


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

}
WiFiClientSecure espClient;
PubSubClient client(mqtt_server,8883,callback,espClient); //set  MQTT port number to 8883 as per //standard
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("coisa")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {

  Serial.begin(115200);
  Serial.setDebugOutput(true);
  setup_wifi();
  delay(1000);
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

Serial.print("Heap: "); Serial.println(ESP.getFreeHeap());

// Load certificate file
File cert = SPIFFS.open("/cert.der", "r"); //replace cert.crt eith your uploaded file name
  if (!cert) {
    Serial.println("Failed to open cert file");
  }
  else
  Serial.println("Success to open cert file");
  
  delay(1000);
  
if(espClient.loadCertificate(cert))
  Serial.println("cert loaded");
else
  Serial.println("cert not loaded");

// Load private key file
File private_key = SPIFFS.open("/private.der", "r"); //replace private eith your uploaded file name
  if (!private_key) {
    Serial.println("Failed to open private cert file");
  }
  else
  Serial.println("Success to open private cert file");
  
  delay(1000);

if(espClient.loadPrivateKey(private_key))
  Serial.println("private key loaded");
else
  Serial.println("private key not loaded");

  
/*  
// Load CA file
File ca = SPIFFS.open("/ca.der", "r"); //replace ca eith your uploaded file name
  if (!ca) {
    Serial.println("Failed to open ca ");
  }
  else
  Serial.println("Success to open ca");
  
  delay(1000);

if(espClient.loadCACert(ca))
  Serial.println("ca loaded");
else
  Serial.println("ca failed");
*/
Serial.print("Heap: "); Serial.println(ESP.getFreeHeap());

 // client.setServer(mqtt_server, 8883);
 // client.setCallback(callback);
}



void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
  }
}
