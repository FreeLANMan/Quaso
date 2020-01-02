// versão QuaSo014 - app Resumo

ADC_MODE(ADC_VCC);
                      //       mmmmmmmmmmmmmmmmmmmmmmmmm
// máximo de texto na tela: "min.: 12.00ºC, máx.: 34.00"

// Pinagem display: GND->G, VDD->5V, SCK->D1, SDA->D2

// Pinagem: Receptor IR ->A0, LED UV -> D7

// -------------------------------- Definições e bibliotecas -----------------------------------------

// Bibliotecas incluidas:
#include <ESP8266WiFi.h> // #include WiFi Library
#include <NTPClient.h>//Biblioteca do NTP. https://github.com/arduino-libraries/NTPClient
#include <WiFiUdp.h>//Biblioteca do UDP. 
#include <ESP8266WebServer.h> // biblioteca do servidor mais sofisticado
#include <FS.h>   // Include the SPIFFS library
//#include <Thermistor.h> 

#include <Wire.h>
#include <SSD1306Wire.h>       // legacy: #include "SSD1306.h"
// Include the UI lib
#include <OLEDDisplayUi.h>
// OR #include "SH1106Wire.h"   // legacy: #include "SH1106.h"

// For a connection via I2C using brzo_i2c (must be installed) include:
// #include <brzo_i2c.h>        // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Brzo.h"
// OR #include "SH1106Brzo.h"

// For a connection via SPI include:
// #include <SPI.h>             // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Spi.h"
// OR #include "SH1106SPi.h"

// Definições para o sistema de conexão automática ao wifi
#define SERIAL_BAUD       9600 /* Serial Baud Rate */
#define WIFI_DELAY        500 /* Delay paramter for connection. */
#define MAX_SSID_LEN      32 /* Max SSID octets. */
#define MAX_CONNECT_TIME  10000 /* Wait this much until device gets IP. */
char ssid[MAX_SSID_LEN] = ""; /* SSID that to be stored to connect. */

// app Wi-Fi Seguro
char ssid1[MAX_SSID_LEN] = ""; /* SSID that to be stored to connect. */
char ssid2[MAX_SSID_LEN] = ""; /* SSID that to be stored to connect. */
char ssid3[MAX_SSID_LEN] = ""; /* SSID that to be stored to connect. */

// Definições para pegar a hora da Internet
// Fonte: NTPClient ntp(udp, "a.st1.ntp.br
WiFiUDP udp;//Cria um objeto "UDP".
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);//Cria um objeto "NTP" com as configurações.
String hora; //Váriavel que armazenara o horario do NTP.

// definições para o webserver
//WiFiServer server(80); //CASO OCORRA PROBLEMAS COM A PORTA 80, UTILIZE OUTRA (EX:8082,8089) E A CHAMADA DA URL FICARÁ IP:PORTA(EX: 192.168.0.15:8082)
//WiFiClient client = server.available();
ESP8266WebServer server(80);

// definições botões adaptado de https://community.wia.io/d/49-wemos-d1-mini-esp8266-smart-iot-button-with-messagebird-sms-and-voice-messages
byte esquerdo = D0;
byte direito = D8;
byte confirma = D5;

float voltagem = 0.01;

byte tela = 0; // guarda em que tela está
byte app = 1; // guarda o app selecionado
byte aplicativo = 0; // guarda o aplicativo escolhido
String linha0 = " ";
String linha1 = " ";
String linha2 = " ";
String linha3 = " ";
String linha4 = " ";
String linha5 = " ";
int i,j;

// Definições para suporte ao display
SSD1306Wire display(0x3c, SDA, SCL); // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h
// SSD1306Wire display(0x3c, D3, D5);  // ADDRESS, SDA, SCL  -  If not, they can be specified manually.
// SSD1306Wire display(0x3c, SDA, SCL, GEOMETRY_128_32);  // ADDRESS, SDA, SCL, OLEDDISPLAY_GEOMETRY  -  Extra param required for 128x32 displays.
// SH1106 display(0x3c, SDA, SCL);     // ADDRESS, SDA, SCL

// Initialize the OLED display using brzo_i2c:
// SSD1306Brzo display(0x3c, D3, D5);  // ADDRESS, SDA, SCL
// or
// SH1106Brzo display(0x3c, D3, D5);   // ADDRESS, SDA, SCL

// Initialize the OLED display using SPI:
// D5 -> CLK
// D7 -> MOSI (DOUT)
// D0 -> RES
// D2 -> DC
// D8 -> CS
// SSD1306Spi display(D0, D2, D8);  // RES, DC, CS
// or
// SH1106Spi display(D0, D2);       // RES, DC
byte rnd1, rnd2;

// definições app cronometro
byte comecou = 0;
int horas = 0;
int minutos = 0;
int segundos = 0;
int ultimo = 0;
unsigned long inicio = 0;
unsigned long tsegs = 0;

