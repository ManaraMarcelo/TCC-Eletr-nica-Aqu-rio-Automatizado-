// -----------------------------------------------TCC MKMK ---------------------------------------------------------
// Bibliotecas -----------------------------------------------------------------------------------------------------
#include <Adafruit_GFX.h> // Biblioteca para o controle de cores do display oled
#include <Adafruit_ST7735.h> // Biblioteca para o controle do dispaly oled
#include <SPI.h> // Biblioteca para usar o SPI
#include <Servo.h> // Biblioteca para controle do servo motor
#include <OneWire.h> // Biblioteca para o sensor de temperatura se comunique usando apenas um pino logico
#include <DallasTemperature.h> // Biblioteca que permite a leitura da temperatura fornecida pelo sensor

Servo myservo; // Criacao de um objeto myservo do tipo Servo

// Variaveis ---------------------------------------------------------------------------------------------------------
int hora = 1; // Variavel que armazena as horas restantes do cronometro (deve ser 6)
int minuto = 0; // Variavel que armazena os minutos restantes do cronometro
int segundo = 0; // Variavel que armazena os segundos restantes do cronometro
const int horaMaxima = 48; // Variavel que armazena o limite maximo de horas
const int horaMinima = 3; // Variavel que armazena o limite minimo de horas
const int BotaoMais = 2; // Pino do botao de incremento de alguma variavel
const int BotaoMenos = 3; // Pino do botao de decremento de alguma variavel
const int BotaoPlay = 4; // Pino do botao que inicia o cronometro
const int BotaoModo = A0; // Pino do botao que vai controlar o modo dos botoes mais e menos
const int ldr = A1; // Pino analógico do ldr
int Modo = 0; // Variavel para armazenar os modos de controle
int anguloServo = 0; // Variavel que armazena o angulo do servo motor
float delayServo = 200; // Variavel que vai armazenar o tempo que vai ficar liberando racao atraves do Servo Motor
int hDefinida = 6; // Variavel que vai armazenar o horario que vai servir de base 
const int TFT_CS = 10; // E o pino do chip select do display st7735 que seleciona o display quando vários dispositivos estão conectados ao mesmo barramento SPI
const int TFT_RST = 8; // E o pino de reset do display. Quando esse pino é definido como LOW, o display é reiniciado
const int TFT_DC = 9; // é o pino que indica ao display se os dados que estão sendo transmitidos são comandos ou dados de exibição. Se for definido como LOW,
//os dados que estão sendo transmitidos são comandos (como definir a posição do cursor), se for definido como HIGH,
//os dados que estão sendo transmitidos são dados de exibição (como pixels).
// O pino SCL deve ser conectado na porta D13
// O pino SDA deve ser conectado na porta D11
const int rele = 5; // Pino do rele que controla o aquecedor 
float tempAtual; // Variavel para armazenar o valor atual da temperatura
int tempDesejada = 26; // Variavel para definir um valor desejado de temperatura
const int tempMax = 29;
const int tempMin = 24;
int sensorTemp = 7; // Pino para o armazenamento de dados do sensor de temperatura
int delayCronometro(3780);// Pois foi o tempo que correspondeu corretamente com o cronometro em tempo real no celular

OneWire oneWire(sensorTemp); // Protocolo da biblioteca
DallasTemperature sensors(&oneWire); // Protocolo da biblioteca
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST); // Protocolo da biblioteca

/* --- Tabela de cores do display: ---
AMARELO:ST7735_RED
LARANJA: ST7735_ORANGE
VERMELHO: ST7735_YELLOW
ROXO: ST7735_GREEN
AZUL CLARO: ST7735_BLUE
AZUL ESCURO: ST7735_CYAN
VERDE: ST7735_MAGENTA
------------------------------------*/

