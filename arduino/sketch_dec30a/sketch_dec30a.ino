
#include <ArduinoJson.h>
#include <MFRC522.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

String ip = "http://find-suitcase.herokuapp.com/";

#define RST_PIN         D0
#define SS_PIN          D8
String value;

const char* nam;
const char* surname;
const char* pat;
float weight;
int stat;
const char* flight;

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::StatusCode status;

byte buffer[18];
byte size = sizeof(buffer);

uint8_t pageAddr = 0x06;

ESP8266WebServer server(80);

void handleRoot() {

  server.send(200, "text/html", SendHTML(value, nam, surname, pat, weight, stat, flight)); //Send web page
}

String SendHTML(String id, String nam, String surname, String pat, float weight, int stat, String flight) {

  String ptr = "<!DOCTYPE html> <html>";
  ptr += "<head> <meta charset=\"UTF-8\"> <title>Find-SuitCase</title>";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 10px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr += ".user_data {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr += ".im {transition: 200ms;padding: 10px; width:320px; height:240px;}\n";
  ptr += ".im:hover {padding: 10px; width:350px; height:270px;}\n";
  ptr += "</style>";

  ptr += " </head>";

  ptr += "<body>";

  ptr += "<div class=\"user_data\">\n";

  ptr += "<p>Name:" + nam + "</p>";
  ptr += "<p>Surname: " + surname + "</p>";
  ptr += "<p>Patronymics: " + pat + "</p>";
  ptr += "<p>Weight: " + String(weight) + "</p>";
  ptr += "<p>Status Code: " + String(stat); "+</p>";
  ptr += "<p>Flight: " + flight + "</p>";

  ptr += "</div>";
  for (int i=1; i <=4; i++) {
    String ipimg = ip + "getphoto/";
    Serial.println(i);
    ipimg += i;
    ipimg += "/";
    ipimg += id;
    ptr += "<img src=" + ipimg+ " class=\"im\">";
  }


  ptr += "</body>";
  ptr += "</html>";
  return ptr;
}

void setup() {
  Serial.begin(115200);

  WiFi.begin("Azamat Wi-fi", "azamat07");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("[INFO] IP: ");
  Serial.println(WiFi.localIP());
  SPI.begin();
  mfrc522.PCD_Init();

  server.on("/", handleRoot);      //Which routine to handle at root location

  server.begin();                  //Start server
  Serial.println("[INFP] HTTP server started");
}

void loop() {
  WiFiClient client;
  HTTPClient http;
  server.handleClient();

  byte block;
  byte len;

  if ( ! mfrc522.PICC_IsNewCardPresent())
    return;

  if ( ! mfrc522.PICC_ReadCardSerial())
    return;


  StaticJsonDocument<200> jdata;
  Serial.println("[INFO] Reading data ... ");
  //data in 4 block is readed at once.
  status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(pageAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("[ERROR] MIFARE_Read() failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  } else {
    value = "";

    nam = "";
    surname = "";
    pat = "";
    weight = 0.0;
    stat = 0;
    flight = "";
  }
  //Dump a byte array to Serial
  for (byte i = 0; i < 16; i++) {
    value += (char)buffer[i];
  }
  Serial.print("[INFO] ID: ");
  Serial.println(value);


  //  for (uint8_t i = 0; i < 16; i++)
  //  {
  //    value += (char)buffer[i];
  //  }
  //  value.trim();


  String httpRequestData = ip + "status/" + value + "/2";
  String httpip = ip + "getdata/" + value;

  http.begin(client, httpRequestData);
  http.addHeader("Content-Type", "text/html");
  int httpcode = http.GET();
  Serial.print("[INFO] HttpCode: ");
  Serial.println(httpcode);
  if (httpcode != 200) {
    Serial.println("[INFO] End Reading");
    return;
  }

  http.begin(client, httpip);
  http.addHeader("Content-Type", "application/json");
  httpcode = http.GET();
  String payload = http.getString();
  deserializeJson(jdata, payload);

  nam = jdata["name"];
  surname = jdata["surname"];
  pat = jdata["patronymics"];
  weight = jdata["weight"];
  flight = jdata["flight"];
  stat = jdata["status"];

  Serial.print("[INFO] Name: ");
  Serial.println(nam);
  Serial.print("[INFO] Surname: ");
  Serial.println(surname);
  Serial.print("[INFO] Patronymics: ");
  Serial.println(pat);
  Serial.print("[INFO] Weight: ");
  Serial.println(weight);
  Serial.print("[INFO] Flight: ");
  Serial.println(flight);
  Serial.print("[INFO] Status Code: ");
  Serial.println(stat);

  Serial.print("[INFO] HttpCode: ");
  Serial.println(httpcode);
  Serial.println("[INFO] End Reading");

  delay(100);

  mfrc522.PICC_HaltA();
}
