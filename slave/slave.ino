

//###############################

#define SLAVE_ADDRESS 1

//###############################


#include <Wire.h>
#include <avr/wdt.h>

const int windings[] = {2, 3, 4, 5};
const int nowindings = sizeof(windings) / sizeof(windings[0]);

int current_winding = 0;

#define DEBUG_PIN 13
#define DEBUG if (digitalRead(DEBUG_PIN) == HIGH)

void setup() {
  // Initialize I2C communication
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receive);
}

void loop() {}

void receive(int bytes) {
  // Check starting byte
  char c = Wire.read();
  if (c == 'R') reboot();
  if (c != '>') {
    emptyWireBuffer();
    return;
  }

  // Read
  String buff="";
  int steps=0, uptime=0, downtime=0;
  bool gotSteps=false, gotUp=false, gotDown=false;
  while(c != '<') {
    c = Wire.read();
    if (c == ':' || c == '<') {
      if (!gotSteps) {steps = buff.toInt(); gotSteps = true;}
      else if (!gotUp) {uptime = buff.toInt(); gotUp = true;}
      else if (!gotDown) {downtime = buff.toInt(); gotDown = true;}
      else {return;}
      buff = "";
    } else {
      buff += c;
    }
  }

  // Execute movements
  for (int i = 0 ; i < abs(steps) ; i++) {
    current_winding += steps/abs(steps); // Math hack used to either add 1 or substract 1 depending if steps is positive or not
    current_winding = (current_winding + nowindings) % nowindings; // Make sure current_winding remains in the range [0, nowindings]
    digitalWrite(windings[current_winding], HIGH); // Set HIGH
    delay(uptime); // Wait for the requested amount
    digitalWrite(windings[current_winding], LOW); // Set LOW
    delay(downtime); // Wait for the requested amount
  }
}

void emptyWireBuffer() {
  while(Wire.available()) Wire.read();
}

void reboot() {
  wdt_disable();
  wdt_enable(WDTO_15MS);
  while (1) {}
}