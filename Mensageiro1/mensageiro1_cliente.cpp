#include <iostream>
#include <string.h>
#include <thread>

#include "../MySocket/mysocket.h"
#include "mensageiro.h"

using namespace std;

/// Este programa tem uma thread principal que espera que o usuário
/// digite alguma coisa e envia esta string para um servidor via
/// socket. Uma outra thread imprime todas as strings recebidas pelo
/// socket.
///
/// PROTOCOLO DE COMUNICACAO
/// Cliente -> servidor:
/// A) Primeira mensagem:
///    1) Envia a string com o nome do usuario
/// B) Mensagens seguintes:
///    1) Envia a string com o nome do destinatario
///    2) Envia a string com a mensagem
/// Servidor -> cliente:
/// 1) Envia a string com o nome do remetente
/// 2) Envia a string com a mensagem

/// Aqui vao as variáveis globais das duas threads

// O socket de comunicacao
tcp_mysocket s;
// O flag que indica que o software deve encerrar todas as threads
bool fim = false;

/// Esta e a thread que escreve em tela as mensagens recebidas

void main_thread(void)
{
  string remetente;
  string msg;
  mysocket_status iResult;

  cout << "Iniciando a thread de leitura do socket!\n";
  while (!fim)
  {
    try
    {
      // Leh login do remetente (sem timeout)
      iResult = s.read_string(remetente);
      if (iResult==mysocket_status::SOCK_ERROR) throw "leitura do remetente";
      if (iResult==mysocket_status::SOCK_DISCONNECTED) throw "servidor desconectou";
      // Leh texto da mensagem
      // Maximo MENSAGEIRO_TIMEOUT segundos depois do remetente
      iResult = s.read_string(msg, 1000*MENSAGEIRO_TIMEOUT);
      if (iResult==mysocket_status::SOCK_ERROR) throw "leitura do texto";
      if (iResult==mysocket_status::SOCK_DISCONNECTED) throw "servidor desconectou";
      if (iResult==mysocket_status::SOCK_TIMEOUT) throw "timeout no texto";
      // Avisa msg recebida
      cout << endl
           << "=========================================================\n"
           << "Mensagem recebida de " << remetente <<":\n"
           << msg << endl
           << "=========================================================\n";
      fim = (msg=="FIM");
    }
    catch(const char* err)
    {
      cerr << "Erro na leitura do socket: " << err << endl;
      fim = true;
    }
  }
  cout << "Fim da thread de leitura do socket!\n";
}

/// O programa principal contem o ciclo que envia as mensagens digitadas

int main(int argc, char **argv)
{
  thread thr;
  string usuario("");
  string msg("");
  mysocket_status iResult;

  // Inicializa a biblioteca de sockets
  iResult = mysocket::init();
  if (iResult != mysocket_status::SOCK_OK)
  {
    cerr << "Biblioteca mysocket nao pode ser inicializada\n";
    exit(-1);
  }

  // Determina o numero IP do servidor
  cout << "Maquina onde estah rodando o servidor (IP): ";
  cin >> msg;

  // Conecta com o servidor
  iResult = s.connect(msg, MENSAGEIRO_PORTA);
  if (iResult == mysocket_status::SOCK_OK)
  {
    cout << "Conectado ao servidor " << msg << " na porta " << MENSAGEIRO_PORTA << endl;
  }
  else
  {
    cerr << "Problema na conexao ao servidor " << msg << " na porta " << MENSAGEIRO_PORTA << endl;
    exit(-2);
  }

  // Cria a thread que escreve as mensagens recebidas
  thr = thread(main_thread);
  if (!thr.joinable())
  {
    cerr << "Problema ao criar thread\n";
    s.close();
    exit(-3);
  }

  // Espera 1 seg para as mensagens nao se sobreporem no terminal
  this_thread::sleep_for(chrono::seconds(1));

  // Envia para o servidor o nome de usuario que identificarah esse cliente
  do
  {
    cout << "Login deste cliente [4 a 16 caracteres]: ";
    cin >> usuario;
  }
  while (usuario.size()<4 || usuario.size()>16);
  iResult = s.write_string(usuario);
  if ( iResult == mysocket_status::SOCK_ERROR )
  {
    cerr << "Problema na escrita do login no socket...\n";
    exit(-4);
  }

  // Este eh o ciclo que envia as mensagens digitadas
  while (!fim)
  {
    try
    {
      // Solicita o destinatario
      do
      {
        cout << "Usuario para enviar msg [4 a 16 caracteres, FIM para terminar]: ";
        cin >> usuario;
        if (usuario=="FIM") throw 1;
      } while (usuario.size()<4 || usuario.size()>16);

      // Solicita o texto da msg
      do
      {
        cout << "Mensagem a enviar [FIM para terminar]: ";
        cin >> ws;
        getline(cin, msg);
        if (msg=="FIM") throw 2;
      } while (msg.size()==0);

      // Envia o destinatario
      iResult = s.write_string(usuario);
      if ( iResult != mysocket_status::SOCK_OK ) throw 3;

      // Envia o texto
      iResult = s.write_string(msg);
      if ( iResult != mysocket_status::SOCK_OK ) throw 4;
    } // fim try
    catch (int erro)
    {
      // Encerra o ciclo de execucao
      fim = true;

      switch (erro)
      {
      case 1:
      case 2:
        // Usuario digitou "FIM"
        // Se o socket ainda estah aberto, informa ao servidor
        if (s.connected())
        {
          iResult = s.write_string("FIM");
          // Nao testa iResult pq o programa jah estah acabando mesmo...
        }
        break;
      case 3:
        cerr << "Problema na escrita do usuario no socket...\n";
        break;
      case 4:
        cerr << "Problema na escrita do texto no socket...\n";
        break;
      default:
        // Nunca deve chegar aqui
        cerr << "Erro desconhecido!\n";
        break;
      }
    } // fim catch

    // Espera 1 seg para as mensagens nao se sobreporem no terminal
    this_thread::sleep_for(chrono::seconds(1));
  }

  // Encerra o socket
  cout << "Fechando o socket...\n";
  s.close();

  // Espera pelo fim da thread de recepcao
  if (thr.joinable())
  {
    cout << "Esperando o fim da thread...\n";
    thr.join();
  }

  // Encerra a biblioteca de sockets
  mysocket::end();
}
