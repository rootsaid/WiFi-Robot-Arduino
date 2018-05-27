#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>

int ledPin = 6;
int status = WL_IDLE_STATUS;
char ssid[] = "RootSaid"; //WiFi Network Name
char pass[] = "WiFi Password"; //WiFi Network Password
int keyIndex = 0;
unsigned int localPort = 6661;
char packetBuffer[255];
char  ReplyBuffer[] = "acknowledged";

WiFiUDP Udp;

void setup() {

pinMode (10, OUTPUT);
pinMode (11, OUTPUT);
pinMode (12, OUTPUT);
pinMode (13, OUTPUT);
pinMode(ledPin, OUTPUT); 
Serial.begin(9600);

if (WiFi.status() == WL_NO_SHIELD) 
  {
    Serial.println("WiFi shield not present");
    while (true);
  }

  // Connect to WiFi
while ( status != WL_CONNECTED) 
  {
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  status = WiFi.begin(ssid, pass);
  delay(10000);
  }
  Serial.println("WiFi Status  ");
  printWiFiStatus();

  Serial.println("\n Starting Listener");
  Udp.begin(localPort);
}

void loop() {

  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    int len = Udp.read(packetBuffer, 255);
    if (len > 0) packetBuffer[len] = 0;
    Serial.println("Contents:");
    Serial.println(packetBuffer);

    if(strcmp(packetBuffer, "forward") == 0){
      forward();
    }
    if(strcmp(packetBuffer, "backward") == 0){
      backward();
    }
    if(strcmp(packetBuffer, "stop") == 0){
      stop();
    }
      
    if(strcmp(packetBuffer, "left") == 0){
      left();
    }
    if(strcmp(packetBuffer, "right") == 0){
      right();
    }

    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  }

}

void printWiFiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void forward()
{
digitalWrite(10, HIGH);
digitalWrite(11, LOW);
digitalWrite(12, HIGH);
digitalWrite(13, LOW);
digitalWrite(ledPin, HIGH);
Serial.println(" Moving Forward");

}
void backward()
{
digitalWrite(10, LOW);
digitalWrite(11, HIGH);
digitalWrite(12, LOW);
digitalWrite(13, HIGH);
digitalWrite(ledPin, HIGH);
Serial.println("Moving Backward");
}
void stop()
{
digitalWrite(10, LOW);
digitalWrite(11, LOW);
digitalWrite(12, LOW);
digitalWrite(13, LOW);
digitalWrite(ledPin, LOW);
Serial.println("Stop");
}
void left()
{
digitalWrite(10, HIGH);
digitalWrite(11, LOW);
digitalWrite(12, LOW);
digitalWrite(13, HIGH);
digitalWrite(ledPin, HIGH);
Serial.println(" Rotate Left");
}
void right()
{
digitalWrite(10, LOW);
digitalWrite(11, HIGH);
digitalWrite(12, HIGH);
digitalWrite(13, LOW);
digitalWrite(ledPin, HIGH);
Serial.println(" Rotate Right");
}

