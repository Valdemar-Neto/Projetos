#include <iostream>
#include <string.h>
#include <list>
#include <algorithm>
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
/// 1) Envia a string com o nome do usuario
/// B) Mensagens seguintes:
/// 1) Envia a string com o nome do destinatario
/// 2) Envia a string com a mensagem
/// Servidor -> cliente:
/// 1) Envia a string com o nome do remetente
/// 2) Envia a string com a mensagem

/// Struct que armazena os dados que definem um cliente: login e socket
struct Client
{
  string login;
  tcp_mysocket s;

  Client();
  bool operator==(const string& L); // Para poder usar "find" da STL
};

Client::Client(): login(""), s() {}

bool Client::operator==(const string& L)
{
  return login==L;
}

/// Apelidos para uma lista de clientes e para o iterator correspondente
typedef list<Client> list_Client;
typedef list_Client::iterator iter_Client;

/// Variáveis globais das 2 threads

/// O socket de conexoes
tcp_mysocket_server c;
/// A lista de clientes
list_Client LC;
/// O flag que indica que o software deve encerrar todas as threads
bool fim = false;

/// Thread que efetivamente desempenha as tarefas do servidor
void servidor(void)
{
  // Fila de sockets para aguardar chegada de dados
  mysocket_queue f;
  // Socket temporario
  tcp_mysocket t;
  // Informacao lida do socket
  string usuario,msg;
  // Variaveis auxiliares
  mysocket_status iResult;
  iter_Client i,iDest;

  cout << "Iniciando a thread de leitura do socket!\n";
  while (!fim)
  {
    try // Erros mais graves que encerram o servidor
    {
      // Encerra se o socket de conexoes estiver fechado
      if (!c.accepting()) throw "socket de conexoes fechado";

      // Inclui na fila de sockets para o select todos os sockets que eu
      // quero monitorar para ver se houve chegada de dados
      f.clear();
      // Monitora o socket de conexoes
      f.include(c);
      // Monitora os sockets dos clientes conectados
      for (i=LC.begin(); i!=LC.end(); ++i)
      {
        if (i->s.connected()) f.include(i->s);
      }

      // Espera ateh que chegue dado em algum socket (sem timeout)
      iResult = f.wait_read();
      if (iResult != mysocket_status::SOCK_OK) throw "fila de espera";

      // Testa se houve atividade nos sockets dos clientes
      for (i=LC.begin(); !fim && i!=LC.end(); ++i)
      {
        if (i->s.connected() && f.had_activity(i->s))
        {
          try // Erros na leitura/envio de mensagem
          {
            // Leh usuario da mensagem
            iResult = i->s.read_string(usuario);
            if (iResult != mysocket_status::SOCK_OK) throw 1;

            // Testa encerramento voluntario
            if (usuario == "FIM") throw 2;

            // Leh texto da mensagem
            // Maximo MENSAGEIRO_TIMEOUT segundos depois do usuario
            iResult = i->s.read_string(msg, 1000*MENSAGEIRO_TIMEOUT);
            if (iResult != mysocket_status::SOCK_OK) throw 3;

            // Testa novamente o encerramento voluntario
            if (msg == "FIM") throw 2;

            // Testa o nome do usuario
            if (usuario.size()<4 || usuario.size()>16) throw 4;

            // Testa se o usuario existe, ou seja,
            // procura o usuario dentro da lista de clientes
            iDest = find(LC.begin(), LC.end(), usuario);
            if (iDest == LC.end()) throw 5;

            // Testa se o usuario estah conectado
            if (!iDest->s.connected()) throw 6;

            // Escreve o login do remetente no socket do usuario
            iResult = iDest->s.write_string(i->login);
            if ( iResult != mysocket_status::SOCK_OK ) throw 7;

            // Escreve o texto da msg
            iResult = iDest->s.write_string(msg);
            if ( iResult != mysocket_status::SOCK_OK ) throw 8;

            // Imprime msg recebida
            cout << i->login << "->" << usuario << ": " << msg << endl;
          } // fim try - Erros na leitura/envio de mensagem
          catch (int erro) // Erros na leitura/envio de mensagem
          {
            string msg_erro;
            switch (erro)
            {
            case 1:
            case 3:
              // Falha na comunicacao de socket do cliente remetente
              msg_erro = "Erro na leitura do socket do cliente " + i->login;
              i->s.close();
              break;
            case 2:
              msg_erro = "Cliente " + i->login + " desconectou";
              i->s.close();
              break;
            case 4:
            case 5:
              msg_erro = "Destinatario " + usuario + " inexistente";
              break;
            case 6:
              msg_erro = "Destinatario " + usuario + " nao estah conectado no momento";
              break;
            case 7:
            case 8:
              // Falha na comunicacao de socket do cliente usuario
              msg_erro = "Erro na escrita no socket do cliente " + iDest->login;
              iDest->s.close();
              break;
            default:
              // Nunca deve chegar aqui
              msg_erro = "Erro " + to_string(erro) + " desconhecido na leitura/envio de mensagem";
              break;
            } // fim switch

            // Comunica o erro:
            // Imprime msg no servidor ou envia msg para o cliente
            if (erro>=4 && erro<=8)
            {
              // Envia para o remetente
              // Escreve o "login" do servidor no socket do remetente
              iResult = i->s.write_string("SERVIDOR");
              if ( iResult == mysocket_status::SOCK_OK )
              {
                // Escreve o texto da msg_erro no socket do remetente
                iResult = i->s.write_string(msg_erro);
              }
              if ( iResult != mysocket_status::SOCK_OK )
              {
                cerr << "Erro na escrita no socket do cliente " << i->login << endl;
                i->s.close();
              }
            }
            else
            {
              // Imprime no servidor
              cerr << msg_erro << endl;
            }
          } // fim catch - Erros na leitura/envio de mensagem
        }  // if (i->s.connected() && f.had_activity(i->s))
      }  // for (i=LC.begin(); i!=LC.end(); ++i)

      // Depois, testa se houve atividade no socket de conexao
      if (!fim && f.had_activity(c))
      {
        // Estabelece nova conexao no socket temporario "t"
        if (c.accept(t) != mysocket_status::SOCK_OK) throw "nova conexao";

        try // Erros em nova conexao
        {
          // Leh o nome de login do cliente
          iResult = t.read_string(usuario);
          if (iResult != mysocket_status::SOCK_OK) throw 1;

          // Testa se o nome de login eh valido
          if (usuario.size()<4 || usuario.size()>16) throw 2;

          // Procura se jah existe um usuario cadastrado com o mesmo login
          i = find(LC.begin(), LC.end(), usuario);
          if (i != LC.end())
          {
            // Jah existe esse login cadastrado
            if (!i->s.connected())
            {
              // O usuario existe e estah se reconectando (OK)
              i->s.swap(t);
              cout << "Cliente " << usuario << " reconectado.\n";
            }
            else // if (!i->s.connected())
            {
              // O usuario existe e jah estah conectado (erro)
              throw 3;
            }
          }
          else // if (i != LC.end())
          {
            // Novo usuario (ainda nao cadastrado)
            Client novo;
            novo.login = usuario;
            novo.s.swap(t);
            LC.push_back(novo);
            cout << "Cliente " << usuario << " criado e conectado.\n";
          }
        } // fim try - Erros em nova conexao
        catch(int erro)
        {
          if (erro==1)
          {
            // Erro na leitura do socket temporario
            cerr << "Nao foi possivel ler o login de um cliente que se conectou.\n";
          }
          else
          {
            t.write_string("SERVIDOR");
            t.write_string("LOGIN "+usuario+" INVALIDO");
          }
          t.close();
        } // fim catch - Erros em nova conexao
      } // if (!fim && f.had_activity(c))

      // Nao elimina da lista de sockets as conexoes que foram fechadas
      // Os usuarios cadastrados permanecem na lista mesmo depois de desconectados

    } // fim try - Erros mais graves que encerram o servidor
    catch(const char* err)  // Erros mais graves que encerram o servidor
    {
      cerr << "Erro no servidor: " << err << endl;
      c.close();
      fim = true;
    } // fim catch - Erros mais graves que encerram o servidor
  } // while (!fim)

  cout << "Fim da thread de leitura do socket!\n";
}

