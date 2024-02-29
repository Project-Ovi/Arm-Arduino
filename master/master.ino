#include <Wire.h>
#include <avr/wdt.h>

const int maxSlaves = 10;

void setup() {
  // Initialize I2C communication
  Wire.begin();

  // Initialize Serial communication
  Serial.begin(2000000);

  // Reset all slaves
  for (int i = 1; i <= maxSlaves ; i++) {
    Wire.beginTransmission(i);
    Wire.println("R");
    Wire.endTransmission();
  }
}

void loop() {
  goto start;
start:
  // Cleanup
  emptySerialBuffer();

  // Wait for Serial data
  while(!Serial.available()) {}

  // Check reset
  char c = Serial.read();
  if (c == 'R') reboot();

  // Check begin byte
  if (c != '>') goto start;

  // Read commands
  String buff="";
  String coms[50];
  int comslen = 0; 
  while(c != '<') {
    c = Serial.read();
    if (c == '|' || c == '<') {
      coms[comslen] = '>' + buff + '<';
      buff = "";
      comslen++;
    } else {
      buff += c;
    }
  }

  // Send commands
  for (int i = 1; i <= comslen ; i++) {
    Wire.beginTransmission(i);
    Wire.print(coms[i-1]);
    Serial.println(coms[i-1]);
    Wire.endTransmission();
  }
}

void emptySerialBuffer() {
  while (Serial.available()) {
    char _ = Serial.read();
  }
}

void reboot() {
  wdt_disable();
  wdt_enable(WDTO_15MS);
  while (1) {}
}