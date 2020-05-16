# Smart home controller (AKA Smart home mini)
Make your own - Harmony hub like - hub with esp8266.
Smart home controller firmware for ESP8266 for controlling smart sockets, switch, tvs, radios, projectors, lights, and others. It works with Node-RED, Openhab, or anything you configure it for.

## Features

 - [x] Control devices via 433Mhz module
 - [x] Control devices via IR
 - [x] Send motion info
 - [x] Send button press
 - [x] Auto switch to access point mode, when not connected to wifi
 - [ ] (OTA update)* 
 - [ ] (Measure temperature)*
 - [ ] (Receive 433Mhz data)*
 - [ ] (Generate own firmware from GUI)*

 
*In progress

## Requirements

 - Installed support for boards with esp - [Tutorial](https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/)
 - PubSubClient library - [Link](https://github.com/knolleary/pubsubclient)
 - RCSwitch library- In source
 - Wifi Manager library - [Link](https://github.com/tzapu/WiFiManager)

## How to use
0. Install libraries (Put them in libraries folder in Arduino sketches)
1. Clone repo or download .zip
2. (Optional) [Get the IR and 433Mhz codes.](#Get%20codes)
3. Connect all things you want to use to the board.
4. Change code for what you want to use - change pins for your connection, and change parameters like mqtt brokers address and port, or mqtt messages. You can change anything you want.
5. Upload code from Arduino IDE.
6. Enjoy :-)

## Get codes

## 433MHz codes

 1. Connect 433Mhz receive module to Arduino, NodeMCU, or something like that to GPIO 2. (Or change the pin in sketch)
 2. Upload the code for catching codes. (*receivers/RF_receive.ino*)
 3. Open serial monitor.
 4. Take the original remote of the sockets, or what you want to control
 5. Use the function you want to record.
 6. In the serial monitor will show the codes.
 7. Write somewhere the **name of function, binary code, pulse length and protocol**.
 8. In *Smart_Home_Controller.ino* file in callback function set this `mySwitch.send("10011000111000110000111111110100");` (**change the binaries to yours**), on the message you want to.
 9. Repeat this for every function you want to record.

## IR codes

 1. Connect IR receiver to Arduino, NodeMCU, or something like that to GPIO 2. Or change the pin in sketch
 2. Upload the code. Upload the code for catching codes. (*receivers/IR_receive.ino*)
 3. Open serial monitor.
 4. Take the remote of device you want to control.
 5. Use the function you want to record.
 6. In the serial monitor will show the codes.
 7. There are two methods:
----------------
 **1. Easy way using Python**
 
 1. Copy all of these codes.
 2. Save it to file named  *ir.txt* in the folder where is *Ir_codes_writer.py*.
 3. Run the Python script. (You must have installed python on your PC)
 4. Open generated *done.txt* file
 5. Copy all content
 6. In the file *ir_commands.h* create function called whatever you want.
 7. In the function body paste the generated content.
 8. In *Smart_Home_Controller.ino* file in callback function set this function on the message you want to.
---------------------
**2. Hard way using manual copy&paste**

 1. In the file *ir_commands.h* create function called whatever you want.
 2. In the function body write this function `delayMicroseconds(40152);`  **!Change the number to your number from left column!**
 3. In the function body write this function `pulseIR(3060);` **!Change the number to your number from right column!**
 4. Repeat this for every row
 5. In *Smart_Home_Controller.ino* file in callback function set this function on the message you want to.
----------------------
