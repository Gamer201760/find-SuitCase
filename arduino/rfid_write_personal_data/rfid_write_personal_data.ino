
#include <SPI.h>
#include <MFRC522.h>


#define SS_PIN          10
#define RST_PIN         9

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::StatusCode status;

byte buffer[18];
byte size = sizeof(buffer);

uint8_t pageAddr = 0x06;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {

  if ( ! mfrc522.PICC_IsNewCardPresent())
    return;

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
    return;

  while (!Serial.available());

  Serial.readBytes(buffer, 16);
//  Serial.println("Done");



  for (int i = 0; i < 4; i++) {
    //data is writen in blocks of 4 bytes (4 bytes per page)
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Ultralight_Write(pageAddr + i, &buffer[i * 4], 4);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("MIFARE_Read() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }
  }
  Serial.println(F("200"));
  mfrc522.PICC_HaltA();

}
