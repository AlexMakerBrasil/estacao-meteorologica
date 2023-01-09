/* 
Estação meteorológica
Alexandro Costa
alexiatba@gmail.com
*/

#include <Wire.h>  
#include <LiquidCrystal_I2C.h> 
LiquidCrystal_I2C lcd(0x27, 20, 4); 

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include<ESP8266WiFi.h>
#include "DHT.h"

#define DHTPIN 13 
#define DHTTYPE DHT22 

DHT dht(DHTPIN, DHTTYPE); 

const char* ssid  = "IATEVENTOS";
const char* senha = "!@T_&V3NT0$";

// servidor web 80
WiFiServer server(80);

int ledA = 12;
int ledB = 11;
int lux  = A0;

void setup() {
  Serial.begin(115200);
  delay(10);
  dht.begin();
 
  Serial.print("Conectando a rede: ");
  Serial.println(ssid);

  WiFi.begin(ssid, senha);

  while(WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.print(".");
  }
  Serial.println("WiFi connectado");

  server.begin();
  Serial.println("Servidor iniciado");
  
  Serial.print("Use a seguinte URL: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());

  Serial.println("/");  

  lcd.init();   
  lcd.backlight();
  lcd.setCursor(0, 0); 
  lcd.print("Estacao IAT"); 
  lcd.setCursor(0, 1);
  lcd.print("IP: "); 
  lcd.setCursor(4, 1); 
  lcd.print(WiFi.localIP()); 
 
}
  int statusA   = LOW;
  int statusB   = LOW;
  int statusLux = 0; 
void loop() {
  WiFiClient cliente = server.available();
  if (!cliente){
    return;
  }
  Serial.println("Novo cliente");
  while(!cliente.available()){
    delay(1);
  }
  String requisicao = cliente.readStringUntil('/r');
  Serial.println(requisicao);
  cliente.flush();
   if(requisicao.indexOf("/LED1=ON") != -1){
    statusA = HIGH;
   }
   if(requisicao.indexOf("/LED1=OFF") != -1){
    statusA = LOW;
   }


  if(requisicao.indexOf("/LED2=ON") != -1){
    statusB = HIGH;
  }
  if(requisicao.indexOf("/LED2=OFF") != -1){
    statusB = LOW;
  }
  
  Serial.print("Status A: ");
  Serial.println(statusA);
  Serial.print("StatusB: ");
  Serial.println(statusB);
  digitalWrite(ledA, statusA);
  digitalWrite(ledB, statusB);



  // Criando uma página web, no formato HTML para nosso controle
  cliente.println("HTTP/1.1 200 OK");
  cliente.println("Content-Type:text/html");
  cliente.println("");
  cliente.println("<!DOCTYPE html>");
  cliente.println("<html>");
  cliente.println("<head>");
  cliente.println("<meta charset='UTF-8'>");
  cliente.println("<title>Estação Meteorológica do IAT</title>");
  cliente.println("</head>");
  cliente.println("<body>");
    
  cliente.println("<h2>Instituto Anísio Teixeira</h2><h3>Estação Meteorológica</h3>");
  cliente.println("<div>");
  cliente.println("<h3>Painel de controle</h3>");
  cliente.println("<div>");

  float u = dht.readHumidity();
  float t = dht.readTemperature();

  if(isnan(t) || isnan(u)){
    Serial.println("falha de leitura");
  }else{
    Serial.print("Umidade: ");
    Serial.println(u);
    
    Serial.print("Temperatura: ");
    Serial.println(t);
  }

  cliente.print("Umidade: ");
  cliente.println(u);
  cliente.print("<br><br>");

  cliente.print("Temperatura: ");
  cliente.print(t);
   cliente.println(" ºC");
  cliente.print("<br><br>");
       
  cliente.println("<a href=\"/LED1=ON\"><button> Ligar </button></a>");
  cliente.println("<a href=\"/LED1=OFF\"><button> Desligar </button></a><br>");
  
  cliente.println("<a href=\"/LED2=ON\"><button> Ligar </button></a>");
  cliente.println("<a href=\"/LED2=OFF\"><button> Desligar </button></a><br>");
  
  cliente.println("<br><br>");
  cliente.println("<div class=\"painel\">");
  
  if(statusA){
    cliente.println("<p style=\"background-color:green\">LED1 ligado</p>");
  }else{
    cliente.println("<p style=\"background-color:red\">LED1 Desligado</p>");
  }
  
  if(statusB){
    cliente.println("<p style=\"background-color:green\">LED2 ligado</p>");
  }else{
    cliente.println("<p style=\"background-color:red\">LED2 Desligado</p>");
  }
  
  cliente.println("</div>");
  cliente.println("</div>");
  cliente.println("</div>");
  
  cliente.println("</body>");
  cliente.println("</html>");
    delay(1); 
}
