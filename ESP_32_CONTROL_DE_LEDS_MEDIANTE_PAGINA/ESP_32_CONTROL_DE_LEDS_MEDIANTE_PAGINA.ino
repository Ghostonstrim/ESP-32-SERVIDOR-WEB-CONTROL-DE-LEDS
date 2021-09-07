#include <WiFi.h>

//------------------Servidor Web en puerto 80---------------------

WiFiServer server(80);

//---------------------Credenciales de WiFi-----------------------

const char* ssid     = "MALUPONTON";
const char* password = "Paulette20";

//---------------------VARIABLES GLOBALES-------------------------
int contconexion = 0;

String header; // Variable para guardar el HTTP request

String estadoSalida = "off";

const int verde = 27;
const int blanco =15;
const int rojo =21;
const int amarillo =19;

//------------------------CODIGO HTML------------------------------
String pagina = "<!DOCTYPE html>"
"<html>"
"<head>"
"<meta charset='utf-8' />"
"<title>Servidor Web ESP32</title>"
"</head>"
"<body style='background-color: rgb(134, 228, 223);'>"
"<center>"
"<h1>Servidor Web ESP32</h1>"

"<h2>Foco Verde</h2>"
"<p><a href='/onV'><button style='height:50px;width:100px;background-color: chartreuse'>ON</button></a></p>"
"<p><a href='/offV'><button style='height:50px;width:100px;background-color: chartreuse'>OFF</button></a></p>"
"<div></div>"
"<h2>Foco Blanco</h2>"
"<p><a href='/onB'><button style='height:50px;width:100px;background-color: cornsilk'>ON</button></a></p>"
"<p><a href='/offB'><button style='height:50px;width:100px;background-color: cornsilk'>OFF</button></a></p>"
"<div></div>"
"<h2>Foco Rojo</h2>"
"<p><a href='/onR'><button style='height:50px;width:100px;background-color: crimson'>ON</button></a></p>"
"<p><a href='/offR'><button style='height:50px;width:100px;background-color: crimson'>OFF</button></a></p>"
"<div></div>"
"<h2>Foco Amarillo</h2>"
"<p><a href='/onA'><button style='height:50px;width:100px;background-color: rgb(253, 237, 8);'>ON</button></a></p>"
"<p><a href='/offA'><button style='height:50px;width:100px;background-color: rgb(255, 239, 10);'>OFF</button></a></p>"
"</center>"
"</body>"
"</html>";




//---------------------------SETUP--------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println("");
  
  pinMode(verde, OUTPUT); 
  pinMode(blanco, OUTPUT); 
  pinMode(rojo, OUTPUT); 
  pinMode(amarillo, OUTPUT); 
  digitalWrite(verde, LOW);
  digitalWrite(blanco, LOW);
  digitalWrite(rojo, LOW);
  digitalWrite(amarillo, LOW);

  // Conexión WIFI
  WiFi.begin(ssid, password);
  //Cuenta hasta 50 si no se puede conectar lo cancela
  while (WiFi.status() != WL_CONNECTED and contconexion <50) { 
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion <50) {
      
      Serial.println("");
      Serial.println("WiFi conectado");
      Serial.println(WiFi.localIP());
      server.begin(); // iniciamos el servidor
  }
  else { 
      Serial.println("");
      Serial.println("Error de conexion");
  }
}

//----------------------------LOOP----------------------------------

void loop(){
  WiFiClient client = server.available();   // Escucha a los clientes entrantes

  if (client) {                             // Si se conecta un nuevo cliente
    Serial.println("New Client.");          // 
    String currentLine = "";                //
    while (client.connected()) {            // loop mientras el cliente está conectado
      if (client.available()) {             // si hay bytes para leer desde el cliente
        char c = client.read();             // lee un byte
        Serial.write(c);                    // imprime ese byte en el monitor serial
        header += c;
        if (c == '\n') {                    // si el byte es un caracter de salto de linea
          // si la nueva linea está en blanco significa que es el fin del 
          // HTTP request del cliente, entonces respondemos:
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // enciende y apaga el GPIO
            if (header.indexOf("GET /onV") >= 0) {
              Serial.println("GPIO onV");
              estadoSalida = "on";
              digitalWrite(verde, HIGH);
            } else if (header.indexOf("GET /offV") >= 0) {
              Serial.println("GPIO offV");
              estadoSalida = "off";
              digitalWrite(verde, LOW);
            } else if (header.indexOf("GET /onB") >= 0) {
              Serial.println("GPIO onB");
              estadoSalida = "on";
              digitalWrite(blanco, HIGH);
            } else if (header.indexOf("GET /offB") >= 0) {
              Serial.println("GPIO off");
              estadoSalida = "offB";
              digitalWrite(blanco, LOW);
            } else if (header.indexOf("GET /onR") >= 0) {
              Serial.println("GPIO onR");
              estadoSalida = "on";
              digitalWrite(rojo, HIGH);
            } else if (header.indexOf("GET /offR") >= 0) {
              Serial.println("GPIO offR");
              estadoSalida = "off";
              digitalWrite(rojo, LOW);
            } else if (header.indexOf("GET /onA") >= 0) {
              Serial.println("GPIO onA");
              estadoSalida = "on";
              digitalWrite(amarillo, HIGH);
            } else if (header.indexOf("GET /offA") >= 0) {
              Serial.println("GPIO off");
              estadoSalida = "offA";
              digitalWrite(amarillo, LOW);
            }
            
            
            // Muestra la página web
            client.println(pagina);
            
            // la respuesta HTTP temina con una linea en blanco
            client.println();
            break;
          } else { // si tenemos una nueva linea limpiamos currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // si C es distinto al caracter de retorno de carro
          currentLine += c;      // lo agrega al final de currentLine
        }
      }
    }
    // Limpiamos la variable header
    header = "";
    // Cerramos la conexión
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