// definições app termometro
#include "DHTesp.h" // Necessária para usar o módulo de temperatura e humidade (app 1)
DHTesp dht;
int kelvin = 0;
float hmax = 0;
float hmin = 100;
float tmax = 0;
float tmin = 100;

//visualizador de arquivos
String getContentType(String filename); // convert the file extension to the MIME type
bool handleFileRead(String path);       // send the right file to the client (if it exists)
//String arquivo;
String path;

//app Resumo
//Thermistor temp(2); //VARIÁVEL DO TIPO THERMISTOR
//int kelvin = 0;
// pino do fototransistor = A0
byte IRrec = A0;
// definir o pino do reed switch: digital
// reedSwitch = D ? ;
//byte reedSwitch = D3;
String estado;
byte mag;

//app UV
byte LedUV = D7;



// --------------------------------- Funções ----------------------------------------------------

void handleRoot() {
  String temp = "<html><head><title>Quaso</title><meta charset='utf-8'><meta http-equiv='refresh' content='4'>";
  temp += "<style type='text/css'>body {background-color: #000000; color: white}</style></head><body>"; // define a cor de fundo e a cor das letras
  temp += "<h1>" + linha0 + "</h1>";
  temp += "<h1>" + linha1 + "</h1>";
  temp += "<h1>" + linha2 + "</h1>";
  temp += "<h1>" + linha3 + "</h1>";
  temp += "<h1>" + linha4 + "</h1>";
  temp += "<h1>" + linha5 + "</h1>";
  temp += "</body></html>";
  //server.sendHeader("Location", "/index.html",true);   //Redirect to our html web page
  //String temp = linha0 + linha1;
  server.send(200, "text/html", temp);
}

/* void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
} */


/* Scan available networks and sort them in order to their signal strength. */
void scanAndSort() {
  memset(ssid, 0, MAX_SSID_LEN);
  int n = WiFi.scanNetworks();
  Serial.println("Scan complete!");
  if (n == 0) {
    Serial.println("No networks available.");
  } else {
    Serial.print(n);
    Serial.println(" networks discovered.");
    int indices[n];
    for (int i = 0; i < n; i++) {
      indices[i] = i;
    }
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
          std::swap(indices[i], indices[j]);
        }
      }
    }
    for (int i = 0; i < n; ++i) {
      Serial.println("The strongest open network is:");
      Serial.print(WiFi.SSID(indices[i]));
      Serial.print(" ");
      Serial.print(WiFi.RSSI(indices[i]));
      Serial.print(" ");
      Serial.print(WiFi.encryptionType(indices[i]));
      Serial.println();
      if(WiFi.encryptionType(indices[i]) == ENC_TYPE_NONE) {
        memset(ssid, 0, MAX_SSID_LEN);
        strncpy(ssid, WiFi.SSID(indices[i]).c_str(), MAX_SSID_LEN);
        break;
      }
    }
  }
}


/* Scan close networks and sort them in order to their signal strength. */
void scanAndSort2() {
  memset(ssid1, 0, MAX_SSID_LEN);
  memset(ssid2, 0, MAX_SSID_LEN);
  memset(ssid3, 0, MAX_SSID_LEN);
  int n = WiFi.scanNetworks();
  Serial.println("Scan complete!");
  if (n == 0) {
    Serial.println("No networks available.");
  } 
  else {
    Serial.print(n);
    Serial.println(" networks discovered.");
    int indices[n];
    for (int i = 0; i < n; i++) {
      indices[i] = i;
    }
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
          std::swap(indices[i], indices[j]);
        }
      }
    }
    for (int i = 0; i < n; ++i) {
      //Serial.println("The strongest network is:");
      //Serial.print(WiFi.SSID(indices[i]));
      // Serial.print(" ");
      //Serial.print(WiFi.RSSI(indices[i]));
      //Serial.print(" ");
      // Serial.print(WiFi.encryptionType(indices[i]));
      // Serial.println();
      if(WiFi.encryptionType(indices[i]) != ENC_TYPE_NONE) {
        memset(ssid1, 0, MAX_SSID_LEN);
        strncpy(ssid1, WiFi.SSID(indices[i]).c_str(), MAX_SSID_LEN);
        Serial.print(WiFi.SSID(indices[i]));
        if(n >= 1) {
          memset(ssid2, 0, MAX_SSID_LEN);
          strncpy(ssid2, WiFi.SSID(indices[i+1]).c_str(), MAX_SSID_LEN);
          Serial.print(WiFi.SSID(indices[i+1]));
          if(n >= 2) {
          memset(ssid3, 0, MAX_SSID_LEN);
          strncpy(ssid3, WiFi.SSID(indices[i+2]).c_str(), MAX_SSID_LEN);
          Serial.print(WiFi.SSID(indices[i+2]));
        }
      if (WiFi.encryptionType(indices[i]) == ENC_TYPE_NONE) { 
        Serial.print(WiFi.SSID(indices[i]));
        Serial.print("É uma rede aberta.");
      }
        break;
      }
    }
  }
  }
}








