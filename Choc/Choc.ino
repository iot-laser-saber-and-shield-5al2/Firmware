//  From the Open Sound Control (OSC) library for the ESP8266/ESP32
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

#include "Wire.h"
#include <MPU6050_tockn.h>
TwoWire i2c;
MPU6050 mpu6050(i2c);


////////////////////////////////////////////////////////////////////////////////
char ssid[] = "Hyde777";                // TODO: your network SSID (name)
char pass[] = "CestYoyo";             // TODO: your network password
const IPAddress outIp(192,168,43,252); // TODO: remote IP of your computer
////////////////////////////////////////////////////////////////////////////////


const unsigned int localPort = 8888;  // local port to listen for OSC packets (not used for sending)
const unsigned int outPort = 4559;    // remote port to receive OSC
WiFiUDP Udp;                          // A UDP instance to let us send and receive packets over UDP


void setup() {
    Serial.begin(115200);
    Serial.println("Hello!");

    i2c.begin(D3, D4);
    mpu6050.begin();

    // Connect to WiFi network
    Serial.print("\nConnecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected - IP address: ");
    Serial.println(WiFi.localIP());

    Serial.println("Starting UDP");
    Udp.begin(localPort);

    Serial.print("Local port: ");
    Serial.println(Udp.localPort());
}

void loop() {
    // handle IMU
    mpu6050.update();
    float x = mpu6050.getAccX();
    float y = mpu6050.getAccY();
    float z = mpu6050.getAccZ();
    Serial.print("-3 3 ");
    Serial.print(x); Serial.print(" ");
    Serial.print(y); Serial.print(" ");
    Serial.println(z); Serial.print(" ");
    if(abs(x) == 2 && abs(y) == 2 && abs(z) == 2)
    {
      Serial.print(wifi_station_get_hostname()); Serial.print(" ");
      Serial.println("choc");
    }
   
    // handle OSC
    OSCMessage msg("/IMU");
    msg.add(x);
    msg.add(y);
    msg.add(z);
    msg.add(wifi_station_get_hostname());
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
    delay(30);
}