// Definindo as as entradas e saidas -----------------------------------------------------------------------------------------
void setup() {
  sensors.begin(); // Inicia o sensor de temperatura 

  tft.initR(INITR_BLACKTAB); // Inicializa o display
  tft.setRotation(3); // Faz uma rotação de 90 graus 3 vezes que equivale a 270 graus, fazendo o display ser exibido deitado
  tft.fillScreen(ST7735_WHITE); // Preenche a tela de AZUL, pois o preto e o branco sao invertidos neste display

  myservo.attach(6); // Define o pino 6 como o pino de controle do servo motor
  pinMode(BotaoMais, INPUT_PULLUP); // Define o pino do botao de incremento de horas como entrada com resistor de pull-up
  pinMode(BotaoMenos, INPUT_PULLUP); // Define o pino do botao de decremento de horas como entrada com resistor de pull-up
  pinMode(BotaoModo, INPUT_PULLUP);
  pinMode(BotaoPlay, INPUT_PULLUP);
  pinMode(ldr, INPUT);
  pinMode(rele, OUTPUT);

  tft.setCursor(20,35);
  tft.setTextSize(2);
  tft.setTextColor(ST7735_ORANGE); 
  tft.print("CONFIGURE:");
  delay(3000);

}

// Inicio das repeticoes da programacao -----------------------------------------------------------------------------------
void loop() {
  sensors.requestTemperatures(); // Ler temperatura
  tempAtual = sensors.getTempCByIndex(0); // Obter temperatura em Celsius

  anguloServo = 90; // Angulo inicial do servo
  myservo.write(anguloServo); // Para ele ler o servo

  // Logica para a mudanca de modo a partir do "botao modo" -----------------------------------------------------------------------
  int valor = digitalRead(A0);
  if (valor == HIGH){ // Se o botao modo ligar
    Modo = Modo + 1; // a variavel "modo" acrescenta um valor
    if (Modo == 1){ // Se o modo for igual a 1 ele configura o tempo em que a portinhola da racao fica aberta
      tft.setCursor(20,35);
      tft.setTextSize(2);
      tft.setTextColor(ST7735_ORANGE); 
      tft.print("MODO QUANT");
      tft.setCursor(30,60);
      tft.setTextSize(2);
      tft.setTextColor(ST7735_ORANGE); 
      tft.print("DE RACAO");
      delay(700);
    }
    if (Modo == 2){ // Se o modo for igual a 2 ele configura a temperatura
      tft.setCursor(55,35);
      tft.setTextSize(2);
      tft.setTextColor(ST7735_GREEN); 
      tft.print("MODO");
      tft.setCursor(15,60);
      tft.setTextSize(2);
      tft.setTextColor(ST7735_GREEN); 
      tft.print("TEMPERATURA");
      delay(700);
    }
    if (Modo > 2){ // Se o modo for igual a 0 ele configura o cronometro
      Modo = 0;
      tft.setCursor(55,35);
      tft.setTextSize(2);
      tft.setTextColor(ST7735_CYAN); 
      tft.print("MODO");
      tft.setCursor(20,60);
      tft.setTextSize(2);
      tft.setTextColor(ST7735_CYAN); 
      tft.print("CRONOMETRO");
      delay(700);
    }
  }

  // Temperatura na parte inferior do display ---------------------------------------------------------------------------------------
  tft.fillScreen(ST7735_WHITE); // Deixa o display preto apos 1 segundo
  sensors.requestTemperatures(); // Sempre que decrementar uma hora sera feita a leitura da temperatura
  tempAtual = sensors.getTempCByIndex(0); // Obter temperatura em Celsius
  tft.setCursor(5,95); // Defino a localizacao onde o meu texto vai ser exibido
  tft.setTextColor(ST7735_BLUE); // Defino a cor vermelha para o meu texto
  tft.setTextSize(1); // Defino o tamanho 3 pre definido pelas bibliotecas
  tft.print("TEMPERATURA: "); // Aparece a mensagem "REGULANDO TEMPERATURA" no display
  tft.setCursor(90,90); // Defino a localizacao onde o meu texto vai ser exibido
  tft.setTextColor(ST7735_BLUE); // Defino a cor vermelha para o meu texto
  tft.setTextSize(2); // Defino o tamanho 3 pre definido pelas bibliotecas
  tft.print(tempAtual); // Aparece a mensagem "TEMPERATURA" no display

  // Logica para o controle da temperatura -------------------------------------------------------------------------------------------
  if (tempAtual > tempDesejada){ // Se a temperatura atual for menor do que a temperatura desejada ele vai fazer o que vem a baixo
    digitalWrite(rele, HIGH); // Ligar o rele para o aquecedor ligar
    sensors.requestTemperatures(); // Sempre que decrementar uma hora sera feita a leitura da temperatura
    tempAtual = sensors.getTempCByIndex(0); // Obter temperatura em Celsius
  }
  else { // Se a temperatura estiver acima da desejada, ele vai fazer o que vem a baixo
    digitalWrite(rele, LOW); // Desliga o rele para o aquecedor desligar
  }
  
  // logica para escolher o que sera controlado com os botoes de mais e menos ----------------------------------------------------------------
  if (Modo == 1){ // Se o modo for igual a 1, sera controlada a quantidade de racao a ser liberada
    if (digitalRead(BotaoMais) == HIGH){ //Se BotaoMais ligar:
      if (delayServo < 3000){ //Verifica se o angulo do servo nao passa de 0 nem de 45 graus
        delayServo = delayServo + 100; //Incremento de 100 milisegundos no tempo de ejecao da racao pelo Servo Motor
        tft.setCursor(10,55); // Localizacao do texto "+ RACAO"
        tft.setTextSize(3); // Tamanho do texto 
        tft.setTextColor(ST7735_MAGENTA); // Cor verde para o texto
        tft.print("+ RACAO"); // Texto a ser exibido
        tft.setCursor(5, 30); // Localizacao do texto "TIME: "
        tft.setTextSize(1);// Tamanho do texto
        tft.setTextColor(ST7735_ORANGE); // Cor vermelha para o texto
        tft.print("TIME: "); // Texto a ser exibido
        tft.print(delayServo/1000); // Divido o tempo que esta em mili por mil, para ele ser representado em segundos
        tft.print(" seconds"); // Adiciono a legenda " seconds" a frente do valor 
        delay(700); //Tempo de intervalo de incremento para incremento
        tft.fillScreen(ST7735_WHITE); // Deixa o display preto apos 1 segundo
      }
      else{ // Se o tempo for menor que 0 e maior que 45 aparece a mensagem: "TEMPO INSUFICIENTE"
        tft.setCursor(20,45);
        tft.setTextSize(2);
        tft.setTextColor(ST7735_YELLOW); 
        tft.print("QUANTIDADE");
        tft.setCursor(20,70);
        tft.setTextSize(2);
        tft.setTextColor(ST7735_YELLOW); 
        tft.print("INADEQUADA");
        delayServo = delayServo - 100;
        tft.setCursor(5,30);
        tft.setTextSize(1);
        tft.setTextColor(ST7735_ORANGE);
        tft.print("TIME: ");
        tft.print(delayServo/1000);
        tft.print(" seconds");
        delay(700);
      }
    }
    if (digitalRead(BotaoMenos) == HIGH){ //Se o BotaoMenos ligar:
      if (delayServo > 100){
        delayServo = delayServo - 100; //Decremento de 100 milisegundos no tempo de ejecao da racao pelo Servo Motor
        tft.setCursor(10,55);
        tft.setTextSize(3);
        tft.setTextColor(ST7735_YELLOW); 
        tft.print("- RACAO");
        tft.setCursor(5, 30);
        tft.setTextSize(1);
        tft.setTextColor(ST7735_ORANGE);
        tft.print("TIME: ");
        tft.print(delayServo/1000);
        tft.print(" seconds");
        delay(700); //Tempo de intervalo de decremento para decremento
        tft.fillScreen(ST7735_WHITE); // Deixa o display preto apos 1 segundo
      }
      else{ // Se o tempo for menor que 0 e maior que 45 aparece a mensagem: "TEMPO INSUFICIENTE"
        tft.setCursor(20,45);
        tft.setTextSize(2);
        tft.setTextColor(ST7735_YELLOW); 
        tft.print("QUANTIDADE");
        tft.setCursor(20,70);
        tft.setTextSize(2);
        tft.setTextColor(ST7735_YELLOW); 
        tft.print("INADEQUADA");
        delayServo = delayServo + 100;
        tft.setCursor(5,30);
        tft.setTextSize(1);
        tft.setTextColor(ST7735_ORANGE);
        tft.print("TIME: ");
        tft.print(delayServo/1000);
        tft.print(" seconds");
        delay(700);
      }
    }
  }
    

  if (Modo == 0){ // Se o modo for igual a 0, sera controlado o cronometro para a liberacao da racao
    // Programacao dos botoes de incremento e decremento de horas no sistema ----------------------------------------------------
    if (digitalRead(BotaoMais) == HIGH) { // Se o botao de incremento for pressionado  
      if (hora < horaMaxima) { // Verifica se ainda ha espaco para aumentar as horas
        hora = hora + 1;
        tft.setCursor(5,30); // Localizacao do texto "+ RACAO"
        tft.setTextSize(2); // Tamanho do texto 
        tft.setTextColor(ST7735_CYAN); // Cor verde para o texto
        tft.print("CRONOMETRO"); // Texto a ser exibido
        tft.setCursor(5,50); // Defino a localizacao onde o meu texto vai ser exibido
        tft.setTextColor(ST7735_CYAN); // Defino a cor vermelha para o meu texto
        tft.setTextSize(2); // Defino o tamanho 2 pre definido pelas bibliotecas
        tft.print("DESEJADO: "); // Aparece o valor escolhido de temperatura
        tft.setTextColor(ST7735_CYAN); // Defino a cor vermelha para o meu texto
        tft.setTextSize(2); // Defino o tamanho 2 pre definido pelas bibliotecas
        tft.print(hora); // Aparece o valor escolhido de temperatura
        tft.setCursor(5,75); // Localizacao do texto "+ RACAO"
        tft.setTextSize(1); // Tamanho do texto 
        tft.setTextColor(ST7735_MAGENTA); // Cor verde para o texto
        tft.print("+1 hora"); // Texto a ser exibido
        delay(700); // Apos meio segundo a tela limpa
        tft.fillScreen(ST7735_WHITE); // Deixa o display preto apos 1 segundo

        hDefinida = hora; // Faz com que a variavel "hDefinida" armazene o valor da variavel "horas"
        // para o cronometro voltar a partir deste valor armazenado.
      }
      else{
        tft.setCursor(50,35);
        tft.setTextSize(2);
        tft.setTextColor(ST7735_YELLOW); 
        tft.print("TEMPO");
        tft.setCursor(5,60);
        tft.setTextSize(2);
        tft.setTextColor(ST7735_YELLOW); 
        tft.print("INSUFICIENTE");
        delay(700);
      }
    }
    if (digitalRead(BotaoMenos) == HIGH) { // Se o botao de decremento de horas for pressionado
      if (hora > horaMinima) { // Verifica se ainda existe espaco para diminuir as horas
        hora = hora - 1;
        tft.setCursor(5,30); // Localizacao do texto "+ RACAO"
        tft.setTextSize(2); // Tamanho do texto 
        tft.setTextColor(ST7735_CYAN); // Cor verde para o texto
        tft.print("CRONOMETRO"); // Texto a ser exibido
        tft.setCursor(5,50); // Defino a localizacao onde o meu texto vai ser exibido
        tft.setTextColor(ST7735_CYAN); // Defino a cor vermelha para o meu texto
        tft.setTextSize(2); // Defino o tamanho 2 pre definido pelas bibliotecas
        tft.print("DESEJADO: "); // Aparece o valor escolhido de temperatura
        tft.setTextColor(ST7735_CYAN); // Defino a cor vermelha para o meu texto
        tft.setTextSize(2); // Defino o tamanho 2 pre definido pelas bibliotecas
        tft.print(hora); // Aparece o valor escolhido de temperatura
        tft.setCursor(5,75); // Localizacao do texto "+ RACAO"
        tft.setTextSize(1); // Tamanho do texto 
        tft.setTextColor(ST7735_YELLOW); // Cor verde para o texto
        tft.print("-1 hora"); // Texto a ser exibido
        delay(700); // Apos meio segundo a tela limpa
        tft.fillScreen(ST7735_WHITE); // Deixa o display preto apos 1 segundo

        hDefinida = hora; // Faz com que a variavel "hDefinida" armazene o valor da variavel "horas"
        // para o cronometro voltar a partir deste valor armazenado.
      }
      else{
        tft.setCursor(50,35);
        tft.setTextSize(2);
        tft.setTextColor(ST7735_YELLOW); 
        tft.print("TEMPO");
        tft.setCursor(5,60);
        tft.setTextSize(2);
        tft.setTextColor(ST7735_YELLOW); 
        tft.print("INSUFICIENTE");
        delay(700);
      }
    }
  }

  if (Modo == 2){ // Se o modo for igual a 2, sera controlada a temperatura, lembrando que ela nunca ficara abaixo da temperatura ambiente,
    //pois o projeto não tem a capacidade de diminuir a temperatura, apenas aumentar, e nem acima da capacidade do aquecedor utilizado no projeto
    if (digitalRead(BotaoMais) == HIGH){ //Se BotaoMais ligar:
      if (tempDesejada < tempMax){
        tempDesejada = tempDesejada + 1; //Incremento de 1 grau de temperatura
        tft.setCursor(5,30); // Localizacao do texto "+ RACAO"
        tft.setTextSize(2); // Tamanho do texto 
        tft.setTextColor(ST7735_GREEN); // Cor verde para o texto
        tft.print("TEMPERATURA"); // Texto a ser exibido
        tft.setCursor(5,50); // Defino a localizacao onde o meu texto vai ser exibido
        tft.setTextColor(ST7735_GREEN); // Defino a cor vermelha para o meu texto
        tft.setTextSize(2); // Defino o tamanho 2 pre definido pelas bibliotecas
        tft.print("DESEJADA: "); // Aparece o valor escolhido de temperatura
        tft.setTextColor(ST7735_GREEN); // Defino a cor vermelha para o meu texto
        tft.setTextSize(2); // Defino o tamanho 2 pre definido pelas bibliotecas
        tft.print(tempDesejada); // Aparece o valor escolhido de temperatura
        tft.setCursor(5,75); // Localizacao do texto "+ RACAO"
        tft.setTextSize(1); // Tamanho do texto 
        tft.setTextColor(ST7735_MAGENTA); // Cor verde para o texto
        tft.print("+1 *C"); // Texto a ser exibido
        delay(700); // Apos meio segundo a tela limpa
        tft.fillScreen(ST7735_WHITE); // Deixa o display preto apos 1 segundo
      }
      else{
        tft.setCursor(10,35);
        tft.setTextSize(2);
        tft.setTextColor(ST7735_YELLOW); 
        tft.print("TEMPERATURA");
        tft.setCursor(15,60);
        tft.setTextSize(2);
        tft.setTextColor(ST7735_YELLOW); 
        tft.print("INADEQUADA");
        delay(700);
      }
    }
    if (digitalRead(BotaoMenos) == HIGH){ //Se o BotaoMenos ligar:
      if (tempDesejada > tempMin){
        tempDesejada = tempDesejada - 1; //Decremento de 1 grau de temperatura
        tft.setCursor(5,30); // Localizacao do texto "+ RACAO"
        tft.setTextSize(2); // Tamanho do texto 
        tft.setTextColor(ST7735_GREEN); // Cor verde para o texto
        tft.print("TEMPERATURA"); // Texto a ser exibido
        tft.setCursor(5,50); // Defino a localizacao onde o meu texto vai ser exibido
        tft.setTextColor(ST7735_GREEN); // Defino a cor vermelha para o meu texto
        tft.setTextSize(2); // Defino o tamanho 2 pre definido pelas bibliotecas
        tft.print("DESEJADA: "); // Aparece o valor escolhido de temperatura
        tft.setTextColor(ST7735_GREEN); // Defino a cor vermelha para o meu texto
        tft.setTextSize(2); // Defino o tamanho 2 pre definido pelas bibliotecas
        tft.print(tempDesejada); // Aparece o valor escolhido de temperatura
        tft.setCursor(5,75); // Localizacao do texto "+ RACAO"
        tft.setTextSize(1); // Tamanho do texto 
        tft.setTextColor(ST7735_YELLOW); // Cor verde para o texto
        tft.print("-1 *C"); // Texto a ser exibido
        delay(700); // Apos meio segundo a tela limpa
        tft.fillScreen(ST7735_WHITE); // Deixa o display preto apos 1 segundo
      }
      else{
        tft.setCursor(10,35);
        tft.setTextSize(2);
        tft.setTextColor(ST7735_YELLOW); 
        tft.print("TEMPERATURA");
        tft.setCursor(15,60);
        tft.setTextSize(2);
        tft.setTextColor(ST7735_YELLOW); 
        tft.print("INADEQUADA");
        delay(700);
      }
    }
  }

  // Logica do botao "play" que vai ativar o cronometro ----------------------------------------------------------
  if (digitalRead(BotaoPlay) == HIGH){ // Se o botao "Play" estiver ativo, execute o que vem a baixo
    myservo.write(anguloServo); // Faz com que o Servo Motor inicie na posicao zero(0);
    // Logica para o sensor de aviso, caso o suprimento acabe, usando buzzer e um fotorresistor ldr ----------------------------------
    int valldr = analogRead(ldr);
    Serial.print(valldr);
    if (valldr < 70){
      tft.setCursor(30,40); // Defino a localizacao onde o meu texto vai ser exibido
      tft.setTextColor(ST7735_CYAN); // Defino a cor vermelha para o meu texto
      tft.setTextSize(2); // Defino o tamanho 3 pre definido pelas bibliotecas
      tft.print("RACAO EM:"); // Aparece a mensagem "REGULANDO TEMPERATURA" no display
      tft.setCursor(37,60); // Defino a localizacao onde o meu texto vai ser exibido
      tft.setTextColor(ST7735_CYAN); // Defino a cor vermelha para o meu texto
      tft.setTextSize(2); // Defino o tamanho 3 pre definido pelas bibliotecas
      tft.print(hora);
      tft.print(":");
      tft.print(minuto);
      tft.print(":");
      tft.print(segundo);

      // Logica para o cronometro ------------------------------------------------------------------
      if (segundo > 0){
      segundo = segundo - 5;
      }
      if (minuto > 0 && segundo == 0){
        segundo = 10; //deve ser 60
        minuto = minuto -1;
      }
      if (hora > 0 && minuto == 0){
        minuto = 0; //deve ser 59
        segundo = 10; //deve ser 60
        hora = 0; //hora - 1;
      }
      delay(delayCronometro);
      tft.fillScreen(ST7735_WHITE); // Deixa o display preto apos 1 segundo
    }
    else{
      tft.setCursor(23,35);
      tft.setTextSize(2);
      tft.setTextColor(ST7735_YELLOW); 
      tft.print("SUPRIMENTO");
      tft.setCursor(10,60);
      tft.setTextSize(2);
      tft.setTextColor(ST7735_YELLOW); 
      tft.print("INSUFICIENTE");
      delay(700);
    }
  }
  else{
    minuto = 0;
    segundo = 0;
    hora = hDefinida;
  }
  
  // Caso o tempo chegue a zero ele fara o que e pedido ----------------------------------------------------------------------------
  if ((hora == 0) && (minuto == 0) && (segundo == 0)){ // Se o cronometro zerar, faca o que vem a baixo:
    tft.fillScreen(ST7735_WHITE);
    tft.setCursor(35,42);
    tft.setTextSize(2);
    tft.setTextColor(ST7735_ORANGE); 
    tft.print("LIBERANDO");
    tft.setCursor(50,72);
    tft.setTextSize(2);
    tft.setTextColor(ST7735_ORANGE); 
    tft.print("RACAO");
    delay(2000);
    tft.fillScreen(ST7735_WHITE); // Deixa o display preto apos 1 segundo

    anguloServo -= 40; // Defino um angulo para o Servo Motor 
    myservo.write(anguloServo); // Faz o Servo Motor ler o valor do angulo definido 
    delay(delayServo); // Determina um intervalo de 0,5 segundos
    anguloServo += 40; // Define o angulo que o Servo devera retornar
    myservo.write(anguloServo); // Faz o Servo Motor voltar ao valor inicial
    delay(500); // Define um tempo de 0,5 segundos para a proxima funcao

    tft.setCursor(15, 42); // Defino a localizacao onde o meu texto vai ser exibido
    tft.setTextColor(ST7735_GREEN); 
    tft.setTextSize(2);
    tft.print("REINICIANDO");
    tft.setCursor(20, 72); // Defino a localizacao onde o meu texto vai ser exibido
    tft.setTextColor(ST7735_GREEN); 
    tft.setTextSize(2);
    tft.print("CRONOMETRO");
    hora = hDefinida; // Faz o cronometro voltar ao valor definido anteriormente e reinicie sua contagem
    delay(2000); // Delay de 2s para reiniciar o cronometro
    tft.fillScreen(ST7735_WHITE); // Deixa o display preto apos 2 segundos
  }
}
