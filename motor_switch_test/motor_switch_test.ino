#define MOTOR_CW_PIN 14
#define MOTOR_CCW_PIN 12
#define SWITCH_ONE_PIN 13
#define SWITCH_TWO_PIN 5


void setup() {
  pinMode(SWITCH_ONE_PIN, INPUT);
  pinMode(SWITCH_TWO_PIN, INPUT);
  pinMode(MOTOR_CW_PIN, OUTPUT);
  pinMode(MOTOR_CCW_PIN, OUTPUT);

  digitalWrite(MOTOR_CW_PIN, LOW);
  digitalWrite(MOTOR_CCW_PIN, LOW);
}

void open() {
  while (digitalRead(SWITCH_TWO_PIN) == HIGH) {
    digitalWrite(MOTOR_CW_PIN, HIGH);
    digitalWrite(MOTOR_CCW_PIN, LOW);
    yield();
  }

  stop();
  delay(2000);
}


void close() {
  while (digitalRead(SWITCH_ONE_PIN) == HIGH) {
    digitalWrite(MOTOR_CW_PIN, LOW);
    digitalWrite(MOTOR_CCW_PIN, HIGH);
    yield();
  }

  stop();
  delay(2000);
}


void stop() {
  digitalWrite(MOTOR_CW_PIN, LOW);
  digitalWrite(MOTOR_CCW_PIN, LOW);
}


void loop() {
  delay(3500);
  
  if ((digitalRead(SWITCH_ONE_PIN) == LOW) && (digitalRead(SWITCH_TWO_PIN) == HIGH))
    open();
  else if ((digitalRead(SWITCH_ONE_PIN) == HIGH) && (digitalRead(SWITCH_TWO_PIN) == LOW))
    close();
  else if ((digitalRead(SWITCH_ONE_PIN) == HIGH) && (digitalRead(SWITCH_TWO_PIN) == HIGH) || (digitalRead(SWITCH_ONE_PIN) == LOW) && (digitalRead(SWITCH_TWO_PIN) == LOW))
    stop();
}
