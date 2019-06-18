void setup() {
  Serial.begin(9600);

  Serial.println("Serial monitor messages...");
}

void loop() {
  char input;
  
  Serial.print('A');
  Serial.println(100);
  Serial.print(99.999);

  Serial.print("Debug: ");
  Serial.println("Here");

  while(1) {
    if(Serial.available() > 0) {
      input = Serial.read();
      Serial.print(input);
    }
  }
}

