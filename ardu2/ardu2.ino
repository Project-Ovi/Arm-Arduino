#define UPTIME 50
#define DOWNTIME 50

const String DEVICENAME = "DEFAULT";
const String DEVICETYPE = "ARM";

const int outputs[] = {2, 3, 4, 5};
const size_t numOfOutputs = sizeof(outputs) / sizeof(outputs[0]);

const int inputs[] = {13};
const size_t numOfInputs = sizeof(inputs) / sizeof(inputs[0]);

void setup() {
  // Start serial coms
  Serial.begin(2000000);
  Serial.setTimeout(1);

  // Set output mode for outputs
  for (int i=0; i < numOfOutputs; i++) {
    pinMode(outputs[i], OUTPUT);
  }

  // Set input mode for inputs
  for (int i=0; i < numOfOutputs; i++) {
    pinMode(inputs[i], INPUT);
  }
}

void loop() {
  // Read data
  int b[50];
  int n = readSerial(b);

  if (n > 0)
    blink(b, n);
}


int readSerial(int blinks[]) {
  String buffer;
  char currentChar;
  int blinkI=0;

  while(true) {
    if (!Serial.available()) continue; // Skip if there is nothing to be read
    currentChar = Serial.read();

    // Special cases
    switch(currentChar) {
    case 'w':
      Serial.println(DEVICENAME + " " + DEVICETYPE);
      emptySerialBuffer();
      return blinkI;
    
    case 'z':
      zeroMotors();
      emptySerialBuffer();
      return blinkI;

    case '\n':
      if (buffer.length() > 0)
        blinks[blinkI] = buffer.toInt();

      emptySerialBuffer();
      if (blinks[blinkI] != 0)
        blinkI++;
      return blinkI;

    case ' ':
      blinks[blinkI] = buffer.toInt();
      blinkI++;
      buffer = "";

    default:
      buffer += currentChar;
    }
  }
}

void emptySerialBuffer() {
  while(Serial.available()) Serial.read();
}

void zeroMotors() {
  while(true) {
    // Turn everything ON
    for (int i=0; i < numOfInputs ; i++) {
      if (digitalRead(inputs[i]) != HIGH) continue;

      digitalWrite(outputs[i], HIGH);
    }
    delay(UPTIME);

    // Turn everything OFF
    for (int i=0; i < numOfInputs ; i++) {
      digitalWrite(outputs[i], LOW);
    }
    delay(DOWNTIME);


    // Check if everything is zeroed
    for (int i=0; i < numOfInputs ; i++) {
      if (digitalRead(inputs[i]) == HIGH) break;

      Serial.println("DONE");
      return;
    }
  }
}

void blink(int blinks[], int len) {
  while(true) {
    // Turn ON
    for (int i=0; i < len ; i++) {
      if (blinks[i] <= 0) continue;

      digitalWrite(outputs[i], HIGH);
      blinks[i]--;
    }
    delay(UPTIME);

    // Turn OFF
    for (int i=0; i < len ; i++) {
      digitalWrite(outputs[i], LOW);
    }
    delay(DOWNTIME);

    // Check if there is anything left to blink
    bool allZero = true;
    for (int i=0; i < len ; i++) {
      if (blinks[i] != 0) {
        allZero = false;
        break;
      }
    }

    if (allZero) {
    Serial.println("DONE");
    return;
    }
  }
}










