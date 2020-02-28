#include <ESP8266WiFi.h>
 
const char* ssid = "redea";
const char* password = "!g@t0pret0203154121"; 
 
// BYTES EM HEXA DE COMANDOS DO RELE, ON/OFF
uint8_t relayOn[] = {0xA0, 0x01, 0x00, 0xA1};
uint8_t relayOff[] = {0xA0, 0x01, 0x01, 0xA2};

#define RL 0

WiFiServer server(80);
 
void setup() 
{
  Serial.begin(115200);
  
	pinMode(RL, OUTPUT);
  
	digitalWrite(RL, LOW);
  digitalWrite(RL, HIGH);
 
  Serial.println();
  Serial.println();
  Serial.print(" Conectando a rede... ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println(" WiFi Conectado");
 
  server.begin();
  Serial.println(" Servidor Iniciado");
 
  Serial.print(" wwww. ");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() 
{
  // VERIFICA SE TEM ALGUM CLIENT CONECTANDO 
  WiFiClient client = server.available();
  if (!client) 
  {
    return;
  }
 
  // VERIFICA SE TEM ALGUM DADO SENDO TRASMITIDO 
  Serial.println(" Novo Cliente");
  while(!client.available())
  {
    delay(1);
  }

  // LÊ A PRIMEIRA LINHA DA REQUISIÇÃO
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  int value = LOW;
  if (request.indexOf("/RELAY=ON") != -1)  
  {
    Serial.println("RELAY=ON");
    Serial.write(relayOn, 0);
    value = LOW;
  }
  if (request.indexOf("/RELAY=OFF") != -1)  
  {
    Serial.println("RELAY=OFF");
    Serial.write(relayOff, 0);
    value = HIGH;
  }
  
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  this is a must
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head><title>ESP8266 RELAY Control</title></head>");
  client.print("Relay is now: ");
 
  if(value == HIGH) 
  {
    client.print("OFF");
  } 
  else 
  {
    client.print("ON");
  }
  client.println("<br><br>");
  client.println("Turn <a href=\"/RELAY=OFF\">OFF</a> RELAY<br>");
  client.println("Turn <a href=\"/RELAY=ON\">ON</a> RELAY<br>");
  client.println("</html>");
 
  delay(1);
  Serial.println(" Client disonnected");
  Serial.println("");
}
