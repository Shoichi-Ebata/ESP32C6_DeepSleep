// Arduino

#include <SoftwareSerial.h>
SoftwareSerial mySerial(0, 1); // RX, TX

int LED = 13;
int count = 0;

void setup(){
  Serial.begin(9600);  
  mySerial.begin(9600); // ソフトウェアシリアルの初期化

  pinMode(LED, OUTPUT);
}

void loop(){
  count++;
  Serial.print("counter = ");
  Serial.println(count);
  
  mySerial.listen();
  mySerial.write(count);
  
  if((count % 2) == 1){ 
    digitalWrite(LED, HIGH);
  }else{
    digitalWrite(LED, LOW);
  }
  delay(1000);
}
