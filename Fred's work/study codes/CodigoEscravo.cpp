/**
 *  ESP-01S - ESCRAVO
 *  Usa GPIO0,1,2,3 como PWM (IN1,IN2,IN3,IN4)
 */
#include <ESP8266WiFi.h>
#include <espnow.h>

struct Packet {
  uint8_t cmd;      // 'F','B','L','R','S','V'
  uint16_t value;   // velocidade
};

int vel = 600;   // default

// Mapeamento fixo
const uint8_t IN1 = 0;   // Motor A Forward
const uint8_t IN2 = 1;   // Motor A Back
const uint8_t IN3 = 2;   // Motor B Forward
const uint8_t IN4 = 3;   // Motor B Back

void parar() {
  analogWrite(IN1,0); analogWrite(IN2,0);
  analogWrite(IN3,0); analogWrite(IN4,0);
}
void frente() {
  analogWrite(IN1,vel); analogWrite(IN2,0);
  analogWrite(IN3,vel); analogWrite(IN4,0);
}
void tras() {
  analogWrite(IN1,0); analogWrite(IN2,vel);
  analogWrite(IN3,0); analogWrite(IN4,vel);
}
void esquerda() {
  analogWrite(IN1,0);   analogWrite(IN2,vel);
  analogWrite(IN3,vel); analogWrite(IN4,0);
}
void direita() {
  analogWrite(IN1,vel); analogWrite(IN2,0);
  analogWrite(IN3,0);   analogWrite(IN4,vel);
}

void recepcao(uint8_t *mac, uint8_t *data, uint8_t len){
  if (len != sizeof(Packet)) return;
  Packet p;  memcpy(&p, data, sizeof(p));
  if (p.cmd=='V') { vel = constrain(p.value,0,1023); return; }
  switch (p.cmd){
    case 'F': frente();   break;
    case 'B': tras();     break;
    case 'L': esquerda(); break;
    case 'R': direita();  break;
    case 'S': parar();    break;
  }
}

void setup() {
  pinMode(IN1,OUTPUT); pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT); pinMode(IN4,OUTPUT);
  parar();

  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(recepcao);
  // Não inicia Serial para liberar GPIO1/3
}

void loop() {}   // Tudo ocorre na interrupção do ESP-NOW
