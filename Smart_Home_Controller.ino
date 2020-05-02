//Import used libraries
#include <DNSServer.h> //DNS server for wifi manager
#include <ESP8266WebServer.h> //Webserver for wifi manager
#include <WiFiManager.h> //Wifi manager
#include <RCSwitch.h> //Library for 433MHz module
#include <ESP8266WiFi.h> //Wifi library
#include <PubSubClient.h> //Mqtt library

#include "ir.h"
#include "ir_commands.h"

WiFiClient espClient;
PubSubClient client(espClient);
RCSwitch mySwitch = RCSwitch();
WiFiManager wifiManager;

//Set pins numbers
int buttonPin = 12;
int greenLed = 4;
int redLed = 0;
int motionSensor = 14;
int IRledPin =  16;
int wirelessModule = 10;

//Set mqtt broker (server)
const char* mqttServer = "10.0.0.130"; //Set your mqtt broker ip address (set your own)
int mqttPort = 1883; //Set your mqtt broker port (set your own)

//set button press properties
long buttonTimer = 0;
long longPressTime = 300;
boolean buttonActive = false;
boolean longPressActive = false;

String a = ""; //Mqtt command string
boolean motion = false; //Motion sensor status

//Function for incoming mqtt message
void callback(char* topic, byte* payload, unsigned int length) {
  a = ""; //Clear command string
  Serial.print("Message arrived in topic: "); //Print message topic on serial monitor
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    a = a + (char)payload[i];
  }
  Serial.println(a); //Print message on serial monitor
  if(a == "on1") //If command is "on1"
  {
    mySwitch.send("10011000111000110000111111110100"); //Then send signal by 433MHz module, (turn on socket one)
    client.publish("sockets/1/status", "on"); //Publish sockets status
    Serial.println("Turning 1 on"); //Print status on serial monitor
    delay(500); //Wait 0.5s
  } 
  if(a == "off1"){
    mySwitch.send("10011000111000110000011111111100");
    client.publish("sockets/1/status", "off");
    Serial.println("Turning 1 off");
    delay(500);
  }
  if(a == "on2"){
    mySwitch.send("10011000111000110000101111110010");
    client.publish("sockets/2/status", "on");
    delay(500);
  }
  if(a == "off2"){
    mySwitch.send("10011000111000110000001111111010");
    client.publish("sockets/2/status", "off");
    delay(500);
  }
  if(a == "on3"){
    mySwitch.send("10011000111000110000110111110110");
    client.publish("sockets/3/status", "on");
    delay(1000);
  }
  if(a == "off3"){
    mySwitch.send("10011000111000110000010111111110");
    client.publish("sockets/3/status", "off");
    delay(1000);
  }
  if(a == "on4"){
    mySwitch.send("10011000111000110000111011110101");
    client.publish("sockets/4/status", "on");
    delay(1000);
  }
  if(a == "off4"){
    mySwitch.send("10011000111000110000011011111101");
    client.publish("sockets/4/status", "off");
    delay(1000);
  }
  if(a == "onall"){
    mySwitch.send("10011000111000110000010011111111");
    client.publish("sockets/all/status", "on");
    delay(1000);
  }
  if(a == "offall"){
    mySwitch.send("10011000111000110000100011110000");
    client.publish("sockets/all/status", "off");
    delay(1000);
  }
  if(a == "tv"){
    tv(); //Call tv function that sends IR signal to tv
    delay(1000); //Wait 1 second
  }
  if(a == "radio"){
    radio(); //Call radio function that sends IR signal to radio
    delay(1000); //Wait 1 second
  }
  Serial.println("-----------------------");
 
}
void setup() {
  //Set pins modes
  pinMode(IRledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(motionSensor, INPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(redLEd, OUTPUT);

  //Begin serial communication
  Serial.begin(9600);

  //Use wifi manager for first time use
  wifiManager.autoConnect("Smart home mini"); //Set access point name

  //Set 433Mhz module properties
  mySwitch.enableTransmit(wirelessModule); //Module pin
  mySwitch.setProtocol(1); //Set protocol (protocol 1 should be work for most sockets)
  mySwitch.setPulseLength(259); //Set pulse length
  while (WiFi.status() != WL_CONNECTED) { //If not connected to wifi
    delay(500); //Wait 0.5 second
    Serial.println("Connecting to WiFi.."); //Print "Connecting to wifi..."
  }
  Serial.println("Connected to the WiFi network"); //If connected, print "Connected to the WiFi network"
  client.setServer(mqttServer, mqttPort); //Set mqtt broker (server) and port
  client.setCallback(callback); //set callback function
  while (!client.connected()) { //If not connected to wifi
    Serial.println("Connecting to MQTT...");
    if (client.connect("SmartHomeMini")) { //If is mqtt client connect
      Serial.println("connected");  //Print "connected"
    } else { //If is not connected
      Serial.print("failed with state "); //Print state
      Serial.print(client.state());
      delay(2000); //Wait 2 seconds
 
    }
  }
  //Subcribe to mqqt topic
  client.subscribe("sockets/state");
}

void loop() {
    client.loop();
    if(digitalRead(motionSensor) == HIGH && motion == false){ //If motion is detected
      client.publish("node/motion", "motion"); //Send mqtt message about motion
      motion = true; //Set motion variable to true (motion), to prevent repeated trigger
      digitalWrite(redLed, HIGH); //Blink red LED
      delay(200);
      digitalWrite(redLed, LOW);
    }
    else if(digitalRead(motionSensor) == LOW && motion == true){ //If is no motion
      motion = false; //Set variable to no-motion
    }
    if (digitalRead(buttonPin) == HIGH) { //If buuton is pressed
      if (buttonActive == false) { //And "active" variable is not true
        buttonActive = true; //Set it to true
        buttonTimer = millis(); //Set time of press
      }
      if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) { //If it is pressed for more than set time, and is not active longPress variable
        longPressActive = true; //Set it to true
        client.publish("node/button", "long"); //Send mqtt message about long press
        digitalWrite(redLed, HIGH); //Twice blink red LED
        delay(100);
        digitalWrite(redLed, LOW);
        delay(100);
        digitalWrite(redLed, HIGH);
        delay(100);
        digitalWrite(redLed, LOW);
        delay(100);
      }
    } else { //Else
      if (buttonActive == true) { //Button is pressed
        if (longPressActive == true) { //And long press is active
          longPressActive = false; //Set it to false
        } else { //else
          client.publish("node/button", "short"); //Send mqtt message about short press
          digitalWrite(greenLed, HIGH); //Twice blink green LED
          delay(100);
          digitalWrite(greenLed, LOW);
          delay(100);
          digitalWrite(greenLed, HIGH);
          delay(100);
          digitalWrite(greenLed, LOW);
          delay(100);
        }
        buttonActive = false; //Set it to false
      }
    }   
}

//Function for sending ir codes
void pulseIR(long microsecs) {
  cli();
  while (microsecs > 0) {
   digitalWrite(IRledPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(IRledPin, LOW);
   delayMicroseconds(10);
   microsecs -= 26;
  }
  sei();
}