/// O programa principal
/// Inicializa os sockets, lanca a thread,
/// e encerra a execucao com FIM

int main(void)
{
  thread thr;
  string msg;

  try
  {
    // Inicializa a biblioteca de sockets
    mysocket_status iResult = mysocket::init();
    if (iResult != mysocket_status::SOCK_OK)
      throw "Biblioteca mysocket nao pode ser inicializada";

    // Coloca o socket de conexoes em escuta
    if (c.listen(MENSAGEIRO_PORTA) != mysocket_status::SOCK_OK)
      throw "Nao foi possivel abrir o socket de conexoes";

    // Cria a thread que recebe e reenvia as mensagens
    thr = thread(servidor);
    if (!thr.joinable())
      throw "Problema ao criar thread";
  }
  catch (const char* erro)
  {
    cerr << "Erro na inicializacao: " << erro << endl;
    // Fecha o socket, caso esteja aberto
    c.close();
    // Encerra o programa
    exit(-1);
  }

  // Espera 1 seg para as mensagens nao se sobreporem no terminal
  this_thread::sleep_for(chrono::seconds(1));

  while (!fim)
  {
    do
    {
      cout << "FIM para terminar: ";
      cin >> ws;
      getline(cin,msg);
    }
    while (msg.size()==0);
    if (!fim) fim = (msg=="FIM");
  }

  // Encerra os sockets
  cout << "Fechando os sockets...\n";
  c.close();
  for (iter_Client i=LC.begin(); i!=LC.end(); ++i) i->s.close();

  // Espera pelo fim da thread do servidor
  if (thr.joinable())
  {
    cout << "Esperando o fim da thread...\n";
    thr.join();
  }

  // Encerra a biblioteca de sockets
  mysocket::end();
}
