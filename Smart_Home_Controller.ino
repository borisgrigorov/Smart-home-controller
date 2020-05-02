#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <RCSwitch.h>
int IRledPin =  16;
RCSwitch mySwitch = RCSwitch();
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//const char* ssid = "WiFiG";
//const char* password = "grigorovovi96";
const char* mqttServer = "10.0.0.130";
WiFiClient espClient;
PubSubClient client(espClient);
long buttonTimer = 0;
long longPressTime = 300;
boolean buttonActive = false;
boolean longPressActive = false;
long odezva, vzdalenost;
String a = "";
boolean motion = false;
void callback(char* topic, byte* payload, unsigned int length) {
  a = "";
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    a = a + (char)payload[i];
  }
  if(a == "on1x")
  {
    mySwitch.send("10011000111000110000111111110100");
    client.publish("sockets/1/status", "on");
    Serial.println("Turning 1 on");
    delay(500);
  } 
  if(a == "off1"){
    mySwitch.send("10011000111000110000011111111100");
    client.publish("sockets/1/status", "off");
    Serial.println("Turning 1 off");
    delay(500);
  }
  if(a == "on2x"){
    mySwitch.send("10011000111000110000101111110010");
    client.publish("sockets/2/status", "on");
    delay(500);
  }
  if(a == "off2"){
    mySwitch.send("10011000111000110000001111111010");
    client.publish("sockets/2/status", "off");
    delay(500);
  }
  if(a == "on3x"){
    mySwitch.send("10011000111000110000110111110110");
    client.publish("sockets/3/status", "on");
    delay(1000);
  }
  if(a == "off3"){
    mySwitch.send("10011000111000110000010111111110");
    client.publish("sockets/3/status", "off");
    delay(1000);
  }
  if(a == "on4x"){bool door = false;
    mySwitch.send("10011000111000110000111011110101");
    client.publish("sockets/4/status", "on");
    delay(1000);
  }
  if(a == "off4"){
    mySwitch.send("10011000111000110000011011111101");
    client.publish("sockets/4/status", "off");
    delay(1000);
  }
  if(a == "onll"){
    mySwitch.send("10011000111000110000010011111111");
    client.publish("sockets/all/status", "on");
    delay(1000);
  }
  if(a == "offa"){
    mySwitch.send("10011000111000110000100011110000");
    client.publish("sockets/all", "off");
    delay(1000);
  }
  if(a == "tv"){
    SendChannelUpCode();
    delay(1000);
  }
  if(a == "radio"){
    radio();
    delay(1000);
  }
  Serial.println(a);
  Serial.println("-----------------------");
 
}
void setup() {
  pinMode(IRledPin, OUTPUT);
  Serial.begin(9600);
  //WiFi.begin(ssid, password);
  WiFiManager wifiManager;
  wifiManager.autoConnect("Smart home mini");
  pinMode(12, INPUT);
  pinMode(14, INPUT);
  pinMode(0, OUTPUT);
  pinMode(4, OUTPUT);
  mySwitch.enableTransmit(10);
  mySwitch.setProtocol(1);
  mySwitch.setPulseLength(259);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");  
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
  client.subscribe("sockets/state");
}

void loop() {
    client.loop();
    if(digitalRead(14) == HIGH && motion == false){
      client.publish("node/motion", "motion");
      motion = true;
      digitalWrite(0, HIGH);
      delay(200);
      digitalWrite(0, LOW);
    }
    else if(digitalRead(14) == LOW && motion == true){
      motion = false;
    }
    if (digitalRead(12) == HIGH) {
      if (buttonActive == false) {
        buttonActive = true;
        buttonTimer = millis();
      }
      if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {
        longPressActive = true;
        client.publish("node/button", "long");
        digitalWrite(0, HIGH);
        delay(100);
        digitalWrite(0, LOW);
        delay(100);
        digitalWrite(0, HIGH);
        delay(100);
        digitalWrite(0, LOW);
        delay(100);
      }
    } else {
      if (buttonActive == true) {
        if (longPressActive == true) {
          longPressActive = false;
        } else {
          client.publish("node/button", "short");
          digitalWrite(4, HIGH);
          delay(100);
          digitalWrite(4, LOW);
          delay(100);
          digitalWrite(4, HIGH);
          delay(100);
          digitalWrite(4, LOW);
          delay(100);
        }
        buttonActive = false;
      }
    }
    
}
void pulseIR(long microsecs) {
  // we'll count down from the number of microseconds we are told to wait
 
  cli();  // this turns off any background interrupts
 
  while (microsecs > 0) {
    // 38 kHz is about 13 microseconds high and 13 microseconds low
   digitalWrite(IRledPin, HIGH);  // this takes about 3 microseconds to happen
   delayMicroseconds(10);         // hang out for 10 microseconds
   digitalWrite(IRledPin, LOW);   // this also takes about 3 microseconds
   delayMicroseconds(10);         // hang out for 10 microseconds
 
   // so 26 microseconds altogether
   microsecs -= 26;
  }
 
  sei();  // this turns them back on
}
 
