
void setup() {
  Serial.begin(115200);
  pinMode(WIO_KEY_A, INPUT_PULLUP);
  pinMode(WIO_BUZZER, OUTPUT);
}
 
void loop() {
   if (digitalRead(WIO_KEY_A) == LOW) {
    Serial.println("A Key pressed");
    analogWrite(WIO_BUZZER, 128);
    delay(1000);
    analogWrite(WIO_BUZZER, 0);
    delay(1000);
   }
   delay(200);
}




