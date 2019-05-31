
// versão 004: teste dos botões e menu inicial

#include <ESP8266WiFi.h> // #include WiFi Library
#include <NTPClient.h>//Biblioteca do NTP. https://github.com/arduino-libraries/NTPClient
#include <WiFiUdp.h>//Biblioteca do UDP. 

// Definições para o sistema de conexão automática ao wifi
#define SERIAL_BAUD       9600 /* Serial Baud Rate */
#define WIFI_DELAY        500 /* Delay paramter for connection. */
#define MAX_SSID_LEN      32 /* Max SSID octets. */
#define MAX_CONNECT_TIME  30000 /* Wait this much until device gets IP. */
char ssid[MAX_SSID_LEN] = ""; /* SSID that to be stored to connect. */

// Definições para pegar a hora da Internet
WiFiUDP udp;//Cria um objeto "UDP".
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);//Cria um objeto "NTP" com as configurações.
String hora; //Váriavel que armazenara o horario do NTP.

// definições para o webserver
WiFiServer server(80); //CASO OCORRA PROBLEMAS COM A PORTA 80, UTILIZE OUTRA (EX:8082,8089) E A CHAMADA DA URL FICARÁ IP:PORTA(EX: 192.168.0.15:8082)

// definições botões adaptado de https://community.wia.io/d/49-wemos-d1-mini-esp8266-smart-iot-button-with-messagebird-sms-and-voice-messages
byte esquerdo = D0;
byte direito = D8;
byte confirma = D5;


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

void setup() {

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
    WiFi.mode(WIFI_STA);
    delay(WIFI_DELAY);
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
  server.begin(); //INICIA O SERVIDOR PARA RECEBER DADOS NA PORTA DEFINIDA EM "WiFiServer server(porta);"
  Serial.println("Servidor iniciado"); //ESCREVE O TEXTO NA SERIAL
  Serial.print("IP para se conectar: "); //ESCREVE O TEXTO NA SERIAL
  Serial.print("http://"); //ESCREVE O TEXTO NA SERIAL
  Serial.println(WiFi.localIP()); //ESCREVE NA SERIAL O IP RECEBIDO DENTRO DA REDE SEM FIO (O IP NESSA PRÁTICA É RECEBIDO DE FORMA AUTOMÁTICA)






  
}

void loop ()  {
   hora = ntp.getFormattedTime();//Armazena na váriavel HORA, o horario atual.
   //Serial.println("Hora:");
   //Serial.println(hora);//Printa a hora já formatada no monitor. 

  // teste botões
  //int a = digitalRead(esquerdo);
  //Serial.println("esquerdo:");
  //Serial.print(a);
  //if (digitalRead(esquerdo) == LOW){ //high indica apertado depois muda para não aperado e fica nisso
  //  Serial.println("esquerdo apertado");
  //}
  //if (digitalRead(direito) == LOW){ //low indica apertado sempre
  //  Serial.println("direito apertado");
  //}
  //if (digitalRead(confirma) == LOW){ //high indica apertado sempre //low indica não apertado sempre
  //  Serial.println("confirma apertado");
  //}


  if (digitalRead(esquerdo) == HIGH) {
    Serial.println("esquerdo apertado");
  }
  if (digitalRead(direito) == HIGH) {
    Serial.println("direito apertado");
  }
  if (digitalRead(confirma) == HIGH) {
    Serial.println("confirma apertado");
  }

  // código do webserver. Adaptado de http://blogmasterwalkershop.com.br/embarcados/nodemcu/nodemcu-como-criar-um-web-server-e-conectar-a-uma-rede-wifi/
  WiFiClient client = server.available(); //VERIFICA SE ALGUM CLIENTE ESTÁ CONECTADO NO SERVIDOR
  if (!client) { //SE NÃO EXISTIR CLIENTE CONECTADO, FAZ
    return; //REEXECUTA O PROCESSO ATÉ QUE ALGUM CLIENTE SE CONECTE AO SERVIDOR
    }
  Serial.println("Novo cliente se conectou!"); //ESCREVE O TEXTO NA SERIAL
  while(!client.available()){ //ENQUANTO CLIENTE ESTIVER CONECTADO
    delay(1); //INTERVALO DE 1 MILISEGUNDO
    }
  String request = client.readStringUntil('\r'); //FAZ A LEITURA DA PRIMEIRA LINHA DA REQUISIÇÃO
  Serial.println(request); //ESCREVE A REQUISIÇÃO NA SERIAL
  client.flush(); //AGUARDA ATÉ QUE TODOS OS DADOS DE SAÍDA SEJAM ENVIADOS AO CLIENTE
  client.println("HTTP/1.1 200 OK"); //ESCREVE PARA O CLIENTE A VERSÃO DO HTTP
  client.println("Content-Type: text/html"); //ESCREVE PARA O CLIENTE O TIPO DE CONTEÚDO(texto/html)
  client.println("");
  client.println("<!DOCTYPE HTML>"); //INFORMA AO NAVEGADOR A ESPECIFICAÇÃO DO HTML
  client.println("<html>"); //ABRE A TAG "html"
  client.println("<h1><center>Ola cliente!</center></h1>"); //ESCREVE "Ola cliente!" NA PÁGINA
  client.println("<center><font size='5'>Seja bem vindo!</center>"); //ESCREVE "Seja bem vindo!" NA PÁGINA
  client.println("</html>"); //FECHA A TAG "html"
  delay(1); //INTERVALO DE 1 MILISEGUNDO
  Serial.println("Cliente desconectado"); //ESCREVE O TEXTO NA SERIAL
  Serial.println(""); //PULA UMA LINHA NA JANELA SERIAL
  

  
}

// Quaso 004
//O sketch usa 279748 bytes (26%) de espaço de armazenamento para programas. O máximo são 1044464 bytes.
//Variáveis globais usam 27568 bytes (33%) de memória dinâmica, deixando 54352 bytes para variáveis locais. O máximo são 81920 bytes.
