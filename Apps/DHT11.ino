// ------------------------------------------------------------
// Área para inclusão de bibliotecas e definições necessárias -
// ------------------------------------------------------------

#include "DHTesp.h" // Necessária para usar o módulo de temperatura e humidade (app 1)
DHTesp dht;




// -------------------------------------------
// Área para inclusão de definições de setup -
// -------------------------------------------
void setup() { 

// app1 
  dht.setup(D5, DHTesp::DHT11);
  float hmax = 0;
  float hmin = 100;
  float tmax = 0;
  float tmin = 100;
  byte vez = 1;
}


// -------------------------------------------
// Código do programa que normalmente fica no 'void loop()' -
// -------------------------------------------
void Aplicativo1() { 
  // termometro para Quaso
  tela = 1;
  if (vez == 1) {
    long inicio = millis() / 1000; // quantos segundos passaram desde que iniciou o sistema
  }
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
  long tsegs = (millis()/1000) - inicio;
  if (tsegs > 3600) {
    display.drawString(rnd1,50,"Tempo ativo: " + String(tsegs) + " horas");
  else if ((tsegs > 60) {
    display.drawString(rnd1,50,"Tempo ativo: " + String(tsegs) + " minutos"); }
  else {
    display.drawString(rnd1,50,"Tempo ativo: " + String(tsegs) + " segundos");}
  }
  
  delay(10000); // tempo de 10s para resposta do sensor
  vez = 2;
}



