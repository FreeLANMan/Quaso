// ------------------------------------------------------------
// Área para inclusão de bibliotecas e definições necessárias -
// ------------------------------------------------------------

// definições app termometro
#include "DHTesp.h" // Necessária para usar o módulo de temperatura e humidade (app 1)
DHTesp dht;
int kelvin = 0;
float hmax = 0;
float hmin = 100;
float tmax = 0;
float tmin = 100;



// -------------------------------------------
// Área para inclusão de definições de setup -
// -------------------------------------------
void setup() { 
  
  //setup - DHT11
  dht.setup(D3, DHTesp::DHT11);
  
}


// -------------------------------------------
// Código do programa que normalmente fica no 'void loop()' -
// -------------------------------------------
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
    
    if (digitalRead(esquerdo) == HIGH) {
      //Serial.println("esquerdo apertado");
      //delay(170); // espera para cada apertada ser vista individualmente 
      tela = 0; // voltar pelo menu
    }
    delay(10000); // tempo de 10s para resposta do sensor
  }  
}


// Arrumar também as entradas no menu que está no loop principal.
