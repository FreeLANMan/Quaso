
// versão 01: Revisão e publicação on-line

// -------------------------------- Definições e bibliotecas -----------------------------------------


// Bibliotecas incluidas:
#include <ESP8266WiFi.h> // #include WiFi Library
#include <NTPClient.h>//Biblioteca do NTP. https://github.com/arduino-libraries/NTPClient
#include <WiFiUdp.h>//Biblioteca do UDP. 
#include <ESP8266WebServer.h> // biblioteca do servidor mais sofisticado
#include <FS.h>   // Include the SPIFFS library

// Definições para o sistema de conexão automática ao wifi
#define SERIAL_BAUD       9600 /* Serial Baud Rate */
#define WIFI_DELAY        500 /* Delay paramter for connection. */
#define MAX_SSID_LEN      32 /* Max SSID octets. */
#define MAX_CONNECT_TIME  30000 /* Wait this much until device gets IP. */
char ssid[MAX_SSID_LEN] = ""; /* SSID that to be stored to connect. */

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

byte tela = 0; // guarda em que tela está
byte app = 1; // guarda o app selecionado
byte aplicativo = 0; // guarda o aplicativo escolhido
String linha0 = " ";
String linha1 = " ";
String linha2 = " ";
String linha3 = " ";
String linha4 = " ";
String linha5 = " ";

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





// --------------------------------- Aplicativos ----------------------------------------------------

void aplicativo3() { 
  // termometro para Quaso
  tela = 1;
  while (tela == 1) {
    server.handleClient();
    float h = dht.getHumidity(); // pega a umidade
    float t = dht.getTemperature(); // pega a temperatura
    kelvin = t + 273,15;
    hmax = max(hmax,h);
    hmin = min(hmin,h);
    //display.drawString(rnd1,20,"Máxima: " + String(hmax) + "  Mínima: " + String(hmin));
    //display.drawString(rnd1,30,"Temperatura: " + String(t));
    tmax = max(tmax,t);
    tmin = min(tmin,t);
  
    // montando o que vai ser apresentado na tela:
    linha0 = "Temperatura:";
    linha1 = String(t) + "ºC, " + String(kelvin) + "K";
    linha2 = "min.: " + String(tmin) + "ºC, máx.: " + String(tmax) + "ºC";
    linha3 = "Umidade:" + String(h) + "%";
    linha4 = "min.: " + String(hmin) + "%, máx.: " + String(hmax) + "%";
    if (h < 60) {
      linha5 = "Nível não adequado para a saúde";
    }
    else if (h < 30 && h > 21) {
      linha5 = "Estado de Atenção!";
    }
    else if (h < 20 && h > 12) {
      linha5 = "Estado de Alerta!";
    }
    else if (h < 12) {
      linha5 = "Estado de Emergência!";
    }
    else {
      linha5 = "10s para nova leitura";
    }
    delay(5000); // tempo de 10s para resposta do sensor
    if (digitalRead(esquerdo) == HIGH) {
      //Serial.println("esquerdo apertado");
      //delay(170); // espera para cada apertada ser vista individualmente 
      tela = 0; // voltar pelo menu
    }
    delay(5000); // tempo de 10s para resposta do sensor
  }  
}




void aplicativo2() {
  // cronometro para Quaso
  tela = 1;
  while (comecou == 0) {
    server.handleClient();
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
  linha2 = " ";
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
    }
    linha3 = " ";
    linha4 = " ";
    server.handleClient();
    if (digitalRead(esquerdo) == HIGH) {
      //Serial.println("esquerdo apertado");
      //delay(170); // espera para cada apertada ser vista individualmente 
      tela = 0; // voltar pelo menu
    }
  }  
}




// --------------------------------- Setup ----------------------------------------------------

void setup() {
  hora = " ";

  // definições botões
  pinMode(esquerdo, INPUT);
  pinMode(direito, INPUT);
  pinMode(confirma, INPUT);

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
      } else {
        Serial.println("Connection FAILED");
      }
    } else {
      Serial.println("No open networks available. :-(");  
    }
  }

  // pega a hora da Internet:
   ntp.begin();//Inicia o NTP.
   ntp.forceUpdate();//Força o Update.

  // setup para o webservidor
  server.on("/", handleRoot);
  //server.onNotFound(handleNotFound);
  server.begin(); //INICIA O SERVIDOR PARA RECEBER DADOS NA PORTA DEFINIDA EM "WiFiServer server(porta);"
  Serial.println("Servidor iniciado"); //ESCREVE O TEXTO NA SERIAL
  Serial.print("IP para se conectar: "); //ESCREVE O TEXTO NA SERIAL
  Serial.print("http://"); //ESCREVE O TEXTO NA SERIAL
  Serial.println(WiFi.localIP()); //ESCREVE NA SERIAL O IP RECEBIDO DENTRO DA REDE SEM FIO (O IP NESSA PRÁTICA É RECEBIDO DE FORMA AUTOMÁTICA)

  // app3
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
    linha1 = "- Escolha a função:";
    linha2 = "(X) Arquivos";
    linha3 = "( ) Cronometro";
    linha4 = "( ) Termometro";
    linha5 = " ";
    
    if (digitalRead(esquerdo) == HIGH) {
      Serial.println("esquerdo apertado");
      delay(170); // espera para cada apertada ser vista individualmente 
      app = 3;
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
    linha1 = "- Escolha a função:";
    linha2 = "( ) Arquivos";
    linha3 = "(X) Cronometro";
    linha4 = "( ) Termometro";
    linha5 = " ";
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
    linha1 = "- Escolha a função:";
    linha2 = "( ) Arquivos";
    linha3 = "( ) Cronometro";
    linha4 = "(X) Termometro";
    linha5 = " ";
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
      app = 1;
    }
    if (digitalRead(confirma) == HIGH) {
      delay(170); // espera para cada apertada ser vista individualmente 
      aplicativo3(); // vai para o 3
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






// Quaso 0.1
// Escrito por Adrian Rupp 2019.
//O sketch usa 330128 bytes (31%) de espaço de armazenamento para programas. O máximo são 1044464 bytes.
//Variáveis globais usam 28968 bytes (35%) de memória dinâmica, deixando 52952 bytes para variáveis locais. O máximo são 81920 bytes.
