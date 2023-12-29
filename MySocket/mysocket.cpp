#include <iostream>
#include <string.h>  // Para memset

#include "mysocket.h"

/* #############################################################
   ##  ATENCAO: VOCE DEVE DESCOMENTAR UM DOS BLOCOS ABAIXO    ##
   ##  PARA PODER COMPILAR NO WINDOWS OU NO LINUX             ##
   ############################################################# */

/// Descomente o bloco a seguir para compilar no Windows

///*

/// A funcao de inicializacao da biblioteca de sockets
mysocket_status mysocket::init()
{
  // All processes that call Winsock functions must first initialize the use of the Windows Sockets DLL (WSAStartup)
  // before making other Winsock functions calls
  // The MAKEWORD(2,2) parameter of WSAStartup makes a request for version 2.2 of Winsock on the system
  WSADATA wsaData;
  int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
  if (iResult==0) return mysocket_status::SOCK_OK;
  return mysocket_status::SOCK_ERROR;
}

/// A funcao de encerramento da biblioteca de sockets
void mysocket::end()
{
  //call WSACleanup when done using the Winsock dll
  WSACleanup();
}

/// A funcao para fechar um socket
static void myclosesocket(SOCKET x)
{
  closesocket(x);
}

//*/

/// Descomente o bloco a seguir para compilar no Linux

/*

// Os arquivos de inclusao
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

/// A funcao de inicializacao dos sockets
mysocket_status mysocket::init()
{
  return mysocket_status::SOCK_OK;
}

/// A funcao de encerramento da biblioteca de sockets
void mysocket::end()
{
}

/// A funcao para fechar um socket
static void myclosesocket(SOCKET x)
{
  close(x);
}

*/

/*********************************************
 A classe base mysocket
 *********************************************/

/// Construtor por default
mysocket::mysocket(): id(INVALID_SOCKET) {}

/// Fecha (destroi) o socket
void mysocket::close()
{
  if (id != INVALID_SOCKET)
  {
    myclosesocket(id);
  }
  id = INVALID_SOCKET;
}

/// Permuta dois sockets
/// Geralmente, deve ser utilizado ao inves do operador de atribuicao
void mysocket::swap(mysocket& S)
{
  SOCKET id_prov(id);
  id = S.id;
  S.id = id_prov;
}

/// Testa se um socket eh "virgem" ou foi fechado
bool mysocket::closed() const
{
  return id==INVALID_SOCKET;
}

/// Testa se um socket estah aberto (aceitando conexoes)
bool mysocket::accepting() const
{
  return !closed();
}

/// Testa se um socket estah conectado (pronto para ler e escrever)
bool mysocket::connected() const
{
  return !closed();
}

/// Impressao
std::ostream& operator<<(std::ostream& os, const mysocket& s)
{
  os << s.id;
  return os;
}

/*********************************************
 Sockets orientados a conexao (STREAM SOCKET)
 *********************************************/

// Sockets clientes

/// Construtor default
tcp_mysocket::tcp_mysocket(): mysocket() {}

