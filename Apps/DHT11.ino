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
}


// -------------------------------------------
// Código do programa que normalmente fica no 'void loop()' -
// -------------------------------------------
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


