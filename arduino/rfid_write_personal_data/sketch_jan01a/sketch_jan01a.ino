
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  char buff[16];
  if(Serial.available()){
    char data = Serial.read();

    if (data == 'd'){
      digitalWrite(LED_BUILTIN, 1);
      Serial.println("Enable");
    }
    if(data=='b'){
      digitalWrite(LED_BUILTIN, 0);
      Serial.println("disable");
    }
  }
  
}
