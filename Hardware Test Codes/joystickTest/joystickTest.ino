#define jsupPin 20
#define jsdownPin 2
#define jsrightPin 10
#define jsleftPin 4
#define jsswitchPin 3

bool jsupVal = 0;
bool jsdownVal = 0;
bool jsrightVal = 0;
bool jsleftVal = 0;
bool jsswitchVal = 0;

void setup() {
  Serial.begin(115200);
  pinMode(jsupPin, INPUT_PULLUP);
  pinMode(jsdownPin, INPUT_PULLUP);
  pinMode(jsrightPin, INPUT_PULLUP);
  pinMode(jsleftPin, INPUT_PULLUP);
  pinMode(jsswitchPin, INPUT_PULLUP);
}

void loop() {
  jsupVal = digitalRead(jsupPin);
  jsdownVal = digitalRead(jsdownPin);
  jsrightVal = digitalRead(jsrightPin);
  jsleftVal = digitalRead(jsleftPin);
  jsswitchVal = digitalRead(jsswitchPin);
  if (jsupVal == LOW) {
    Serial.println("UP");
  }
  if (jsdownVal == LOW) {
    Serial.println("DOWN");
  }
  if (jsrightVal == LOW) {
    Serial.println("RIGHT");
  }
  if (jsleftVal == LOW) {
    Serial.println("LEFT");
  }
  if (jsswitchVal == LOW) {
    Serial.println("SWITCH");
  }
}
