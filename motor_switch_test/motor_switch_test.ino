#define MOTOR_CW_PIN 14
#define MOTOR_CCW_PIN 12
#define SWITCH_ONE_PIN 13
#define SWITCH_TWO_PIN 15

void setup() {
  pinMode(SWITCH_ONE_PIN, INPUT);
  pinMode(SWITCH_TWO_PIN, INPUT);
  pinMode(MOTOR_CW_PIN, OUTPUT);
  pinMode(MOTOR_CCW_PIN, OUTPUT);

  digitalWrite(MOTOR_CW_PIN, LOW);
  digitalWrite(MOTOR_CCW_PIN, LOW);
}
     
void loop() {
  Serial.println("Delay...");
  delay(3000);
  if ((digitalRead(SWITCH_ONE_PIN) == HIGH) && (digitalRead(SWITCH_TWO_PIN) == LOW)) {    
    digitalWrite(MOTOR_CW_PIN, HIGH);
    digitalWrite(MOTOR_CCW_PIN, LOW);
    Serial.println("CW");
  } else if ((digitalRead(SWITCH_ONE_PIN) == LOW) && (digitalRead(SWITCH_TWO_PIN) == HIGH)) {    
    digitalWrite(MOTOR_CW_PIN, LOW);    
    digitalWrite(MOTOR_CCW_PIN, HIGH);
    Serial.println("CCW");
  } else if ((digitalRead(SWITCH_ONE_PIN) == LOW) && (digitalRead(SWITCH_TWO_PIN) == LOW)) {
    digitalWrite(MOTOR_CW_PIN, LOW);    
    digitalWrite(MOTOR_CCW_PIN, LOW);
    Serial.println("NEITHER");
  }
}

/*
void setup() {
}
     
void loop() {
}*/
