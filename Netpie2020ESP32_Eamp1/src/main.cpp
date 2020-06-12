#include <Arduino.h>
#include <WiFi.h>
#include<PubSubClient.h>
#include<esp32iotall_v2.h>
const char* ssid = "Smartfarm-IoT";
const char* password = "62505062";
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "f403325c-dcff-4927-ba20-5854520e73b4";
const char* mqtt_username = "Hyx8fH6F1r9fGCp6S5T68L2gJ8GKZFuW";
const char* mqtt_password = "Cs9nM(dt_NqRUvJy(e3fW7omg(PF052c";
WiFiClient espClient;
PubSubClient client(espClient);
char msg[100];

void reconnect() {
  while (!client.connected()) {
  Serial.print("Sensor MQTT connection…");
  if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
  Serial.println("connected");
  client.subscribe("@msg/LEDBLUEstatus");
  }
  else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println("try again in 5 seconds");
    delay(5000);
  }
 }
}

void onoff(int lighting){
  if(lighting == 1){
    Serial.println("Turn on LED");
    digitalWrite(LED_BLUE,1);
    client.publish("@shadow/data/update", "{\"data\" : {\"LEDBLUEstatus\" : \"on\"}}");
  }
  else if(lighting == 0){
    Serial.println("Turn off LED");
    digitalWrite(LED_BLUE,0);
    client.publish("@shadow/data/update", "{\"data\" : {\"LEDBLUEstatus\" : \"off\"}}");

  }

}

void callback(char* topic,byte* payload, unsigned int length) {
  int lighting;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  String msg;
  for (int i = 0; i < length; i++) {
    msg = msg + (char)payload[i];
  }
   Serial.println(msg);
  if (String(topic) == "@msg/LEDBLUEstatus") {
    if (msg == "on"){
      lighting = 1;
      onoff(lighting); 
    } 
    else if (msg == "off") {
      lighting = 0;
      onoff(lighting);
    } 
  }
}


void setup() {
  Serial.begin(115200);
  pinMode(LED_BLUE,OUTPUT);
  digitalWrite(LED_BLUE,0);
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
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
    static int Moisture = 10;
    if (!client.connected()) {
      reconnect();
    }
    client.loop();
    vTaskDelay(100);
    String data = "{\"data\": {\"Moisture\":" + String(Moisture) + "}}";
    Serial.println(data);
    data.toCharArray(msg, (data.length() + 1));
    client.publish("@shadow/data/update", msg);
    delay(2000);
    if(Moisture++ > 100)
      Moisture = 10;
}