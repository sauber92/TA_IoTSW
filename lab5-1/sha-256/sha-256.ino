#include <SArduino.h>

void dump(byte *buf, int len) {
  int i;

  for(i = 0; i < len; i++) {
    Serial.print((char)buf[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600, SERIAL_8E2);

  if(!Init_SE()) {
    Serial.println("SE Connection Failure");
  }

  byte plain_data[] = "Hello IoT Software!";
  int plain_len = strlen(plain_data);

  byte digest[32];
  int digest_len = 32;

  Serial.print("plain data: ");
  dump(plain_data, plain_len);

  if(SHA_256(plain_data, plain_len, digest, &digest_len)) {
    Serial.print("digest: ");
    dump(digest, digest_len);
  }
  else {
    Serial.println("SHA_256 Failure");
  }
}

void loop() {
  
}