// Função para imprimir no display Oled
void PraTela() { 
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  rnd1 = random(255);
  if (rnd1 == 1) {
    rnd2 = random(9);  
  }
  display.drawString(rnd2, 0, linha0);
  display.drawString(rnd2, 13, linha1);
  display.drawString(rnd2, 23, linha2);
  display.drawString(rnd2, 33, linha3);
  display.drawString(rnd2, 43, linha4);
  display.drawString(rnd2, 53, linha5);
  display.display();
}



// Função para imprimir no display Oled
void LimpaTela() { 
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  rnd1 = random(255);
  if (rnd1 == 1) {
    rnd2 = random(9);  
  }
  display.drawString(rnd2, 0, " ");
  display.drawString(rnd2, 13, " ");
  display.drawString(rnd2, 23, " ");
  display.drawString(rnd2, 33, " ");
  display.drawString(rnd2, 43, " ");
  display.drawString(rnd2, 53, " ");
  display.display();
}





// --------------------------------- Aplicativos ----------------------------------------------------


void aplicativo5() { // app Wi-Fi Seguro
  tela = 1;
  linha1 = " ";
  linha2 = " ";
  linha3 = " ";
  linha4 = " ";
  linha5 = " ";
  int senhaDesconhecida = 1;
  while (tela == 1) {
    PraTela();
    linha0 = "Wi-Fi Seguro";
    // Encontrar as redes de Wi-Fi
    //WiFi.disconnect(); //disconecta da rede em que está (se está em alguma)
    scanAndSort2(); //função que scaneia e classifica as redes com senhas
    delay(WIFI_DELAY);
    
    // se senha = nome da 
    if (strlen(ssid1) > 0) {
      WiFi.begin(ssid1, ssid1);
      linha1 = "Rede:" + String(ssid1);
      linha2 = "Senha avaliada: ";
      linha3 = String(ssid1);
      PraTela();
      unsigned short try_cnt = 0;
      while (WiFi.status() != WL_CONNECTED && try_cnt < MAX_CONNECT_TIME / WIFI_DELAY) {
        if (digitalRead(esquerdo) == HIGH) {
          //Serial.println("esquerdo apertado");
          //delay(170); // espera para cada apertada ser vista individualmente 
          tela = 0; // voltar pelo menu
        }
        delay(WIFI_DELAY);
        Serial.print(".");
        try_cnt++;
      }
      if(WiFi.status() == WL_CONNECTED) {
        Serial.println("");
        Serial.println(ssid1);
        Serial.println("Connection Successful!");
        Serial.println("Your device IP address is ");
        Serial.println(WiFi.localIP());
        //display.drawString(0, 0, "Conectado por WiFi!");
        linha4 = "Senha Correta!";
        senhaDesconhecida = 0;
        PraTela();
        if (digitalRead(esquerdo) == HIGH) {
          //Serial.println("esquerdo apertado");
          //delay(170); // espera para cada apertada ser vista individualmente 
          tela = 0; // voltar pelo menu
        }
      }
        

      // se senha = nome da rede + 123
      if (senhaDesconhecida == 1) {
      String senha = String(ssid1) + "123";
      WiFi.begin(ssid1, senha);
      linha1 = "Rede:" + String(ssid1);
      linha2 = "Senha avaliada: ";
      linha3 = String(senha);
      PraTela();
      unsigned short try_cnt = 0;
      while (WiFi.status() != WL_CONNECTED && try_cnt < MAX_CONNECT_TIME / WIFI_DELAY) {
        if (digitalRead(esquerdo) == HIGH) {
          //Serial.println("esquerdo apertado");
          //delay(170); // espera para cada apertada ser vista individualmente 
          tela = 0; // voltar pelo menu
        }
        delay(WIFI_DELAY);
        Serial.print(".");
        try_cnt++;
      }
      if(WiFi.status() == WL_CONNECTED) {
        Serial.println("");
        Serial.println("Connection Successful!");
        Serial.println("Your device IP address is ");
        Serial.println(WiFi.localIP());
        //display.drawString(0, 0, "Conectado por WiFi!");
        linha4 = "Senha Correta!";
        senhaDesconhecida = 0;
        PraTela();
        if (digitalRead(esquerdo) == HIGH) {
          //Serial.println("esquerdo apertado");
          //delay(170); // espera para cada apertada ser vista individualmente 
          tela = 0; // voltar pelo menu
        }
      }
    }
    }
    else {
      linha2 = "Sem redes seguras";
      if (digitalRead(esquerdo) == HIGH) {
        //Serial.println("esquerdo apertado");
        //delay(170); // espera para cada apertada ser vista individualmente 
        tela = 0; // voltar pelo menu
        break;
      }
    }
    
    //Pegando uma senha do arquivo
    File f = SPIFFS.open("/Senhas.dat", "r"); // r = abrir para leitura
    while(senhaDesconhecida == 1) {
      if (digitalRead(esquerdo) == HIGH) {
        //Serial.println("esquerdo apertado");
        //delay(170); // espera para cada apertada ser vista individualmente 
        tela = 0; // voltar pelo menu
      }
      String line = f.readStringUntil('\n');
      linha1 = "Rede:" + String(ssid1);
      linha2 = "Senha avaliada: ";
      linha3 = line;
      PraTela();
      Serial.println(line);
      //delay(500);
      if(strlen(ssid1) > 0) { /* Global ssid param need to be filled to connect. */
        //Serial.print("Connecting to ");
        //Serial.println(ssid1);
        WiFi.begin(ssid1, line);
        //delay(500);
        unsigned short try_cnt = 0;
        /* Wait until WiFi connection but do not exceed MAX_CONNECT_TIME */
        while (WiFi.status() != WL_CONNECTED && try_cnt < MAX_CONNECT_TIME / WIFI_DELAY) {
          if (digitalRead(esquerdo) == HIGH) {
            //Serial.println("esquerdo apertado");
            //delay(170); // espera para cada apertada ser vista individualmente 
            tela = 0; // voltar pelo menu
          }
          delay(WIFI_DELAY);
          Serial.print(".");
          try_cnt++;
        }
        if(WiFi.status() == WL_CONNECTED) {
          Serial.println("");
          Serial.println("Connection Successful!");
          Serial.println("Your device IP address is ");
          Serial.println(WiFi.localIP());
          //display.drawString(0, 0, "Conectado por WiFi!");
          linha4 = "Senha Correta!";
          senhaDesconhecida = 0;
          PraTela();
          while (tela == 1) {
            if (digitalRead(esquerdo) == HIGH) {
              //Serial.println("esquerdo apertado");
              delay(170); // espera para cada apertada ser vista individualmente 
              tela = 0; // voltar pelo menu
            }
            delay(500);
          }
          //break;
        }
        else {
          //linha4 = "Senha não encontrada!";
          PraTela();
          //f.close();
          Serial.println("Connection FAILED");
          if (digitalRead(esquerdo) == HIGH) {
            //Serial.println("esquerdo apertado");
            //delay(170); // espera para cada apertada ser vista individualmente 
            tela = 0; // voltar pelo menu
            break;
          }
        }
        }
      //f.close();
    }
}
}













