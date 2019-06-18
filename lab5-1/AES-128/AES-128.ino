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

  int key_num = 0x0;

  byte plain_data[] = "Hello IoT Software!";
  int plain_len = strlen(plain_data);
  plain_len = ((plain_len / 16) + 1) * 16;

  byte enc_data[64];
  int enc_len;
  
  byte dec_data[64];
  int dec_len;

  if(!Generate_AES128Key(key_num))
    Serial.println("Set AES128 Key Generation Failure");

  Serial.print("plain data: ");
  dump(plain_data, plain_len);

  if(Encrypt_AES128(key_num, plain_data, plain_len, enc_data, &enc_len)) {
    Serial.print("enc_data: ");
    dump(enc_data, enc_len);
  }
  else
    Serial.println("Encrypt plain_data Failure");

  if(Decrypt_AES128(key_num, enc_data, enc_len, dec_data, &dec_len)) {
    Serial.print("dec_data: ");
    dump(dec_data, dec_len);
  }
  else
    Serial.println("Decrypt enc_data Failure");
}

void loop() {
  
}
