// ------------------------------------------------------------
// Área para inclusão de bibliotecas e definições necessárias -
// ------------------------------------------------------------



// -------------------------------------------
// Área para inclusão de definições de setup -
// -------------------------------------------
void setup() { 

// app timer
  byte start = 0;
  int horas = 0;
  int minutos = 0;
  int segundos = 0;
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
      if horas == 0 {
      horas = 99;}
      else {
      horas = horas - 1;
      display.drawString(rnd1,30, string(horas) + " horas");
      
    if ((digitalRead(botao2) == HIGH) && (tela == 1)) { // botão direito foi apertado
      if horas == 99 {
      horas = 0;}
      else {
      horas = horas + 1;
      display.drawString(rnd1,30, string(horas) + " horas");
  
     if ((digitalRead(botao3) == HIGH) && (tela == 1) && (tela == 1) ) { // botão Ok foi apertado
      display.drawString(rnd1,20, string(horas) + " horas"); 
  
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