void aplicativo4() { // app Luz Negra
  tela = 1;
  byte modoPisca = 0;
  LimpaTela();
  while (tela == 1) {
    digitalWrite(LedUV,HIGH);
    PraTela();
    linha0 = "LED UV";
    linha1 = " "; 
    linha2 = "Clique direito para modo pisca"; 
    linha3 = " "; 
    linha4 = " "; 
    linha5 = " "; 
    if (modoPisca == 1) {
         digitalWrite(LedUV,LOW);
    }
    if (digitalRead(esquerdo) == HIGH) {
      //Serial.println("esquerdo apertado");
      //delay(170); // espera para cada apertada ser vista individualmente 
      tela = 0; // voltar pelo menu
      digitalWrite(LedUV,LOW);
    }
    if (digitalRead(direito) == HIGH) {
      //Serial.println("esquerdo apertado");
      //delay(170); // espera para cada apertada ser vista individualmente 
      modoPisca = 1;
    }
    delay(2000);
  }
}



void aplicativo3() {
  // app Resumo dos dados 
  tela = 1;
  byte telaLigada = 1;
  while (tela == 1) {
    delay(500);
    server.handleClient();
    display.clear();
    PraTela();
    hora = ntp.getFormattedTime();//Armazena na váriavel HORA, o horario atual.
    byte h = dht.getHumidity(); // pega a umidade
    byte t = dht.getTemperature(); // pega a temperatura
    delay(500);
    t = t - 2; // correção pq tá na embalagem
    kelvin = t + 273,15;
    voltagem = ESP.getVcc();
    int redes = WiFi.scanNetworks();
    //mag = map(analogRead(IRrec),0, 1023, 0, 100); // Para evitar ocilações chatas
    //mag = mag - 7; //pra começar no zero
    mag = analogRead(IRrec); 
    mag = mag - 255;
    delay(500);
    linha0 = "Temp: "+ String(t) + "ºC, " + String(kelvin) + "K";
    linha1 = "Umidade: " + String(h) + "%";
    linha2 = " ";
    //if (digitalRead(reedSwitch) == LOW) {estado = "Sim";}
    //else {estado = "Não";}
    //linha3 = "imã detectado: " + estado;
    //linha4 = "Tempo ligado: " ; 
    linha3 = "Redes Wi-Fi: " + String(redes);
    linha4 = "Voltagem: " + String(voltagem);
    linha5 = "Hora: " + hora;
    // programar alternativas: se não tem hora, mostrar nome da rede de wifi conectada, se não tem também mostrar corrente elétrica
    //delay(500);
    if (digitalRead(esquerdo) == HIGH) { // para sair do app
      //Serial.println("esquerdo apertado");
      //delay(170); // espera para cada apertada ser vista individualmente 
      tela = 0; // voltar pelo menu
    }

    if (digitalRead(direito) == HIGH) { // para desligar/ligar a tela
      //Serial.println("esquerdo apertado");
      delay(170); // espera para cada apertada ser vista individualmente 
      if (telaLigada == 1) {
        display.displayOff();
        telaLigada = 0;
      }
      else {
        display.displayOn();
        telaLigada = 1;
      }
    }
    
  }
}


    // montando o que vai ser apresentado na tela:
    // máximo "min.: 12.00ºC, máx.: 34.00º"