/// Se conecta a um socket aberto
/// Soh pode ser usado em sockets "virgens" ou explicitamente fechados
/// Retorna mysocket_status::SOCK_OK, se tudo deu certo, ou mysocket_status::SOCK_ERROR
mysocket_status tcp_mysocket::connect(const std::string& name, const std::string& port)
{
  if (id!=INVALID_SOCKET)
  {
    return(mysocket_status::SOCK_ERROR);
  }

  // The getaddrinfo function is used to determine the values in the sockaddr structure:
  // The getaddrinfo function provides protocol-independent translation from an ANSI host name to an address.
  // Parameters:
  // pNodeName: A pointer to a nullptr-terminated ANSI string that contains a host name or a numeric host address as string
  //   nullptr = any
  // pServiceName: A pointer to a nullptr-terminated ANSI string that contains either a service name or port number as string
  //   DEFAULT_PORT ("27015") is the port number associated with the server that the client will connect to.
  // pHints: A pointer to an addrinfo structure that provides hints about the type of socket the caller supports.
  //   AF_INET is used to specify the IPv4 address family.
  //   SOCK_STREAM is used to specify a stream socket.
  //   IPPROTO_TCP is used to specify the TCP protocol .
  //   AI_PASSIVE flag indicates the caller intends to use the returned socket address structure in a call to the bind function.
  //     When the AI_PASSIVE flag is set and pNodeName parameter to the getaddrinfo function is a nullptr pointer,
  //     the IP address portion of the socket address structure is set to INADDR_ANY for IPv4 addresses or
  //     IN6ADDR_ANY_INIT for IPv6 addresses.
  // ppResult: A pointer to a linked list of one or more addrinfo structures that contains response information about the host.

  struct addrinfo hints, *result = nullptr;  // para getaddrinfo, bind
  int intResult;

  memset(&hints, 0, sizeof (hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  // Call the getaddrinfo function requesting the IP address for the server.
  intResult = getaddrinfo(name.c_str(), port.c_str(), &hints, &result);
  if (intResult != 0)
  {
    return mysocket_status::SOCK_ERROR;
  }

  // Create a SOCKET for the client to communicate with the server
  // Use the first IP address returned by the call to getaddrinfo that matched the address family, socket type, and protocol
  // specified in the hints parameter. In this example, a TCP stream socket was specified with a socket type of SOCK_STREAM
  // and a protocol of IPPROTO_TCP. The address family was left unspecified (AF_UNSPEC), so the returned IP address could be
  // either an IPv6 or IPv4 address for the server.
  id = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  if (id == INVALID_SOCKET)
  {
    freeaddrinfo(result);
    return mysocket_status::SOCK_ERROR;
  }

  // Connect to server.
  intResult = ::connect( id, result->ai_addr, (int)result->ai_addrlen);
  if (intResult != 0)
  {
    // Should really try the next address returned by getaddrinfo if the connect call failed
    // But for this simple example we just free the resources
    // returned by getaddrinfo and print an error message
    freeaddrinfo(result);
    close();
    return mysocket_status::SOCK_ERROR;
  }

  // Once the connect function is called, the address information returned by the getaddrinfo function is no longer needed.
  // The freeaddrinfo function is called to free the memory allocated by the getaddrinfo function for this address information.
  freeaddrinfo(result);

  return(mysocket_status::SOCK_OK);
}

/// Leh de um socket conectado
/// Soh pode ser usado em socket para o qual tenha sido feito um "connect" antes
/// Ou entao em um socket retornado pelo "accept" de um socket servidor
/// O ultimo parametro eh o tempo maximo (em milisegundos) para esperar
/// por dados; se for <0, que eh o default, espera indefinidamente.
/// Retorna:
/// - mysocket_status::SOCK_OK, em caso de sucesso;
/// - mysocket_status::SOCK_TIMEOUT, se retornou por timeout;
/// - mysocket_status::SOCK_DISCONNECTED, se a conexao foi fechada corretamente; ou
/// - mysocket_status::SOCK_ERROR, em caso de erro
mysocket_status tcp_mysocket::read(mybyte* dado, size_t len, long milisec) const
{
  if (!connected() || len<=0)
  {
    return(mysocket_status::SOCK_ERROR);
  }
  if (milisec>=0)
  {
    // Com timeout
    mysocket_queue f;
    f.include(*this);
    mysocket_status iResult=f.wait_read(milisec);
    if (iResult==mysocket_status::SOCK_ERROR ||
        iResult==mysocket_status::SOCK_TIMEOUT)
    {
      return iResult;
    }
  }

  int ultima_leitura,recebidos=0,falta_receber=len;
  do
  {
    // recv: receives data from a connected socket
    // Parameters:
    // s: The descriptor that identifies a connected socket.
    // buf: A pointer to the buffer to receive the incoming data.
    // len: The length, in bytes, of the buffer pointed to by the buf parameter.
    // flags: A set of flags that influences the behavior of this function.
    ultima_leitura = ::recv(id,(char*)dado,falta_receber,0);

    if ( ultima_leitura == 0 )
    {
      // Servidor desconectou
      return mysocket_status::SOCK_DISCONNECTED;
    }
    if ( ultima_leitura == SOCKET_ERROR )
    {
      // Deu erro
      return mysocket_status::SOCK_ERROR;
    }
    recebidos += ultima_leitura;
    falta_receber -= ultima_leitura;
    if (falta_receber > 0)
    {
      dado += ultima_leitura;
    }
  }
  while (falta_receber>0);
  return(mysocket_status::SOCK_OK);
}

/// Escreve em um socket conectado
/// Soh pode ser usado em socket para o qual tenha sido feito um "connect" antes
/// Ou entao em um socket retornado pelo "accept" de um socket servidor
/// Retorna:
/// - mysocket_status::SOCK_OK, em caso de sucesso;
/// - mysocket_status::SOCK_ERROR, em caso de erro
mysocket_status tcp_mysocket::write(const mybyte* dado, size_t len) const
{
  if (!connected())
  {
    return(mysocket_status::SOCK_ERROR);
  }
  if (len==0)
  {
    return(mysocket_status::SOCK_ERROR);
  }

  int ultimo_envio,enviados=0,falta_enviar=len;
  do
  {
    // send: sends data on a connected socket
    // Parameters:
    // s: The descriptor that identifies a connected socket.
    // buf: A pointer to a buffer containing the data to be transmitted.
    // len: length, in bytes, of the data in buffer pointed to by the buf parameter.
    // flags: A set of flags that specify the way in which the call is made.
    ultimo_envio = ::send(id, (char*)dado, falta_enviar, 0);

    if ( ultimo_envio == SOCKET_ERROR )
    {
      return mysocket_status::SOCK_ERROR;
    }
    enviados += ultimo_envio;
    falta_enviar -= ultimo_envio;
    if (falta_enviar > 0)
    {
      dado += ultimo_envio;
    }
  }
  while (falta_enviar>0);
  return(mysocket_status::SOCK_OK);
}

/// Leh um int32_t de um socket conectado
/// O ultimo parametro eh o tempo maximo (em milisegundos) para esperar
/// por dados; se for <0, que eh o default, espera indefinidamente.
/// Retorna:
/// - mysocket_status::SOCK_OK, em caso de sucesso;
/// - mysocket_status::SOCK_TIMEOUT, se retornou por timeout;
/// - mysocket_status::SOCK_DISCONNECTED, se a conexao foi fechada corretamente; ou
/// - mysocket_status::SOCK_ERROR, em caso de erro
mysocket_status tcp_mysocket::read_int(int32_t& num, long milisec) const
{
  return read((mybyte*)&num,sizeof(num),milisec); // sizeof(num) == 4
}

/// Escreve um int32_t em um socket conectado
/// Retorna:
/// - mysocket_status::SOCK_OK, em caso de sucesso;
/// - mysocket_status::SOCK_ERROR, em caso de erro
mysocket_status tcp_mysocket::write_int(const int32_t num) const
{
  return write((mybyte*)&num,sizeof(num)); // sizeof(num) == 4
}

/// Leh uma string de um socket conectado
/// Primeiro leh o numero de bytes da string (int32_T), depois os caracteres
/// O ultimo parametro eh o tempo maximo (em milisegundos) para esperar
/// por dados; se for <0, que eh o default, espera indefinidamente.
/// Retorna:
/// - mysocket_status::SOCK_OK, em caso de sucesso;
/// - mysocket_status::SOCK_TIMEOUT, se retornou por timeout;
/// - mysocket_status::SOCK_DISCONNECTED, se a conexao foi fechada corretamente; ou
/// - mysocket_status::SOCK_ERROR, em caso de erro
mysocket_status tcp_mysocket::read_string(std::string& msg, long milisec) const
{
  int32_t len;
  mysocket_status iResult;

  // Leh o numero de caracteres da string
  iResult = read_int(len,milisec);
  if (iResult==mysocket_status::SOCK_ERROR ||
      iResult==mysocket_status::SOCK_TIMEOUT ||
      iResult==mysocket_status::SOCK_DISCONNECTED)
  {
    // Erro ou timeout ou desconexao
    msg = "";
    return iResult;
  }
  msg.resize(len);
  // Escreve nos bytes da string
  char* buffer = (char*)msg.data();
  // Le os caracteres da string
  iResult = read((mybyte*)buffer,len,milisec);
  if (iResult==mysocket_status::SOCK_ERROR ||
      iResult==mysocket_status::SOCK_TIMEOUT ||
      iResult==mysocket_status::SOCK_DISCONNECTED)
  {
    // Erro ou timeout ou desconexao
    msg = "";
    return iResult;
  }
  return mysocket_status::SOCK_OK;
}

/// Escreve uma string em um socket conectado
/// Primeiro escreve o numero de bytes da string (int32_T), depois os caracteres
/// Retorna:
/// - mysocket_status::SOCK_OK, em caso de sucesso;
/// - mysocket_status::SOCK_ERROR, em caso de erro
mysocket_status tcp_mysocket::write_string(const std::string& msg) const
{
  int32_t len;
  mysocket_status iResult;

  len = msg.size();
  iResult = write_int(len);
  if (iResult == mysocket_status::SOCK_ERROR)
  {
    return mysocket_status::SOCK_ERROR;
  }
  iResult = write((mybyte*)msg.c_str(),len);
  if ( iResult == mysocket_status::SOCK_ERROR )
  {
    return mysocket_status::SOCK_ERROR;
  }
  return mysocket_status::SOCK_OK;
}

// Sockets servidores

/// Construtor default
tcp_mysocket_server::tcp_mysocket_server(): mysocket() {}

/// Abre um novo socket para esperar conexoes
/// Soh pode ser usado em sockets "virgens" ou explicitamente fechados
/// Retorna mysocket_status::SOCK_OK ou mysocket_status::SOCK_ERROR
mysocket_status tcp_mysocket_server::listen(const std::string& port, int nconex)
{
  if (id!=INVALID_SOCKET)
  {
    return(mysocket_status::SOCK_ERROR);
  }
  // Cria o socket

  // The getaddrinfo function is used to determine the values in the sockaddr structure:
  // The getaddrinfo function provides protocol-independent translation from an ANSI host name to an address.
  // Parameters:
  // pNodeName: A pointer to a nullptr-terminated ANSI string that contains a host name or a numeric host address as string
  //   nullptr = any
  // pServiceName: A pointer to a nullptr-terminated ANSI string that contains either a service name or port number as string
  //   DEFAULT_PORT ("27015") is the port number associated with the server that the client will connect to.
  // pHints: A pointer to an addrinfo structure that provides hints about the type of socket the caller supports.
  //   AF_INET is used to specify the IPv4 address family.
  //   SOCK_STREAM is used to specify a stream socket.
  //   IPPROTO_TCP is used to specify the TCP protocol .
  //   AI_PASSIVE flag indicates the caller intends to use the returned socket address structure in a call to the bind function.
  //     When the AI_PASSIVE flag is set and pNodeName parameter to the getaddrinfo function is a nullptr pointer,
  //     the IP address portion of the socket address structure is set to INADDR_ANY for IPv4 addresses or
  //     IN6ADDR_ANY_INIT for IPv6 addresses.
  // ppResult: A pointer to a linked list of one or more addrinfo structures that contains response information about the host.

  struct addrinfo hints, *result = nullptr;  // para getaddrinfo, bind
  int intResult;

  memset(&hints, 0, sizeof (hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  // Resolve the local address and port to be used by the server
  intResult = getaddrinfo(nullptr, port.c_str(), &hints, &result);
  if (intResult != 0)
  {
    return mysocket_status::SOCK_ERROR;
  }

  // Create a SOCKET for the server to listen for client connections
  // Use the first IP address returned by the call to getaddrinfo that matched the address family, socket type, and protocol
  // specified in the hints parameter
  id = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  if (id == INVALID_SOCKET)
  {
    freeaddrinfo(result);
    return mysocket_status::SOCK_ERROR;
  }

  // Atribuicao do nome do socket

  // For a server to accept client connections, it must be bound to a network address within the system.
  // Call the bind function, passing the created socket and sockaddr structure returned from the getaddrinfo function as parameters.
  // Setup the TCP listening socket
  intResult = bind( id, result->ai_addr, (int)result->ai_addrlen);
  if (intResult == SOCKET_ERROR)
  {
    freeaddrinfo(result);
    close();
    return mysocket_status::SOCK_ERROR;
  }

  // Once the bind function is called, the address information returned by the getaddrinfo function is no longer needed.
  // The freeaddrinfo function is called to free the memory allocated by the getaddrinfo function for this address information.
  freeaddrinfo(result);

  // After the socket is bound to an IP address and port on the system,
  // the server must then listen on that IP address and port for incoming connection requests
  // Parameters:
  // s: A descriptor identifying a bound, unconnected socket.
  // backlog: The maximum length of the queue of pending connections.
  //   If set to SOMAXCONN, will set the backlog to a maximum reasonable value.
  // the created socket and a value for the maximum length of the queue of pending connections to accept
  intResult = ::listen( id, nconex );
  if ( intResult == SOCKET_ERROR )
  {
    close();
    return mysocket_status::SOCK_ERROR;
  }

  return mysocket_status::SOCK_OK;
}

/// Aceita uma conexao que chegou em um socket aberto
/// Soh pode ser usado em socket para o qual tenha sido feito um "listen" antes
/// O socket "a" passado como parametro, em caso de sucesso, estarah conectado
/// (nao-conectado em caso de erro)
/// Retorna mysocket_status::SOCK_OK ou mysocket_status::SOCK_ERROR
mysocket_status tcp_mysocket_server::accept(tcp_mysocket& a) const
{
  if (!accepting())
  {
    return mysocket_status::SOCK_ERROR;
  }
  a.id = ::accept(id,nullptr,nullptr);
  if (a.id == INVALID_SOCKET)
  {
    return mysocket_status::SOCK_ERROR;
  }
  return mysocket_status::SOCK_OK;
}

/*********************************************
A CLASSE mysocket_queue (FILA DE SOCKETS)
*********************************************/

/// Construtor e destrutor
mysocket_queue::mysocket_queue()
{
  clear();
}
mysocket_queue::~mysocket_queue()
{
  clear();
}

/// Limpa a lista de sockets
void mysocket_queue::clear()
{
  nfds = 0;
  FD_ZERO(&set);
}

/// Adiciona um socket a uma fila de sockets
/// Retorna mysocket_status::SOCK_OK ou mysocket_status::SOCK_ERROR
mysocket_status mysocket_queue::include(const mysocket& a)
{
  if (nfds <= a.id)
  {
    nfds = a.id+1;
  }
  FD_SET(a.id,&set);
  return(mysocket_status::SOCK_OK);
}

/// Retira um socket de uma fila de sockets
/// Retorna mysocket_status::SOCK_OK ou mysocket_status::SOCK_ERROR
mysocket_status mysocket_queue::exclude(const mysocket& a)
{
  if (FD_ISSET(a.id,&set))
  {
    FD_CLR(a.id,&set);
    return(mysocket_status::SOCK_OK);
  }
  return(mysocket_status::SOCK_ERROR);
}

/// Bloqueia ateh haver alguma atividade de leitura em socket da fila
/// Retorna:
/// - mysocket_status::SOCK_OK, caso haja dados a serem lidos (sucesso);
/// - mysocket_status::SOCK_TIMEOUT, se retornou por timeout; ou
/// - mysocket_status::SOCK_ERROR, em caso de erro
mysocket_status mysocket_queue::wait_read(long milisec)
{
  int intResult;
  if (milisec >= 0)
  {
    struct timeval t;
    t.tv_sec = milisec/1000;
    t.tv_usec = 1000*(milisec - 1000*t.tv_sec);
    intResult = ::select(nfds, &set, nullptr, nullptr, &t);
  }
  else
  {
    intResult = ::select(nfds, &set, nullptr, nullptr, nullptr);
  }
  if (intResult<0) return mysocket_status::SOCK_ERROR;
  if (intResult==0) return mysocket_status::SOCK_TIMEOUT;
  return mysocket_status::SOCK_OK;
}

/// Bloqueia ateh haver alguma atividade de conexao em socket da fila
/// Retorna:
/// - mysocket_status::SOCK_OK, caso haja dados a serem lidos (sucesso);
/// - mysocket_status::SOCK_TIMEOUT, se retornou por timeout; ou
/// - mysocket_status::SOCK_ERROR, em caso de erro
mysocket_status mysocket_queue::wait_connect(long milisec)
{
  return wait_read(milisec);
}

/// Bloqueia ateh haver alguma atividade de escrita em socket da fila
/// Retorna:
/// - mysocket_status::SOCK_OK, caso haja dados a serem lidos (sucesso);
/// - mysocket_status::SOCK_TIMEOUT, se retornou por timeout; ou
/// - mysocket_status::SOCK_ERROR, em caso de erro
mysocket_status mysocket_queue::wait_write(long milisec)
{
  int intResult;
  if (milisec >= 0)
  {
    struct timeval t;
    t.tv_sec = milisec/1000;
    t.tv_usec = 1000*(milisec - 1000*t.tv_sec);
    intResult = ::select(nfds, nullptr, &set, nullptr, &t);
  }
  else
  {
    intResult = ::select(nfds, nullptr, &set, nullptr, nullptr);
  }
  if (intResult<0) return mysocket_status::SOCK_ERROR;
  if (intResult==0) return mysocket_status::SOCK_TIMEOUT;
  return mysocket_status::SOCK_OK;
}

// Testa se houve atividade em um socket especifico da fila
bool mysocket_queue::had_activity(const mysocket& a)
{
  return(FD_ISSET(a.id,&set));
}
