#include <ESP8266WiFi.h>

const char* ssid = "ESP_Carrinho";
const char* password = "12345678";

WiFiServer server(80);

// Mapeamento dos pinos
const int IN1 = 0; // GPIO0
const int IN2 = 1; // GPIO1 (TX)
const int IN3 = 2; // GPIO2
const int IN4 = 3; // GPIO3 (RX)

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Inicia o Wi-Fi como Access Point
  WiFi.softAP(ssid, password);
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  while (client.connected()) {
    if (client.available()) {
      String request = client.readStringUntil('\r');
      client.flush();

      if (request.indexOf("/f") != -1) { // Frente
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
      } else if (request.indexOf("/b") != -1) { // Ré (Back)
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
      } else if (request.indexOf("/l") != -1) { // Esquerda (Left)
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
      } else if (request.indexOf("/r") != -1) { // Direita (Right)
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
      } else if (request.indexOf("/s") != -1) { // Stop
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
      }

      // Interface HTML
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      client.println("<html><body>");
      client.println("<h2>Carrinho Controle Wi-Fi</h2>");
      client.println("<a href=\"/f\">Frente</a><br>");
      client.println("<a href=\"/b\">Ré</a><br>");
      client.println("<a href=\"/l\">Esquerda</a><br>");
      client.println("<a href=\"/r\">Direita</a><br>");
      client.println("<a href=\"/s\">Parar</a><br>");
      client.println("</body></html>");
      break;
    }
  }
  delay(10);
  client.stop();
}