void aplicativo2() {
  // cronometro para Quaso
  tela = 1;
  while (comecou == 0) {
    server.handleClient();
    PraTela();
    linha0 = "Cronômetro";
    linha1 = " ";
    linha2 = "Clique OK para iniciar a contagem.";
    linha3 = " ";
    linha4 = " ";
    linha5 = " ";
    //client.stop(); 
    //imprime(linha0,linha1,linha2,linha3,linha4,linha5);
    

  if (digitalRead(confirma) == HIGH) {
      Serial.println("confirma apertado");
      delay(170); // espera para cada apertada ser vista individualmente 
      inicio = millis() / 1000; // quantos segundos passaram desde que iniciou o sistema
      comecou = 1; 
      }
  }
  while (comecou == 1) { // se já começou a contagem https://www.instructables.com/id/TESTED-Timekeeping-on-ESP8266-Arduino-Uno-WITHOUT-/
    server.handleClient();
    tsegs = (millis() / long(1000)) - inicio;
    /* //segundos = tsegs - ultimo;
    //tsegs = (millis()/long (1000)) - inicio; // segundos desde que foi dado o start
    if (tsegs > 59) {
      segundos = 0;
      minutos = minutos + 1;
    }
    else {
      segundos = tsegs;
    }
    if (minutos == 60){ 
      horas = horas + 1;
      minutos = 0;
      segundos = 0;
    } */
    
    if (tsegs > 3599) { // já se passaram horas
      horas = int(tsegs) / 3600;
      minutos = (int(tsegs) % 3600) / 60;
      segundos = (minutos % 60);
      linha4 = String(horas) + ":" + String(minutos) + ":" + String(segundos);
      //imprime(linha0,linha1,linha2,linha3,linha4,linha5);
    }
    else if (tsegs > 59) { // já se passaram minutos
      minutos = int(tsegs) / 60;
      segundos = (int(tsegs) % 60);
      linha4 = String(horas) + ":" + String(minutos) + ":" + String(segundos);
      //imprime(linha0,linha1,linha2,linha3,linha4,linha5);
    }
    else { // já se passaram segundos
      linha4 = String(horas) + ":" + String(minutos) + ":" + String(tsegs);
      //imprime(linha0,linha1,linha2,linha3,linha4,linha5);
    }
    linha2 = "Aperte OK para parar a contagem";
    if (hora != " ") {
      hora = ntp.getFormattedTime();//Armazena na váriavel HORA, o horario atual.
      linha0 = hora;
    }
    //linha4 = String(horas) + ":" + String(minutos) + ":" + String(segundos);
    if (digitalRead(confirma) == HIGH) {
      Serial.println("confirma apertado");
      delay(170); // espera para cada apertada ser vista individualmente 
      linha2 = "Aperte OK para nova contagem";
      
      if (digitalRead(confirma) == HIGH) {
        Serial.println("confirma apertado");
        delay(170); // espera para cada apertada ser vista individualmente 
        comecou = 0; // parou a contagem
        segundos = 0;
        minutos = 0;
        horas = 0;
      }
    }
  }
}





