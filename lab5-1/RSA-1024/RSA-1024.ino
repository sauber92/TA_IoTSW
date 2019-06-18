#include <SArduino.h>

#define PRIVATE   0
#define PUBLIC    1

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

  int key_num = 0x0;

  byte plain_data[] = "Hello IoT Software!";
  int plain_len = strlen(plain_data);
  
  byte enc_data[128];
  int enc_len;
  
  byte dec_data[64];
  int dec_len;

  if(!Generate_RSA1024Key(key_num))
    Serial.println("Set RSA1024 Key Pair Failure");

  Serial.print("plain data: ");
  dump(plain_data, plain_len);

  if(Encrypt_RSA1024(key_num, PUBLIC, plain_data, plain_len, enc_data, &enc_len)) {
    Serial.print("enc_data: ");
    dump(enc_data, enc_len);
  }
  else
    Serial.println("Encrypt plain_data Failure");

  if(Decrypt_RSA1024(key_num, PRIVATE, enc_data, enc_len, dec_data, &dec_len)) {
    Serial.print("dec_data: ");
    dump(dec_data, dec_len);
  }
  else
    Serial.println("Decrypt enc_data Failure");
}

void loop() {
  
}
