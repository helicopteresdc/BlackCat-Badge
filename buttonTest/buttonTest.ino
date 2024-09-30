#define buttonAPin 5
#define buttonBPin 6

bool buttonAVal = 0;
bool buttonBVal = 0;

void setup() {
  Serial.begin(115200);
  pinMode(buttonAPin, INPUT_PULLUP);
  pinMode(buttonBPin, INPUT_PULLUP);
}

void loop() {
  buttonAVal = digitalRead(buttonAPin);
  buttonBVal = digitalRead(buttonBPin);
  if (buttonAVal == LOW) {
    Serial.println("BUTTON A");
  }
  if (buttonBVal == LOW) {
    Serial.println("BUTTON B");
  }
}