void aplicativo1() {
  // arquivos para quaso
  // Os arquivos devem ser colocados na pasta do código numa pasta chamada 'data' 
  // Os nomes dos arquivos não devem conter espaços
  // Recomenda-se que os arquivos tenham no máximo 100Kb, ou seja, é para imagens, textos, jogos flash e não videos ou imagens de CD.
  // os arquivos podem ser colocados na memória interna com a ferramenta 'ESP8266 Sketch Data Upload'
  // mais detalhes em: www.filipeflop.com/blog/esp8266-gravando-dados-memoria-flash/
  tela = 1;
  String texto;
  while (tela == 1) {
    linha0 = "Arquivos:";
    linha1 = " ";    
    linha2 = " ";   
    // como listar os arquivos
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
      String arquivo = dir.fileName();
      linha2 += "<a href='" + String(arquivo) + "'>" + String(arquivo) + "</a>" ;
      linha2 += "<br />";
      //delay(500);
      if (arquivo.endsWith(".txt")) {
        //display.drawString(0, 0, "-" + String(arquivo)");
        File f = SPIFFS.open(arquivo, "r");                    // Open the file
        for(i=0;i<f.size();i++) { //Read upto complete file size
          //linha 0
          String line = f.readStringUntil('\n');
          if (line.length() >= 24) {
            // corta para caber numa linha deixa o resto para as linhas ou telas seguintes. como??
          }
          display.drawString(0,0,line);
          display.display();
          //linha 1
          line = f.readStringUntil('\n');
          display.drawString(0,13,line);
          display.display();
          //linha 2
          line = f.readStringUntil('\n');
          display.drawString(0,23,line);
          display.display();
          //linha 3
          line = f.readStringUntil('\n');
          display.drawString(0,33,line);
          display.display();
          //linha 4
          line = f.readStringUntil('\n');
          display.drawString(0,43,line);
          display.display();
          //linha 5
          line = f.readStringUntil('\n');
          display.drawString(0,53,line);
          display.display();
          delay(8000);
          display.clear();

    if (digitalRead(esquerdo) == HIGH) {
      //Serial.println("esquerdo apertado");
      //delay(170); // espera para cada apertada ser vista individualmente 
      tela = 0; // voltar pelo menu
      
        /*

          display.clear();
          //display.end();
          delay(500);
          //linha 0
          for(j=0;j<24;j++) {
            texto = texto + (char)file.read();
          }
          display.drawString(0,0,texto);
          display.display();
          texto = "";
          delay(100);
          //linha 1
          for(j=0;j<24;j++) {
            texto = texto + (char)file.read();
          }
          display.drawString(0,13,texto);
          display.display();
          texto = "";
          delay(100);
          //linha 2
          for(j=0;j<24;j++) {
            texto = texto + (char)file.read();
          }
          display.drawString(0,23,texto);
          display.display();
          texto = "";
          delay(100);
          //linha 3
          for(j=0;j<24;j++) {
            texto = texto + (char)file.read();
          }
          display.drawString(0,33,texto);
          display.display();
          texto = "";
          delay(100);
          //linha 4
          for(j=0;j<24;j++) {
            texto = texto + (char)file.read();
          }
          display.drawString(0,43,texto);
          display.display();
          texto = "";
          delay(100);
          //linha 5
          for(j=0;j<24;j++) {
            texto = texto + (char)file.read();
          }
          display.drawString(0,53,texto);
          display.display();
          texto = "";
          */
          //}
        }
        
        //display.drawString(0, 0, String(arquivo));
        f.close();      
        
      }

    }
    linha3 = " ";
    linha4 = " ";
    linha5 = " ";
    server.handleClient();
    //PraTela();
    
    
    display.display();
    if (digitalRead(esquerdo) == HIGH) {
      //Serial.println("esquerdo apertado");
      //delay(170); // espera para cada apertada ser vista individualmente 
      tela = 0; // voltar pelo menu
    }
  }  
}
}



// --------------------------------- Setup ----------------------------------------------------

