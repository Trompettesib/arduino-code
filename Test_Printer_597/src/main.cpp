#include <Arduino.h>
#include <Adafruit_Thermal.h>
#include <SoftwareSerial.h>

#define TX_PIN D4
#define RX_PIN D3
#define DEBUG_MODE false
#define NB_CHAR_MAX_PRINTED 32

SoftwareSerial mySerial(RX_PIN, TX_PIN);
Adafruit_Thermal printer(&mySerial);

String serial_input;

void setup() {
  Serial.begin(9600);
  mySerial.begin(19200);
  Serial.flush();
  delay(5000);
  mySerial.flush();

  if (DEBUG_MODE) {
    Serial.println("printer initialisation");
  }
  delay(1000);
  printer.begin();
  delay(1000);
  printer.flush();
  delay(1000);
  serial_input = "";
}

void loop() {
  serial_input = Serial.readString();
  if (serial_input != "") {
    printer.println(serial_input);
    if (DEBUG_MODE) {
      Serial.println(serial_input);
    }
    serial_input = "";
  }
}