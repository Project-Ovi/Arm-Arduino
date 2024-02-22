const int windings[] = {2, 3, 4, 5};
const int noWindings = sizeof(windings)/sizeof(windings[0]);

#define UID "AA" // Unique identification code
#define debug_pin 13

#define DEBUG if (debugstate)

int steps;
int speed;
int currWind = 0;
bool debugstate;

void setup() {
  // Init serial
  Serial.begin(2000000);

  // pinMode
  for (int i=0; i < noWindings ; i++)
    pinMode(windings[i], OUTPUT);
  pinMode(debug_pin, INPUT);
}

void loop() {
start:
  // Clean up any mess
  emptySerialBuffer();

  // Await data
  while (!Serial.available()) {}
  delay(1);

  // Set debugstate
  if (digitalRead(debug_pin) == HIGH)
    debugstate = true;
  else
    debugstate = false;
  
  // Check ID integrity
  char ID[2];
  ID[0] = Serial.read();
  if (!Serial.available()) goto start;
  ID[1] = Serial.read();
  if (ID[1] == '\n') goto start;
  if (Serial.read() != ':') goto start;

  // Check ID
  if (ID[0] != UID[0] || ID[1] != UID[1]) goto start;

  // Read steps
  if (!Serial.available()) goto start;
  char cur = Serial.read();
  String _;
  while (cur != ':' && cur != '\n' && cur != ' ') {
    _+= cur;
    cur = Serial.read();
  }
  steps = _.toInt();

  // Read speed
  if (!Serial.available()) goto start;
  cur = Serial.read();
  _="";
  while (cur != ':' && cur != '\n' && cur != ' ') {
    _+= cur;
    cur = Serial.read();
  }
  speed = _.toInt();

  // Move
  if (steps > 0) {
    for (; steps > 0 ; currWind++) {
      digitalWrite(windings[currWind%noWindings], HIGH);
      delay(speed/2);
      digitalWrite(windings[currWind%noWindings], LOW);
      delay(speed/2);
      steps--;
      DEBUG {
        Serial.print("Current winding: ");
        Serial.print(currWind%noWindings, DEC);
        Serial.print("; Steps left: ");
        Serial.println(steps, DEC);
      }
    }
  } else if (steps < 0) {
    currWind -= 2;
    for (; steps < 0 ; currWind--) {
      if (currWind < 0) currWind += noWindings;
      digitalWrite(windings[currWind%noWindings], HIGH);
      delay(speed/2);
      digitalWrite(windings[currWind%noWindings], LOW);
      delay(speed/2);
      steps++;
      DEBUG {
        Serial.print("Current winding: ");
        Serial.print(currWind%noWindings, DEC);
        Serial.print("; Steps left: ");
        Serial.println(steps, DEC);
      }
    }
  } else {
    while(true) {
      digitalWrite(windings[currWind%noWindings], HIGH);
      delay(speed/2);
      digitalWrite(windings[currWind%noWindings], LOW);
      delay(speed/2);
      currWind++;
      DEBUG {
        Serial.print("Current winding: ");
        Serial.println(currWind%noWindings, DEC);
      }
    }
  }

  // Report back
  Serial.print(UID);
  Serial.println(":DONE");

}

void emptySerialBuffer() {
  while(Serial.available()) Serial.read();
}
