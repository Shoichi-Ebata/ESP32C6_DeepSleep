// arduinoipin(Tx)→ESP32C6 GPIO　0,2 並列接続
// arduinoからの受信のみなので Rx0　と　C6のD1は接続なしでOK

#include <esp_sleep.h>
#include <SoftwareSerial.h>

#define RX_PIN 0
#define TX_PIN 1
#define WAKE_PIN 2

#define TIMEOUT_MS 10000

SoftwareSerial mySerial(RX_PIN, TX_PIN);  // RX, TX
unsigned long lastReceiveTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(WAKE_PIN, INPUT);  // wake pin

  esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
  if (cause == ESP_SLEEP_WAKEUP_EXT1) {
    Serial.println("Wakeup by GPIO2");
  } else {
    Serial.println("Normal boot");
  }

  mySerial.begin(9600);
  lastReceiveTime = millis();
}

void loop() {
  if (mySerial.available()) {
    String received = mySerial.readStringUntil('\n');
    Serial.println("受信: " + received);
    lastReceiveTime = millis();
  }

  if (millis() - lastReceiveTime > TIMEOUT_MS) {
    Serial.println("10秒間通信なし → DeepSleep");
    mySerial.end();

    // ESP32C6用：ext1でGPIO2をHIGH検出
    esp_sleep_enable_ext1_wakeup(1ULL << WAKE_PIN, ESP_EXT1_WAKEUP_ANY_HIGH);

    delay(100);  // Serial flush
    esp_deep_sleep_start();
  }
}
