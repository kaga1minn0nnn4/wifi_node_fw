#include <Arduino.h>
#include "wrap_wifi.hpp"
#include "Servo.h"
#include <string>

Servo myservo;

WrapWiFiLib::WrapWiFi wifi_handler{};
WiFiServer server(5000);

char tx_msg[4096];
uint8_t tx_data;
uint8_t led_status = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(5, OUTPUT);
  pinMode(16, OUTPUT);
  printf("test\n");
  printf("connecting...");
  //wifi_handler.SetupFixedIp(WrapWiFiLib::setup_type_t::sta_mode,"aterm-8386ec-g","0b3ea4fcebd9e",IPAddress{10,6,140,242},IPAddress{255,255,255,0});
  digitalWrite(16, 0);
  wifi_handler.SetupFixedIp(
    WrapWiFiLib::setup_type_t::sta_mode, 
    "te-py3",
    "hinanaitns", 
    IPAddress{192, 168, 0, 100}, 
    IPAddress{255, 255, 255 , 0}
  );
  
  printf("!connected\n");
  printf("%s\n", wifi_handler.GetMyIpAddress().toString().c_str());
  digitalWrite(5, 1);
  server.begin();
  myservo.attach(4);
}

void loop() {
  // put your main code here, to run repeatedly:
  //printf("new client\n");
  
  WiFiClient client = server.available();

  if (client) {
    printf("client connect\n");
    printf("connected : %d\n", client.connected());
    //while (client.connected()) {
      int size = client.available();
      printf("size : %d\n", size);
      if (size) {
        printf("size:%d\n",size);
        for (int i = 0; i < size; i++) {
          tx_msg[i] = client.read();
        }
        std::string msg{tx_msg};
        printf("%s\n", msg.c_str());
        led_status = !led_status;
        printf("%d\n",led_status);
      }
    //}
    client.stop();
    printf("led_status : %d\n", led_status);
  }
  digitalWrite(5, led_status);
  myservo.write(100 * led_status);
}