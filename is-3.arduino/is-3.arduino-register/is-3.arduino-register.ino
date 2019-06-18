void setup() {
  DDRB = 0x80;
}

void loop() {
  PORTB = 0x80;
  delay(1000);
  PORTB = 0x00;
  delay(1000);
}