void SendChannelUpCode() {
  delayMicroseconds(40152); //Time off (Left Column on serial monitor)
  pulseIR(3060);           //Time on  (Right Column on serial monitor)
  delayMicroseconds(3120);
  pulseIR(440);
  delayMicroseconds(1600);
  pulseIR(480);
  delayMicroseconds(1560);
  pulseIR(480);
  delayMicroseconds(1580);
  pulseIR(480);
  delayMicroseconds(1580);
  pulseIR(460);
  delayMicroseconds(1620);
  pulseIR(460);
  delayMicroseconds(1580);
  pulseIR(460);
  delayMicroseconds(2580);
  pulseIR(460);
  delayMicroseconds(1620);
  pulseIR(460);
  delayMicroseconds(1580);
  pulseIR(460);
  delayMicroseconds(1600);
  pulseIR(480);
  delayMicroseconds(1580);
  pulseIR(500);
  delayMicroseconds(1560);
  pulseIR(460);
  delayMicroseconds(2620);
  pulseIR(440);
  delayMicroseconds(1600);
  pulseIR(460);
  delayMicroseconds(2580);
  pulseIR(460);
  delayMicroseconds(2580);
  pulseIR(480);
  delayMicroseconds(4100);
  pulseIR(500);
  delayMicroseconds(23480);
  pulseIR(3100);
  delayMicroseconds(3100);
  pulseIR(460);
  delayMicroseconds(1580);
  pulseIR(480);
  delayMicroseconds(1580);
  pulseIR(480);
  delayMicroseconds(1580);
  pulseIR(460);
  delayMicroseconds(1600);
  pulseIR(460);
  delayMicroseconds(1580);
  pulseIR(480);
  delayMicroseconds(1600);
  pulseIR(480);
  delayMicroseconds(2560);
  pulseIR(480);
  delayMicroseconds(1600);
  pulseIR(480);
  delayMicroseconds(1560);
  pulseIR(460);
  delayMicroseconds(1580);
  pulseIR(460);
  delayMicroseconds(1600);
  pulseIR(480);
  delayMicroseconds(1580);
  pulseIR(480);
  delayMicroseconds(2580);
  pulseIR(480);
  delayMicroseconds(1600);
  pulseIR(440);
  delayMicroseconds(2600);
  pulseIR(440);
  delayMicroseconds(2600);
  pulseIR(440);
  delayMicroseconds(4140);
  pulseIR(480);
  
}
void radio(){
delayMicroseconds(160);
pulseIR(3440);
delayMicroseconds(1900);
pulseIR(400);
delayMicroseconds(1240);
pulseIR(400);
delayMicroseconds(1240);
pulseIR(400);
delayMicroseconds(480);
pulseIR(420);
delayMicroseconds(480);
pulseIR(460);
delayMicroseconds(440);
pulseIR(380);
delayMicroseconds(500);
pulseIR(460);
delayMicroseconds(1180);
pulseIR(380);
delayMicroseconds(520);
pulseIR(380);
delayMicroseconds(1240);
pulseIR(400);
delayMicroseconds(1220);
pulseIR(400);
delayMicroseconds(500);
pulseIR(460);
delayMicroseconds(440);
pulseIR(400);
delayMicroseconds(1220);
pulseIR(460);
delayMicroseconds(440);
pulseIR(400);
delayMicroseconds(1220);
pulseIR(460);
delayMicroseconds(440);
pulseIR(400);
delayMicroseconds(1220);
pulseIR(400);
delayMicroseconds(480);
pulseIR(400);
delayMicroseconds(520);
pulseIR(460);
delayMicroseconds(420);
pulseIR(420);
delayMicroseconds(480);
pulseIR(380);
delayMicroseconds(520);
pulseIR(400);
delayMicroseconds(500);
pulseIR(360);
delayMicroseconds(520);
pulseIR(460);
delayMicroseconds(440);
pulseIR(460);
delayMicroseconds(440);
pulseIR(380);
delayMicroseconds(1240);
pulseIR(400);
delayMicroseconds(500);
pulseIR(360);
delayMicroseconds(540);
pulseIR(360);
delayMicroseconds(520);
pulseIR(380);
delayMicroseconds(520);
pulseIR(380);
delayMicroseconds(520);
pulseIR(360);
delayMicroseconds(1260);
pulseIR(380);
delayMicroseconds(520);
pulseIR(380);
delayMicroseconds(500);
pulseIR(380);
delayMicroseconds(520);
pulseIR(380);
delayMicroseconds(520);
pulseIR(380);
delayMicroseconds(520);
pulseIR(400);
delayMicroseconds(480);
pulseIR(460);
delayMicroseconds(440);
pulseIR(460);
delayMicroseconds(1160);
pulseIR(460);
delayMicroseconds(420);
pulseIR(400);
delayMicroseconds(1240);
pulseIR(380);
delayMicroseconds(520);
pulseIR(380);
delayMicroseconds(520);
pulseIR(380);
delayMicroseconds(500);
pulseIR(380);
delayMicroseconds(520);
pulseIR(460);
delayMicroseconds(440);
pulseIR(380);
}