void setup() {

  // setup para o suporte ao display
  // Initialising the UI will init the display too.
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  randomSeed(analogRead(0)); // seed para o shift do display
  display.drawString(0, 0, "QuaSO versão 0.09");
  
  hora = " ";
  
  // definições botões
  pinMode(esquerdo, INPUT);
  pinMode(direito, INPUT);
  pinMode(confirma, INPUT);
  pinMode(IRrec, INPUT);
  //pinMode(reedSwitch, INPUT);
  pinMode(LedUV, OUTPUT);

// Scan, Sort, and Connect to WiFi   Obrigado para Leatherneck Garage
// Link: https://www.instructables.com/id/Connect-an-ESP8266-to-Any-Open-WiFi-Network-Automa/
  Serial.begin(SERIAL_BAUD);

  Serial.println("Scanning for open networks...");
   if(WiFi.status() != WL_CONNECTED) {
    /* Clear previous modes. */
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    //WiFi.mode(WIFI_STA);
    WiFi.mode(WIFI_AP_STA); // tentando ativar modo AP + cliente
    delay(WIFI_DELAY);
    WiFi.softAP("ESP8266 2342"); // tentando criar o AP
    /* Scan for networks to find open guy. */
    scanAndSort();
    delay(WIFI_DELAY);
    /* Global ssid param need to be filled to connect. */
    if(strlen(ssid) > 0) {
      Serial.print("Connecting to ");
      Serial.println(ssid);
      /* No pass for WiFi. We are looking for non-encrypteds. */
      WiFi.begin(ssid);
      unsigned short try_cnt = 0;
      /* Wait until WiFi connection but do not exceed MAX_CONNECT_TIME */
      while (WiFi.status() != WL_CONNECTED && try_cnt < MAX_CONNECT_TIME / WIFI_DELAY) {
        delay(WIFI_DELAY);
        Serial.print(".");
        try_cnt++;
      }
      if(WiFi.status() == WL_CONNECTED) {
        Serial.println("");
        Serial.println("Connection Successful!");
        Serial.println("Your device IP address is ");
        Serial.println(WiFi.localIP());
        display.drawString(0, 0, "Conectado por WiFi!");
      } else {
        Serial.println("Connection FAILED");
      }
    } else {
      Serial.println("No open networks available. :-(");  
      display.drawString(0, 0, "Sem rede de WiFi aberta.");
    }
  }

  // pega a hora da Internet:
  if (WiFi.status() == WL_CONNECTED){
   ntp.begin();//Inicia o NTP.
   ntp.forceUpdate();//Força o Update.
  }

  // setup para o webservidor
  server.on("/", handleRoot);
  //server.onNotFound(handleNotFound);
  server.begin(); //INICIA O SERVIDOR PARA RECEBER DADOS NA PORTA DEFINIDA EM "WiFiServer server(porta);"
  Serial.println("Servidor iniciado"); //ESCREVE O TEXTO NA SERIAL
  Serial.print("IP para se conectar: "); //ESCREVE O TEXTO NA SERIAL
  Serial.print("http://"); //ESCREVE O TEXTO NA SERIAL
  Serial.println(WiFi.localIP()); //ESCREVE NA SERIAL O IP RECEBIDO DENTRO DA REDE SEM FIO (O IP NESSA PRÁTICA É RECEBIDO DE FORMA AUTOMÁTICA)


  // app termometro e Resumo
  dht.setup(D3, DHTesp::DHT11);

  SPIFFS.begin();        // Start the SPI Flash Files System

// para enviar o arquivo solicitado ou mensagem de não encontrado
  server.onNotFound([]() {                              // If the client requests any URI
    if (!handleFileRead(server.uri()))                  // send it if it exists
      server.send(404, "text/plain", "404: Não encontrado"); // otherwise, respond with a 404 (Not Found) error
  });

}






// --------------------------------- Loop principal ----------------------------------------------------

