void setup() {
  Serial.begin(9600);
}

void loop() {
  String message = "hello world"; // message to hash
  
  uint8_t salt[32] = { 
  0x45, 0x21, 0x7B, 0x9F, 0x5A, 0x3C, 0x88, 0x2B, 
  0x0A, 0x4F, 0x6E, 0xD8, 0x1B, 0x9C, 0xE5, 0x73, 
  0x2E, 0x85, 0x47, 0x63, 0xB9, 0x1E, 0x06, 0xAF, 
  0xDC, 0x51, 0xF3, 0x89, 0x74, 0x67, 0xC2, 0xAB 
  }; // 256-bit salt
  
  uint32_t hash[8];
  for (int i = 0; i < 8; i++) {
    hash[i] = salt[i * 4] << 24 | salt[i * 4 + 1] << 16 | salt[i * 4 + 2] << 8 | salt[i * 4 + 3];
  }
  
  // Hash the message
  for (int i = 0; i < message.length(); i++) {
    uint32_t rotate = (i % 32);
    uint32_t temp = ((uint32_t) message[i]) << rotate | ((uint32_t) message[i]) >> (32 - rotate);
    int index = i % 8;
    hash[index] ^= temp;
  }
  
  // Print the hash
  for (int i = 0; i < 8; i++) {
    Serial.print(hash[i], HEX);
  }
  
  delay(1000);
}