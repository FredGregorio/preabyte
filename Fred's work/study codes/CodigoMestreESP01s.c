/**
 *  ESP-01S - MESTRE  (recebe comandos pela Serial)
 *  Comandos ASCII no serial: 
 *    1F   = escravo1 Frente
 *    1B   = escravo1 Back (Ré)
 *    1L   = escravo1 Left
 *    1R   = escravo1 Right
 *    1S   = escravo1 Stop
 *    1V700= escravo1 Velocidade 700  (0-1023)
 *    2F ... idem para escravo2
 */
#include <ESP8266WiFi.h>
#include <espnow.h>

struct Packet {
  uint8_t cmd;      // 'F','B','L','R','S','V'
  uint16_t value;   // velocidade (0-1023) ou 0
};

uint8_t mac1[] = {0x50,0x02,0x91,0xC9,0x43,0xF6};  // Escravo 1
uint8_t mac2[] = {0x84,0xF3,0x12,0xAA,0xBB,0x02};  // Escravo 2

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
      Serial.println("Erro ESP-NOW");
      return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(mac1, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(mac2, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  Serial.println("Mestre pronto. Digite comando …");
}

void envia(uint8_t *mac, Packet &p){
  esp_now_send(mac, (uint8_t*)&p, sizeof(p));
}

void loop() {
  static String line;
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (line.length() >= 2) {
        int slave = line[0]-'0';   // 1 ou 2
        char cmd  = line[1];       // F,B,L,R,S,V
        Packet p = { (uint8_t)cmd, 0 };
        if (cmd=='V') p.value = line.substring(2).toInt();
        uint8_t* mac = (slave==1)? mac1 : mac2;
        envia(mac,p);
      }
      line = "";   // limpa
    } else {
      line += c;
    }
  }
}
