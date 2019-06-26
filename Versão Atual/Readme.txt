Versão atual para baixar: 0.1

Aplicativos:
- Arquivos (acesso aos arquivos da memória interna pelo Browser)
- Cronômetro (um app exemplo de um crônometro)
- Termômetro (acesso aos dados de um DHT11: temperatura, umidade, máximas de mínimas desde que o aparelho foi ligado.

Observações:
- Este programa está em fase beta.

- você precisa criar uma pasta chamada 'data' dentro da pasta em que vai o .ino onde coloca os arquivos que vão para a memória interna. Recomendo usar arquivos com no máximo 100Kb. 
Mais informações sobre como usar a memória interna em: www.filipeflop.com/blog/esp8266-gravando-dados-memoria-flash

Pinagem:
- Wemos: 
D0 -> botão esquerdo
D8 -> botão direito
D5 -> botão confirma
D3 -> entrada de dados do sensor DHT-11
5V -> a energia para alimentar os botões.
3V3 -> energia para o sensor DHT-11.
G -> o terra para o sensor e os botões.
- Botões: foram usados os de 4 terminais: 1 terminal é o de sinal de entrada para o Wemos, 1 é o ground (terra) e o outro é o 5V. É usado um resistor antes da ligação com o terra por precaução.
- sensor DHT-11: ligação GND (ground), VCC (corrente de 3.3V) e DATA (dados, ligado ao pino D3 do Wemos).
A imagem do 1º protótipo está aqui: 
https://github.com/FreeLANMan/Quaso/blob/master/Primeiro%20prot%C3%B3tipo.jpeg
