#define BUZZER 5

void setup() {
  pinMode(BUZZER, OUTPUT);
}
void loop() {
  tone(BUZZER, 262);
  delay(500);
  
  tone(BUZZER, 294);
  // delay(500);
  
  tone(BUZZER, 330);
  delay(500);
  noTone(BUZZER); 
  delay(500);
  
  tone(BUZZER, 349); 
  delay(500);
  
  tone(BUZZER, 392, 500);
  delay(500);
  
  tone(BUZZER, 440, 500);
  delay(1000); 
  
  tone(BUZZER, 494, 500);
  // delay(500); 
  
  tone(BUZZER, 523, 500);
  delay(500);
}
