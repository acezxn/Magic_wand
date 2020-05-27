#include "arduino_secrets.h"
#include <Arduino_APDS9960.h>
#include <Arduino_LSM6DS3.h>
#include <SPI.h>
#include <WiFiNINA.h>

int std_force = 600;
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID; // your network SSID (name)
char pass[] =
    SECRET_PASS;  // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0; // your network key Index number (needed only for WEP)

int led = LED_BUILTIN;
int rpin = 5;
int gpin = 6;
int bpin = 9;

int status = WL_IDLE_STATUS;
WiFiServer server(5555);

void black() {
  digitalWrite(rpin, LOW);
  digitalWrite(gpin, LOW);
  digitalWrite(bpin, LOW);
}

void AP_init() {
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
      ;
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // by default the local IP address of will be 192.168.4.1
  // you can override it with the following:
  // WiFi.config(IPAddress(10, 0, 0, 1));

  // print the network name (SSID);
  // Serial.print("Creating access point named: ");
  // Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true)
      ;
  }

  // wait 10 seconds for connection:
  delay(10000);

  // start the web server on port 80
  server.begin();

  // you're connected now, so print out the status
  // printWiFiStatus();
}

void IMU_init() {
  if (!IMU.begin()) {

    while (1)
      ;
  }
}
void setup() {
  Serial.begin(9600);

  pinMode(rpin, OUTPUT);
  pinMode(gpin, OUTPUT);
  pinMode(bpin, OUTPUT);

  // while (!Serial)
  //   ;

  // Serial.println("Access Point Web Server");

  pinMode(led, OUTPUT); // set the LED pin mode

  // check for the WiFi module:
  AP_init();
  IMU_init();
  black();
}

void loop() {
  float x, y, z;
  float oldx = 0, oldy = 0, oldz = 0;
  double oldtime = 0;
  float old = millis();
  // compare the previous status to the current status
  if (status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      // a device has connected to the AP
      // Serial.println("Device connected to AP");
    } else {
      // a device has disconnected from the AP, and we are back in listening
      // mode
      // Serial.println("Device disconnected from AP");
    }
  }

  WiFiClient client = server.available(); // listen for incoming clients

  if (client) { // if you get a client,

    float m = 0;
    // Serial.println("new client"); // print a message out the serial port

    char msg = client.read();
    while (client.connected()) { // loop while the client's connected
      client.read();
      digitalWrite(gpin, HIGH);
      digitalWrite(rpin, LOW);
      digitalWrite(bpin, LOW);

      m = millis() - oldtime;
      while (m < 100) {
        m = millis() - oldtime;
      }

      if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(x, y, z);
      }

      if (z > std_force) {
        client.write("left");
        // Serial.println("yah");
        black();
        digitalWrite(bpin, HIGH);
        while (z > 10) {
          if (IMU.gyroscopeAvailable()) {
            IMU.readGyroscope(x, y, z);
          }
        }
        oldtime = millis();

      } else {
        if (z < -std_force) {
          client.write("right");
          // Serial.println("nope");
          black();
          digitalWrite(bpin, HIGH);
          while (z < -10) {
            if (IMU.gyroscopeAvailable()) {
              IMU.readGyroscope(x, y, z);
            }
          }
          oldtime = millis();
        } else {
          if (y > std_force) {
            client.write("up");
            // Serial.println("space");
            black();
            digitalWrite(bpin, HIGH);
            while (y > 10) {
              if (IMU.gyroscopeAvailable()) {
                IMU.readGyroscope(x, y, z);
              }
            }
            oldtime = millis();
          } else {
            if (y < -std_force) { // first down
              while (y < -10) {
                if (IMU.gyroscopeAvailable()) {
                  IMU.readGyroscope(x, y, z);
                }
              }
              oldtime = millis();
              m = millis() - oldtime;
              bool custom_gesture_token = false;
              bool custom_gesture_token2 = false;
              black();
              digitalWrite(rpin, HIGH);
              while (m < 300) {
                m = millis() - oldtime;
                if (IMU.gyroscopeAvailable()) {
                  IMU.readGyroscope(x, y, z);
                }
                if (y < -std_force) { // second down
                  custom_gesture_token = true;
                  while (y < -10) {
                    if (IMU.gyroscopeAvailable()) {
                      IMU.readGyroscope(x, y, z);
                    }
                  }
                  oldtime = millis();
                  m = millis() - oldtime;
                  while (m < 300) {
                    m = millis() - oldtime;
                    if (IMU.gyroscopeAvailable()) {
                      IMU.readGyroscope(x, y, z);
                    }
                    if (y < -std_force) { // third down
                      client.write("custom2");
                      // Serial.println("back");
                      custom_gesture_token2 = true;
                      break;
                    }
                  }
                }
              }
              if (custom_gesture_token) {
                if (custom_gesture_token2) {
                  oldtime = millis();
                } else {
                  client.write("custom");
                }
              } else {
                client.write("down");
                // Serial.println("back");
              }
            }
          }
        }
      }
    }
    // close the connection:
    client.stop();
    // Serial.println("client disconnected");
    black();
  }
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