void loop ()  {

  server.handleClient();
  PraTela(); // Função que manda o texto para o display
  
   hora = ntp.getFormattedTime();//Armazena na váriavel HORA, o horario atual.
   //Serial.println("Hora:");
   //Serial.println(hora);//Printa a hora já formatada no monitor. 

  // teste botões
  /* if (digitalRead(esquerdo) == HIGH) {
    Serial.println("esquerdo apertado");
    delay(170); // espera para cada apertada ser vista individualmente 
  }
  if (digitalRead(direito) == HIGH) {
    Serial.println("direito apertado");
    delay(330);
  }
  if (digitalRead(confirma) == HIGH) {
    Serial.println("confirma apertado");
  }  */
  delay(150); // espera para cada apertada ser vista individualmente 

  if (tela == 0 && app == 1) { // tela inicial acabou de ligar.
    if (hora != " ") {
      linha0 = hora;
    }
    else {
      linha0 = "QuaSO versão 0.09";
    }
    linha1 = "(X) Arquivos";
    linha2 = "( ) Cronometro";
    linha3 = "( ) Resumo";
    linha4 = "( ) Luz Negra";
    linha5 = "( ) Wi-Fi Seguro";
    
    if (digitalRead(esquerdo) == HIGH) {
      Serial.println("esquerdo apertado");
      delay(170); // espera para cada apertada ser vista individualmente 
      app = 5;
    }
    if (digitalRead(direito) == HIGH) {
      Serial.println("direito apertado");
      delay(170); // espera para cada apertada ser vista individualmente 
      app = 2;
    }
    if (digitalRead(confirma) == HIGH) {
      aplicativo1(); // vai para o app1
      delay(170);
    }
  }
  
  if (tela == 0 && app == 2) { // tela inicial acabou de ligar.
    if (hora != " ") {
      linha0 = hora;
    }
    else {
      linha0 = "QuaSO versão 0.09";
    }
    linha1 = "( ) Arquivos";
    linha2 = "(X) Cronometro";
    linha3 = "( ) Resumo";
    linha4 = "( ) Luz Negra";
    linha5 = "( ) Wi-Fi Seguro";
    //client.stop(); // fecha a conexão anterior para carregar a nova página
    //imprime(linha0,linha1,linha2,linha3,linha4,linha5);
    
    if (digitalRead(esquerdo) == HIGH) {
      //Serial.println("esquerdo apertado");
      delay(170); // espera para cada apertada ser vista individualmente 
      app = 1;
    }
    if (digitalRead(direito) == HIGH) {
      //Serial.println("direito apertado");
      delay(170); // espera para cada apertada ser vista individualmente 
      app = 3;
    }
    if (digitalRead(confirma) == HIGH) {
      delay(170); // espera para cada apertada ser vista individualmente 
      aplicativo2(); // vai para o cronometro
    }
  }
  
  if (tela == 0 && app == 3) { // Menu item 3
    //linha0 = WiFi.localIP();
    linha1 = "( ) Arquivos";
    linha2 = "( ) Cronometro";
    linha3 = "(X) Resumo";
    linha4 = "( ) Luz Negra";
    linha5 = "( ) Wi-Fi Seguro";
    //client.stop(); // fecha a conexão anterior para carregar a nova página
    //imprime(linha0,linha1,linha2,linha3,linha4,linha5);
    
    if (digitalRead(esquerdo) == HIGH) {
      Serial.println("esquerdo apertado");
      delay(170); // espera para cada apertada ser vista individualmente 
      app = 2;
    }
    if (digitalRead(direito) == HIGH) {
      Serial.println("direito apertado");
      delay(170); // espera para cada apertada ser vista individualmente 
      app = 4;
    }
    if (digitalRead(confirma) == HIGH) {
      delay(170); // espera para cada apertada ser vista individualmente 
      aplicativo3(); // vai para o 3
    } 
  }
  
if (tela == 0 && app == 4) { // Menu item 4
    //linha0 = WiFi.localIP();
    linha1 = "( ) Arquivos";
    linha2 = "( ) Cronometro";
    linha3 = "( ) Resumo";
    linha4 = "(X) Luz Negra";
    linha5 = "( ) Wi-Fi Seguro";
    //client.stop(); // fecha a conexão anterior para carregar a nova página
    //imprime(linha0,linha1,linha2,linha3,linha4,linha5);
    
    if (digitalRead(esquerdo) == HIGH) {
      Serial.println("esquerdo apertado");
      delay(170); // espera para cada apertada ser vista individualmente 
      app = 3;
    }
    if (digitalRead(direito) == HIGH) {
      Serial.println("direito apertado");
      delay(170); // espera para cada apertada ser vista individualmente 
      app = 5;
    }
    if (digitalRead(confirma) == HIGH) {
      delay(170); // espera para cada apertada ser vista individualmente 
      aplicativo4(); // vai para o 4
    } 
  }

if (tela == 0 && app == 5) { // Menu item 4
    //linha0 = WiFi.localIP();
    linha1 = "( ) Arquivos";
    linha2 = "( ) Cronometro";
    linha3 = "( ) Resumo";
    linha4 = "( ) Luz Negra";
    linha5 = "(X) Wi-Fi Seguro";
    //client.stop(); // fecha a conexão anterior para carregar a nova página
    //imprime(linha0,linha1,linha2,linha3,linha4,linha5);
    
    if (digitalRead(esquerdo) == HIGH) {
      Serial.println("esquerdo apertado");
      delay(170); // espera para cada apertada ser vista individualmente 
      app = 4;
    }
    if (digitalRead(direito) == HIGH) {
      Serial.println("direito apertado");
      delay(170); // espera para cada apertada ser vista individualmente 
      app = 1;
    }
    if (digitalRead(confirma) == HIGH) {
      delay(170); // espera para cada apertada ser vista individualmente 
      aplicativo5(); // vai para o 5
    } 
  }


  
}


// Para abrir arquivos da memória interna
// adaptado de https://tttapa.github.io/ESP8266/Chap11%20-%20SPIFFS.html
String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  else if (filename.endsWith(".swf")) return "application/x-shockwave-flash";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";  
  else if (filename.endsWith(".jpg")) return "image/jpeg";  
  return "text/plain";
}
bool handleFileRead(String path){  // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  //if(path.endsWith("/")) path += "index.html";           // If a folder is requested, send the index file
  String contentType = getContentType(path);             // Get the MIME type
  String pathWithGz = path + ".gz";
  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){  // If the file exists, either as a compressed archive, or normal
    if(SPIFFS.exists(pathWithGz))                          // If there's a compressed version available
      path += ".gz";                                         // Use the compressed version
    File file = SPIFFS.open(path, "r");                    // Open the file
    size_t sent = server.streamFile(file, contentType);    // Send it to the client
    file.close();                                          // Close the file again
    Serial.println(String("\tSent file: ") + path);
    return true;
  }
  Serial.println(String("\tFile Not Found: ") + path);
  return false;                                          // If the file doesn't exist, return false
}






// Quaso 0.12
// Escrito por Adrian Rupp 2019.
//O sketch usa 343300 bytes (32%) de espaço de armazenamento para programas. O máximo são 1044464 bytes.
//Variáveis globais usam 29796 bytes (36%) de memória dinâmica, deixando 52124 bytes para variáveis locais. O máximo são 81920 bytes.
