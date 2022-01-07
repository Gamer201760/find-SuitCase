#include <MFRC522.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

String ip = "http://192.168.0.101/";

#define RST_PIN         D0
#define SS_PIN          D8

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;

void setup() {
  Serial.begin(115200);

  WiFi.begin("Azamat Wi-fi", "azamat07");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi Connected");
  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {
  WiFiClient client;
  HTTPClient http;

  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  byte block;
  byte len;

  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.println(F("**Card Detected:**"));

  byte buffer1[18];

  block = 4;
  len = 18;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer1, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  String value = "";
  for (uint8_t i = 0; i < 16; i++)
  {
    value += (char)buffer1[i];
  }
  //  value.trim();
  Serial.println(value);
  String httpRequestData = ip + "status/" + value + "/done";
  http.begin(client, httpRequestData);
  http.addHeader("Content-Type", "text/html");
  int httpcode = http.GET();
  Serial.println(httpcode);
  Serial.println(F("\n**End Reading**\n"));

  delay(1000);

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
