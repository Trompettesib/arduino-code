#include <Arduino.h>
#include <Adafruit_Thermal.h>
#include <SoftwareSerial.h>

#define TX_PIN D4
#define RX_PIN D3
#define DEBUG_MODE true
#define NB_CHAR_MAX_PRINTED 32
#define ALLIGN_TO_LEFT true

SoftwareSerial mySerial(RX_PIN, TX_PIN);
Adafruit_Thermal printer(&mySerial);

String serial_input;

void setup() {
  Serial.begin(9600);
  mySerial.begin(19200);
  Serial.flush();
  delay(1000);
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
  Serial.println("Ready !");
}

void print(char c) {
  if (DEBUG_MODE) {
    Serial.print(c);
  } else {
    printer.print(c);
  }
}

bool space_after_space(int i, int j, char *str) {
  i++;
  j++;
  //Serial.println(strlen(str));
  if (j >= NB_CHAR_MAX_PRINTED) {
    //Serial.println("false1");
    return (false);
  }
  //Serial.print("\nj = ");
  //Serial.println(j);
  for (; str[i] != '\0' && str[i] != ' '; i++, j++) {
    //Serial.print("\ni = ");
    //Serial.println(i);
    //Serial.print("\nj = ");
    //Serial.println(j);
    //Serial.print("\nstr[i] = ");
    //Serial.println(str[i]);
    //Serial.print("\nstrlen(str) = ");
    //Serial.println(strlen(str));
    if (j >= NB_CHAR_MAX_PRINTED) {
      //Serial.println("false2");
      return (false);
    }
  }
  //Serial.println("true");
  return (true);
}

void allign_left(String text) {
  //Serial.print("\n text.length = ");
  //Serial.println(text.length());
  char *str = new char [text.length() + 1];
  strcpy(str, text.c_str());
  for (int i = 0, j = 0; str[i] != '\0'; i++, j++) {
    if (str[i] == ' ') {
      if (!space_after_space(i, j, str)) {
        print('\n');
        j = 0;
        i++;
      }
    }
    print(str[i]);
  }
  delete [] str;
}

void loop() {
  serial_input = Serial.readString();
  if (serial_input != "") {
    if (ALLIGN_TO_LEFT) {
      Serial.println(' ');
      Serial.print("Result : ");
      allign_left(serial_input);
    } else {
      if (DEBUG_MODE) {
        Serial.println(serial_input);
      } else {
        printer.println(serial_input);
      }
    }
    serial_input = "";
  }
}