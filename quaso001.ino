// Programa: QuaSO para Wemos D1 mini e Display OLED 128x64
// Autor: Adrian Rupp

//Lista de bibliotecas incluidas
#include <Wire.h>  // Necessária para usar o display
#include "SSD1306Wire.h"  // Necessária para usar o display

#include "DHTesp.h" // Necessária para usar o módulo de temperatura e humidade (app 1)
DHTesp dht;

// Configuração do display
SSD1306Wire display(0x3c, SDA, SCL);  // endereço, SDA, SCL
// SSD1306Wire display(0x3c, SDA, SCL, GEOMETRY_128_32);  // endereço, SDA, SCL, para displays com 128x32.

// Initialize the OLED display using SPI:
// D5 -> CLK
// D7 -> MOSI (DOUT)
// D0 -> RES
// D2 -> DC
// D8 -> CS
// SSD1306Spi display(D0, D2, D8);  // RES, DC, CS
// or
// SH1106Spi display(D0, D2);       // RES, DC


// Configuração das portas
byte botao1 = 0; // pino do botão esquerdo
byte botao2 = 3; // pino do botão direito
byte botao3 = 8; // pino do botão de OK
byte app = 1; // variável que quarda qual app está selecionado.
byte tela = 0; // variável que quarda qual tela está ativa. Tela = 0 é a tela de escolha de app 
byte napp = 3; // variável que quarda quantos app estão incluidos, mínimo aceito = 3
byte rnd1; // variável da posição do menu para ela variar e evitar o efeito burn in do oled



// Definição de quais sensores estão instalados
bool DHT11 = false;




void setup() {  // Configurações 
  display.init();  // inicializando o display
  randomSeed(analogRead(0)); // semente para a criação de números randonômicos 
  pinMode(botao1, INPUT);
  pinMode(botao2, INPUT);
  pinMode(botao3, INPUT);

  // app1 
  dht.setup(D5, DHTesp::DHT11);
  float hmax = 0;
  float hmin = 100;
  float tmax = 0;
  float tmin = 100;
}


// insira aqui o código do seu app 

void Aplicativo1() { 
  // termometro para Quaso
  tela = 1;
  display.clear(); // clear the display
  float h = dht.getHumidity(); // pega a umidade
  float t = dht.getTemperature(); // pega a temperatura
  display.setFont(ArialMT_Plain_10); // definição da fonte: pequena
  display.drawString(rnd1,10,"Umidade Relativa: " + String(h));
  hmax = max(hmax,h);
  hmin = min(hmin,h);
  display.drawString(rnd1,20,"Máxima: " + String(hmax) + "  Mínima: " + String(hmin));
  display.drawString(rnd1,30,"Temperatura: " + String(t));
  tmax = max(tmax,h);
  tmin = min(tmin,h);
  display.drawString(rnd1,40,"Máxima: " + String(tmax) + "  Mínima: " + String(tmin));
  
  delay(2000);
}


void Aplicativo2() {
  
}


void Aplicativo3() {
  
}







void loop() {  // Código principal que sempre repete
  // 128 em caracteres de 5: 123456789012  123456789012 123456789012 123456789012 123456789012 
  display.setFont(ArialMT_Plain_10); // definição da fonte: pequena
  rnd1 = random(10); // Definição aleatória de posição inicial para evitar efeito burn in
  if (tela = 0) {
    display.drawString(rnd1,0,"QuaSO v0.1");  // 
    display.drawString(rnd1,10,"- Escolha a função:");
    if (app == 1) {
      display.drawString(rnd1,20,"(X) Termômetro");
      display.drawString(rnd1,30,"( ) Trena Eletrônica");
      display.drawString(rnd1,40,"( ) Game Pong");
    }
    if (app == 2) {
      display.drawString(rnd1,20,"( ) Termômetro");
      display.drawString(rnd1,30,"(X) Trena Eletrônica");
      display.drawString(rnd1,40,"( ) Game Pong");
    }
    if (app == 3) {
      display.drawString(rnd1,20,"( ) Termômetro");
      display.drawString(rnd1,30,"( ) Trena Eletrônica");
      display.drawString(rnd1,40,"(X) Game Pong");
    }
  }



  // Definição do app selecionado botão esquerdo
  if ((digitalRead(botao1) == HIGH) && (tela == 0)) { // Checa se o botão esquerdo foi apertado enquanto está na escolha de app
    if (app == 1) {app = napp;}
    if (app == 2) {app = 1;}
    if (app == 3) {app = 2;}
  }

  // Definição do app selecionado botão direito
  if ((digitalRead(botao1) == HIGH) && (tela == 0)) { // Checa se o botão esquerdo foi apertado enquanto está na escolha de app
    if (app == 1) {app = 2;}
    if (app == 2) {app = 3;}
    if (app == 3) {app = 1;}
  }



// Inicia o app selecionado
  if ((digitalRead(botao3) == HIGH) && (app == 1) && (tela == 0)) { // confere que: apertaram o botão de confirma, tá selecionado o app 1 e tá na tela de escolha de app
    Aplicativo1;
  }

  if ((digitalRead(botao3) == HIGH) && (app == 2) && (tela == 0)) { // confere que: apertaram o botão de confirma, tá selecionado o app 1 e tá na tela de escolha de app
    Aplicativo2;
  }

  if ((digitalRead(botao3) == HIGH) && (app == 3) && (tela == 0)) { // confere que: apertaram o botão de confirma, tá selecionado o app 1 e tá na tela de escolha de app
    Aplicativo3;
  }


  delay(1000); 
}

//O sketch usa 271784 bytes (26%) de espaço de armazenamento para programas. O máximo são 1044464 bytes.
//Variáveis globais usam 27436 bytes (33%) de memória dinâmica, deixando 54484 bytes para variáveis locais. O máximo são 81920 bytes.
