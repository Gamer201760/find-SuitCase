
#include <ArduinoJson.h>
#include <MFRC522.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

String ip = "http://192.168.0.101/";

#define RST_PIN         D0
#define SS_PIN          D8
String value;

const char* nam;
const char* f_name;
int stat;
const char* flight;

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::StatusCode status;

byte buffer[18];
byte size = sizeof(buffer);

uint8_t pageAddr = 0x06;

ESP8266WebServer server(80);

void handleRoot() {

  server.send(200, "text/html", SendHTML(value, nam, f_name, stat, flight)); //Send web page
}
String SendHTML(String id, String nam, String f_name, int stat, String flight) {

  String ptr = "<!DOCTYPE html> <html>";
  ptr += "<head> <meta charset=\"UTF-8\"> <title>Find-SuitCase</title>";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 10px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr += ".user_data {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr += ".im {transition: 200ms;padding: 10px; width:300px; height:300px;}\n";
  ptr += ".im:hover {padding: 10px; width:330px; height:330px;}\n";
  ptr += "</style>";

  ptr += " </head>";

  ptr += "<body>";

  ptr += "<div class=\"user_data\">\n";

  ptr += "<p>Name:" + nam + "</p>";
  ptr += "<p>First Name: " + f_name + "</p>";
  ptr += "<p>Status Code: " + String(stat); "+</p>";
  ptr += "<p>Flight: " + flight + "</p>";

  ptr += "</div>";
  for (int i; i <= 4; i++) {
    String ip = "http://192.168.0.101/getphoto/";
    ip += i;
    ip += "/";
    ip += id;
    ptr += "<img src=" + ip + " class=\"im\">";
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

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  SPI.begin();
  mfrc522.PCD_Init();

  server.on("/", handleRoot);      //Which routine to handle at root location

  server.begin();                  //Start server
  Serial.println("HTTP server started");
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
  Serial.println(F("Reading data ... "));
  //data in 4 block is readed at once.
  status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(pageAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  } else {
    value = "";
    nam = "";
    f_name = "";
    stat = 0;
    flight = "";
  }
  Serial.print(F("Readed data: "));
  //Dump a byte array to Serial
  for (byte i = 0; i < 16; i++) {
   Serial.write(buffer[i]);
   value += (char)buffer[i];
  }
  Serial.print("Id: ");
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
  Serial.println(httpcode);

  http.begin(client, httpip);
  http.addHeader("Content-Type", "application/json");
  httpcode = http.GET();
  String payload = http.getString();
  deserializeJson(jdata, payload);

  nam = jdata["name"];
  f_name = jdata["first_name"];
  flight = jdata["flight"];
  stat = jdata["status"];

  Serial.println(nam);
  Serial.println(f_name);
  Serial.println(flight);
  Serial.println(stat);

  Serial.println(httpcode);
  Serial.println(F("\n**End Reading**\n"));

  delay(100);

  mfrc522.PICC_HaltA();
}
