#include "Arduino.h"

void setup() {
  pinMode(6, OUTPUT); // 날씨 전송 pin
  pinMode(2,INPUT); // 초음파
  Serial2.begin(115200);
  Serial.begin(115200);

  delay(10000);
  Serial.println("Started");
}

int is_OK() {
  if (Serial2.available()) {
    String data = Serial2.readStringUntil('\n');
    int len = data.length();
    
    if (len > 2) {
      const char *buf = data.c_str();
      if (buf[0] == 'O' && buf[1] == 'K') {
        return 1;
      }
    }
  }

  return 0;
}

#define STATE_CREATE        1
#define STATE_CREATE_WAIT   2
#define STATE_CONNECT       3
#define STATE_CONNECT_WAIT  4
#define STATE_WRITE         5
#define STATE_READ          6
unsigned long lastTime = 0;
unsigned long time_now = 0;
void loop() {
  static int state = 1;

  switch (state) {

    case STATE_CREATE:
      Serial.write("Create ...\r\n");
      Serial2.write("AT+WSOCR=0,shscript.com,80,1,0\r\n");
      state = STATE_CREATE_WAIT;
      lastTime = millis();
      break;
    
    case STATE_CREATE_WAIT:
      time_now = millis();
      if(time_now - lastTime > 10000) {
        state = STATE_CREATE;
        Serial.println("Create fail...");
        break;
      }
      if (is_OK()) {
        Serial.write("Create OK\r\n");
        state = STATE_CONNECT;
      }
      break;
    
    case STATE_CONNECT:
      Serial.write("Connect ...\r\n");
      Serial2.write("AT+WSOCO=0\r\n");
      state = STATE_CONNECT_WAIT;
      lastTime = millis();
      break;

    case STATE_CONNECT_WAIT:
      time_now = millis();
      if(time_now - lastTime > 10000) {
        state = STATE_CREATE;
        Serial.println("CONNECT fail...");
        break;
      }
      if (is_OK()) {
        Serial.write("Connect OK\r\n");
        state = STATE_WRITE;
      }
      break;

    case STATE_WRITE:
      if(digitalRead(2)==1) Serial2.write("AT+WSOWR=0,70,GET /?us=1 HTTP/1.1\r\n");
      else Serial2.write("AT+WSOWR=0,70,GET /?us=0 HTTP/1.1\r\n");
      Serial2.write("HOST: weather.shscript.com\r\n");
      Serial2.write("Connection: close\r\n"); 
      Serial2.write("\r\n");
      Serial2.write("\r\n");
      Serial2.flush();

      state = STATE_READ;
      lastTime = millis();
      break;

    case STATE_READ:
    
      for (;;) {
        time_now = millis();
        if(time_now - lastTime > 10000) {
          state = STATE_CREATE;
          Serial.println("READ fail...");
          break;
        }
        if (Serial2.available()) {
          String data = Serial2.readStringUntil('\n');
          const char *buf = data.c_str();
          Serial.println(buf);
          if( *buf == '0') {
            Serial.println("비가 안와요");
            digitalWrite(6,LOW);

            break;
          }
          else if (*buf == '1') {
            Serial.println("비가 와요");
            digitalWrite(6,HIGH);
            break;
          }
        }
      }

      delay(10000);
      Serial2.flush();
      state = STATE_CREATE;
      break;
  }
}

