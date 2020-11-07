#include <LiquidCrystal.h>
#include <string.h>
#include <stdio.h>

//Variaveis globais
String words[]={"bico","fato","brio","caos"};
int count;
char letra[]="abcdefghijklmnopqrstuvwxyz";
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
char wd[16];
bool state;
String wordOption[] = {"Palavra 1", "Palavra 2","Palavra 3", "Palavra 4"};
int loser;
char filter[16];
//Função de configuração unica
void setup() {
  
    // Inicializa o hardware atribuindo a porta e a função
    pinMode(2,INPUT_PULLUP);
    pinMode(3,INPUT_PULLUP);
    pinMode(4,INPUT_PULLUP);
    pinMode(5,OUTPUT);
    pinMode(6,OUTPUT);
    pinMode(7,OUTPUT);
    //serial para apresentação de status no console
    Serial.begin(9600);
    //ligar leds
    digitalWrite(5,HIGH);
    digitalWrite(6,HIGH);
    digitalWrite(7,HIGH);
    // Inicializa o cursor em 16*2
    lcd.begin(16,2);
    lcd.print("Bem vindo!");
    lcd.setCursor(0,1);
    lcd.print("#Jogo da forca#");
  
	delay(1000);
  
    lcd.begin(16,2);
    lcd.print("SI-Prof Heuber");
    lcd.setCursor(0,1);
    lcd.print("##Joao  Victor##");
    // Inicializa as variaveis com seus valores default
	count = 0;
    loser = 3;
    state = false;
    String empty="";
  
    for(int i=0; strlen(wd); i++)
      wd[i]=empty[i];

    delay(1000);// Seta um delay de 1,5 segundos 
    lcd.begin(16,2); // Inicia o cursor em 16*2
    lcd.print("Esco. a palavra:"); // Solicita a entrada da letra
}
//Função loop do arduino que ferra com todo o esquema
void loop() {
    int stateUp = digitalRead(4);
    int stateDown = digitalRead(3);
    int stateSelect = digitalRead(2);
  
    //Verifica o estado inverso dos botoes, se 0, executam suas tarefas com um delay de 300 milesegundos
    if(stateUp == 0){
       selectUp();
       delay(300);
    }
    if(stateDown == 0){
        selectDown();
        delay(300);
    }
    if(stateSelect == 0){
        selectChar();
        delay(300);
    }
}
//Função de escolha up - Hardware
void selectUp(){
  if(strlen(wd) == 0){
    state = true;
    lcd.setCursor(0,1);
    lcd.print(wordOption[count]);
   	count++;
    return;
  }
  state = true;
  lcd.setCursor(5,1);
  lcd.print(letra[count]);
  count++;
}
//Função de escolha down - Hardware
void selectDown(){
    if(strlen(wd) == 0){
    state = false;
    lcd.setCursor(0,1);
    count--;
    lcd.print(wordOption[count]);
    return;
  }
  state = false;
  lcd.setCursor(5,1);
  count--;
  lcd.print(letra[count]);
}
//Função de seleção da letra - Hardware
void selectChar(){
  if(state)
    count--;
  if(strlen(wd) == 0){
    String wordSingle = words[count];
    for(int i=0; i < wordSingle.length(); i++){
      wd[i] = wordSingle[i];
    }
    //Preenche o array filter com '_' do mesmo tamanho do array wd
  	for(int i = 0; i<strlen(wd); i++)
  		filter[i]='_';
   
    count = 0;
    state = false;
    lcd.begin(16,2);
  	lcd.print("Esco. uma letra"); // Solicita a entrada da letra
    return;
  }
  //Chama a função responsavel pela regra de negocio do game
  validateTest(letra[count]);
}

//Função de validação - Regra de negocio do game
void validateTest(char c){
    int error = 0;
    lcd.begin(0,2);
    for(int i = 0; i < strlen(wd); i++){
        if(wd[i] == letra[count]){
            filter[i] = wd[i];
        }else // Para cada letra de wd se nao houver correspondencia incrementa a variavel error
        error++;
    }
    // Se error for igual ao tamanho de wd significa que nao houve nenhuma correspondencia
    if(error  == strlen(wd)){
        loser--; // Decrementa a variavel de chances do jogador

    if(loser == 2){
        digitalWrite(7, LOW);
    }
    if(loser == 1){
        digitalWrite(6, LOW);
    }
    if(loser == 0){ // Verifica se loser chegou a 0 ou seja as chances do jogador terminaram
        lcd.print("Voce perdeu!"); // Dispara a mensagem
        for(int i = 0; i < 4; i++){
            digitalWrite(5, HIGH);
            digitalWrite(6, HIGH);
            digitalWrite(7, HIGH);
            delay(500);
            digitalWrite(5, LOW);
            digitalWrite(6, LOW);
            digitalWrite(7, LOW);
            delay(500);
        }

      setup(); // chama função principal para reiniciar o game
    }else{ // Se loser nao for 0 o jogador ainda tem chances,
        lcd.print(filter); // Mostra o progresso do game  e emite uma menssagem para o jogador
        lcd.setCursor(0,1);
        lcd.print("Ops! Errado");

        count = 0; // Seta o contador do array de letras como 0
        delay(1000); // Sguarda 1 segundo
        lcd.begin(16,2); // Inicia o cursor e passa a mensagem novamente Ecolha uma letra
        lcd.print("Esco. a letra: ");
    }
    // Se error nao for igual ao strlen(wd) significa que foi encontrao  uma letra equivalente
    }else{ 
    //Percorre o array filter procurando se ainda existem caracteres '_'
    // se o caractere na posição i nao for igual a '_' incrementa entao a variavel charactere
    int charactere = 0;
    lcd.print(filter);
    for(int i=0; i < strlen(filter); i++){
        if(filter[i] != '_')
        charactere++;
    }
    //Se charactere for igual ao tamanho de filter significa que nao ha nenhum '_' entao o jogador venceu
    if(charactere == strlen(filter)){
        lcd.blink(); //Pisca o visor (nao funciona no simulador)
        lcd.setCursor(0,1); //seta a posição do cursor em coluna 0 linha 1 
        lcd.print("###Parabens!!###"); // imprime a msg de vitoria
        delay(1000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("A palavra correta");
        lcd.setCursor(0,1);
        lcd.print(filter);

        for(int i = 0; i < 4; i++){
            digitalWrite(5, HIGH);
            delay(200);
            digitalWrite(5, LOW);
            delay(200);
            digitalWrite(6, HIGH);
            delay(200);
            digitalWrite(6, LOW);
            delay(200);
            digitalWrite(7, HIGH);
            delay(200);
            digitalWrite(7, LOW);
            delay(200);
    }
    setup(); // chama função principal para reiniciar o game

    }else{ //Caso ainda hajam caracteres '_' ou seja o game nao terminou
        lcd.setCursor(0,1); //Seta a posição do cursor em coluna 0 e linha 1
        lcd.print("Esta correto!!"); //Dispara a menssagem que sua escolha esta correta

        delay(1000); //Aguarda 1 segundo
        lcd.begin(16,2); // Incializa o display em 16*2 limpando a tela
        lcd.print("Esco. a letra: "); // e Recomeça para a proxima chance
        count =0; // Seta o contador do array de letras para 0
        }
    }
}