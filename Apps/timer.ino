// ------------------------------------------------------------
// Área para inclusão de bibliotecas e definições necessárias -
// ------------------------------------------------------------



// -------------------------------------------
// Área para inclusão de definições de setup -
// -------------------------------------------
void setup() { 

// app timer
  byte start = 0;
  int horas = -1;
  int minutos = -1;
  int segundos = -1;
}


// -------------------------------------------
// Código do programa que normalmente ficaria no 'void loop()' (mas no QuaSO fica antes)
// -------------------------------------------
void Aplicativo1() { // troque por um número de aplicativo livre como 'Aplicativo2()'
  // timer para Quaso
  tela = 1;
  if (start == 0) {
    display.drawString(rnd1,0,"Timer" );
    display.drawString(rnd1,20,"Definir número de horas:" );
    if ((digitalRead(botao1) == HIGH) && (tela == 1)) { // botão esquerdo foi apertado
      if horas == 0 { horas = 99;}
      else { horas = horas - 1; }
      display.drawString(rnd1,30, string(horas) + " horas");
    }
    if ((digitalRead(botao2) == HIGH) && (tela == 1)) { // botão direito foi apertado
      if horas == 99 { horas = 0;}
      else { horas = horas + 1; }
      display.drawString(rnd1,30, string(horas) + " horas");
    }
    if ((digitalRead(botao3) == HIGH) && (tela == 1) && (horas != -1) ) { // botão Ok foi apertado
      display.drawString(rnd1,20, "Definir número de minutos:"); 
      if ((digitalRead(botao1) == HIGH) && (tela == 1)) { // botão esquerdo foi apertado
        if minutos == 0 { minutos = 59;}
        else { minutos = minutos - 1; }
        display.drawString(rnd1,30, string(horas) + " horas");
      }
      if ((digitalRead(botao2) == HIGH) && (tela == 1)) { // botão direito foi apertado
        if minutos == 59 { minutos = 0;}
        else { minutos = minutos + 1; }
        display.drawString(rnd1,30, string(minutos) + " minutos"); 
      }
     if ((digitalRead(botao3) == HIGH) && (tela == 1) && (minutos != -1) ) { // botão Ok foi apertado
      display.drawString(rnd1,20, "Definir número de segundos:"); 
      if ((digitalRead(botao2) == HIGH) && (tela == 1)) { // botão direito foi apertado
        if segundos == 59 { segundos = 0; }
        else { segundos = segundos + 1; }
        display.drawString(rnd1,30, string(segundos) + " segundos"); 
      }
    }
    if ((digitalRead(botao3) == HIGH) && (tela == 1) && (segundos != -1) ) { // botão Ok foi apertado
      display.drawString(rnd1,20, string(horas) + ":" + string(minutos) + ":" + string(segundos)); 
      display.drawString(rnd1,30, "Clique Ok para iniciar a contagem");
      delay(600);
      If ((digitalRead(botao3) == HIGH) { start = 1; }
    }
  }
          
          
      long inicio = millis() / 1000; // quantos segundos passaram desde que iniciou o sistema
      byte start = 1; }
  }
  else if (start = 1) { // se já começou a contagem
    long tsegs = (millis()/1000) - inicio; // segundos desde que foi dado o start
    if (tsegs > 3600) { // já se passaram horas
      horas = int(tsegs) / 3600;
      minutos = (int(tsegs) % 3600) / 60;
      segundos = (minutos % 60);
      display.drawString(rnd1,30,"String(horas) + ":" + String(minutos) + ":" + String(segundos));
    }
    else if ((tsegs > 60) { // já se passaram minutos
      minutos = int(tsegs) / 60;
      segundos = (minutos % 60);
      display.drawString(rnd1,30,"String(horas) + ":" + String(minutos) + ":" + String(segundos));
    }
    else {                      // já se passaram segundos
      display.drawString(rnd1,30,"String(horas) + ":" + String(minutos) + ":" + String(tsegs));
    }
    display.drawString(rnd1,40,"Aperte OK para parar a contagem";
    if ((digitalRead(botao3) == HIGH) {
      start = 0; // parou a contagem
    }
  }
  delay(400);
}
