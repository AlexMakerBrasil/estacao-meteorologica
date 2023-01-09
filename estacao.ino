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
cliente.println("<html lang=\"pt-br\">");
cliente.println("<head>");
cliente.println("  <title>Instituto Anisio Teixeira</title>");
cliente.println("  <meta charset=\"utf-8\">");
cliente.println("  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
cliente.println("  <link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/css/bootstrap.min.css\" rel=\"stylesheet\">");
cliente.println("  <script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/js/bootstrap.bundle.min.js\"></script>");
cliente.println("</head>");
cliente.println("<body>");

cliente.println("<div class=\"container-fluid p-5 bg-primary text-white text-center\">");
cliente.println("  <h1>Estação Meteorológica do Instituto Anisio Teixeira</h1>");
cliente.println("  <p>Educar é crescer. E crescer é viver. Educação é, assim, vida no sentido mais autêntico da palavra.</p>"); 
cliente.println("   <span>Anísio Teixeira</span> ");
cliente.println("</div>");
  

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

 
cliente.println("<div class=\"container mt-5\">");
cliente.println(" <div class=\"row\">");
    
cliente.println("    <div class=\"row mb-2\">");
cliente.println("      <div class=\"col-md-6\">");
cliente.println("        <div class=\"row g-0 border rounded overflow-hidden flex-md-row mb-4 shadow-sm h-md-250 position-relative\">");
cliente.println("          <div class=\"col p-4 d-flex flex-column position-static\">");
cliente.println("            <strong class=\"d-inline-block mb-2 text-primary\">Local: Espaço externo Maria Felipa Lab</strong>");
cliente.println("            <h3 class=\"mb-0\">Estação Meteorológica</h3>");
cliente.println("            <div class=\"mb-1 text-muted\">Sensor DHT22.</div>");
cliente.print("TEMPERATURA: ");
cliente.print(t);
cliente.println(" ºC");
cliente.print("UMIDADE: ");
cliente.println(u);
cliente.println("%");            
cliente.println("          </div>");
cliente.println("          <div class=\"col-auto d-none d-lg-block\">");
cliente.println("         <svg class=\"bd-placeholder-img\" width=\"200\" height=\"250\" xmlns=\"http://www.w3.org/2000/svg\" role=\"img\" aria-label=\"Placeholder: Thumbnail\" preserveAspectRatio=\"xMidYMid slice\" focusable=\"false\"><title>Placeholder</title><rect width=\"100%\" height=\"100%\" fill=\"#55595c\"/><text x=\"50%\" y=\"50%\" fill=\"#eceeef\" dy=\".3em\"> </text></svg>");
cliente.println("          </div>");
cliente.println("        </div>");
cliente.println("      </div>");
      
cliente.println("      <div class=\"col-md-6\">");
cliente.println("        <div class=\"row g-0 border rounded overflow-hidden flex-md-row mb-4 shadow-sm h-md-250 position-relative\">");
cliente.println("          <div class=\"col p-4 d-flex flex-column position-static\">");
cliente.println("            <strong class=\"d-inline-block mb-2 text-primary\">Local: Espaço externo Maria Felipa Lab</strong>");
cliente.println("            <h3 class=\"mb-0\">Luminosidade e CO2</h3>");
cliente.println("            <div class=\"mb-1 text-muted\">LDR SENSOR e MQ-7</div>");
cliente.println("            <p class=\"card-text mb-auto\">LUMINOSIDADE: </p>");
cliente.println("            <p class=\"card-text mb-auto\">SENSOR DE CO2: </p>");
            
cliente.println("          </div>");
cliente.println("          <div class=\"col-auto d-none d-lg-block\">");
cliente.println("            <svg class=\"bd-placeholder-img\" width=\"200\" height=\"250\" xmlns=\"http://www.w3.org/2000/svg\" role=\"img\" aria-label=\"Placeholder: Thumbnail\" preserveAspectRatio=\"xMidYMid slice\" focusable=\"false\"><title>Placeholder</title><rect width=\"100%\" height=\"100%\" fill=\"#55595c\"/><text x=\"50%\" y=\"50%\" fill=\"#eceeef\" dy=\".3em\"> </text></svg>");
cliente.println("          </div>");
cliente.println("        </div>");
cliente.println("      </div>");
cliente.println("    </div>");



cliente.println("<a href=\"/LED1=ON\"><button> Ligar </button></a>");
cliente.println("<a href=\"/LED1=OFF\"><button> Desligar </button></a><br>");
  
cliente.println("<a href=\"/LED2=ON\"><button> Ligar </button></a>");
cliente.println("<a href=\"/LED2=OFF\"><button> Desligar </button></a><br>");


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


cliente.println("  </div>");
cliente.println("</div>");

cliente.println("</body>");
cliente.println("</html>");

    delay(1); 
}
