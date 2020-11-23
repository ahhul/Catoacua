/* Catoacua - medidor de nível de água em recipiente de alimentação de pets       */
/* Projeto Final da disciplina MAC0546 Fundamentos da Internet das Coisas 2020/2  */
/* Aluna: Ludmila Ferreira Vicente e Silva                                        */

#include <CTBot.h>

#define REDE "" // insira o nome da rede
#define SENHA "" // insira a senha da rede
#define TOKEN "" // insira o token do bot no telegram
#define PINO A0 // pino de entrada analogia do NodeMcu


CTBot bot_telegram;
int saida_sensor = 0;

void setup () {
  Serial.begin (9600); // inicia a comunicacao serial

  bot_telegram.wifiConnect (REDE, SENHA); // conecta a rede
  bot_telegram.setTelegramToken (TOKEN); // define o bot do telegram

  Serial.println ("Iniciando conexão de rede\n");
  if (bot_telegram.testConnection ())
    Serial.println ("\nA conexão foi estabelecida.");
  else
    Serial.println ("\nA conexão falhou.");
    
  pinMode (PINO, INPUT); // Define
}

void loop() {
  TBMessage mensagem;
  String atualizacao; // armazena a mensagem a ser enviada ao usuario
  int tempo = 500; // define o tempo base para as notificacoes
  
  saida_sensor = analogRead (PINO); // lê o valor no pino A0
  Serial.println (saida_sensor); // imprime no serial o nível atual do líquido

  // Procura por uma nova mensagem e avalia o seu conteudo, 
  // se ela for um comando valido alguma das condicoes abaixo é executada
  if (bot_telegram.getNewMessage(mensagem)) {
    //primeira mensagem quando se inicia o bot
    Serial.println ("Mensagem recebida");
    
    if ((mensagem.text == "/start") || bot_telegram.getNewMessage (mensagem)) {
      // mostra as opcoes do usuario
      atualizacao = "Bem vindo ao *Catoacua*!\nEste bot avisa quando seu pet está prestes a" 
                     "ficar sem água, não se esqueça de abastecer e limpar o recipiente!\n"
                     "Para monitorar o nível água do seu pet envie \"Nivel atual\".";
      bot_telegram.sendMessage (mensagem.sender.id, atualizacao);
    }
  
    if ((mensagem.text == "Nivel atual") || (mensagem.text == "nivel atual")) {
      // retorna o nivel de agua atual
      atualizacao = "O nível atual de agua no pote é: " + (String) saida_sensor;
      bot_telegram.sendMessage (mensagem.sender.id, atualizacao);
    }
  }
  
  if (saida_sensor <= 40) {
    if (saida_sensor == 0){
      // notifica o baixo nível no recipiente ao usuario
      atualizacao = "A água dos seus pets acabou!\n"
                     "Corra para reabastecer o recipiente!\n"
                     "Nível atual: " + (String) saida_sensor;
      bot_telegram.sendMessage (mensagem.sender.id, atualizacao);
    } else {
      // notifica recipiente vazio ao usuario
      atualizacao = "A água dos seus pets está acabando!\n"
                     "Corra para abastecer o recipiente!\n"
                     "Nível atual: " + (String) saida_sensor;
      bot_telegram.sendMessage (mensagem.sender.id, atualizacao);
    }
    
    delay (tempo * tempo * 30); // aguarda o tempo definido até a proxima iteracao do loop
    
  } else {
    
    delay (tempo);   
  }
}
