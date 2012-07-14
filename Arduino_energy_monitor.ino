/*
  unplugg energy monitor
 
 This sketch connects to the unplugg server,
 sending real time data of your power consumption using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Power sensor linked to Analog 0
 
 created Dec 2011
 by Rafael Jegundo

 Thanks to:
 David A. Mellis
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <avr/wdt.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  0x90, 0xA2, 0xDA, 0x00, 0x4A, 0xA0 };
//IPAddress server(173,194,65,121); // Postbin
//IPAddress server(50,16,215,41); // 
char serverName[] = "unplugg.herokuapp.com";

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

//unplugg configuration
String token = "\"GHWcksG8oGgBRMtHpuu8\""; // Replace by your API KEY
int meterId = 5;
int homeId = 1;

long previousWdtMillis = 0;
long wdtInterval = 0;

void setup() {
  // start the serial library:
  Serial.begin(9600);
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");
}

void loop(){
 
  if ((millis() - previousWdtMillis) > wdtInterval) {
    previousWdtMillis = millis();
    wdtInterval = 60000;
    setup();
    Serial.println("wdt reset");
  } 
  // if there are incoming bytes available 
  // from the server, read them and print them:
//  if (client.available()) {
//    char c = client.read();
//    Serial.print(c);
//  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
  postData();
}

void postData(){
  // Get reading value
  String reading = String(analogRead(A0),DEC);

  String body = "{\"auth_token\":" + token + ", \"reading\":" + reading + ", \"home_id\":" + homeId + ", \"meter_id\":" + meterId + "}";
  
  // if you get a connection, report back via serial:
  if (client.connect(serverName, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    //client.print("POST /1l7mdr0 HTTP/1.0\n");
    //client.print("Host: www.postbin.org\n");
    client.print("POST /consumptions.json HTTP/1.0\n");
    client.print("Host: deep-night-85.heroku.com\n");
    client.print("Content-length: ");
    client.println(body.length());
    client.print("Content-Type: application/json\n");
    client.println("Connection: close\n");
    client.println(body);
    Serial.println("Post made");
    delay(1000);
  }
  else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
    Ethernet.begin(mac);
  }
  delay(1000);
}

