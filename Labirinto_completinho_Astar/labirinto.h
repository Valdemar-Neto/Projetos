#ifndef _LABIRINTO_H_
#define _LABIRINTO_H_

#include <vector>
#include <list>
#include "coord.h"
#include <cmath>

#define LARGURA_PADRAO_MAPA 25
#define ALTURA_PADRAO_MAPA 10
#define PERC_PADRAO_OBST 0.35

using namespace std;

/// Os possiveis estados de uma celula do mapa
enum class EstadoCel
{
  LIVRE,
  OBSTACULO,
  ORIGEM,
  DESTINO,
  CAMINHO,
  INVALIDO
};

// Funcao para converter um estado de celula em uma string que o represente
std::string estadoCel2string(EstadoCel E);

class Noh{
public:
    Coord pos;
    Coord ant;
    double g;
    double h;

    Noh(Coord i_pos, Coord _ant, double g_past, double h_future): pos(i_pos), ant(_ant), g(g_past), h(h_future) {};

    ~Noh(){};

    double f() const { return g+h;};

      ///Calcula a heuristica da celula pela distancia euclidiana
  void cal_heuristica(const Coord& dest)  {
        double h_rot;

        if ( (pos.lin - dest.lin == 0) || (pos.col - dest.col == 0) || (pos.lin - dest.lin == pos.col-dest.col)) h_rot = 0.0;
        else h_rot = M_PI/8;

        h = sqrt(2)*min(abs(pos.lin -dest.lin), abs(pos.col - dest.col)) + abs(abs(pos.lin -dest.lin) - abs(pos.col - dest.col)) + h_rot;
    }


    bool operator==(const Noh& N) const {return (pos == N.pos);};
    bool operator==(const Coord& C) const {return (pos == C);};
    bool operator<(const Noh& N) const { return f() < N.f();};
    bool operator>(const Noh& N) const { return f() > N.f();};
};

/// A classe que armazena o mapa e os metodos de resolucao de labirintos
class Labirinto
{
private:
  /// Dimensoes do mapa
  /// NL = altura (numero de linhas)
  /// NC = largura (numero de colunas)
  int NLin, NCol;

  /// "Matriz" que contem os estados das casas do mapa.
  /// Na realidade, a "matriz" eh um vector de EstadoCel
  /// O acesso aos elementos da "matriz" se dah atraves dos metodos "set" e "at",
  /// que transformam os indices linha e coluna da matriz no indice do vetor:
  /// | 00 01 02 03 |
  /// | 10 11 12 13 |
  /// | 20 21 22 23 | -> 00 01 02 03 10 11 12 13 20 21 22 23
  std::vector<EstadoCel> mapa;
  list <Noh>ABERTO;
  list<Noh>FECHADO;

  /// A origem e o destino do caminho
  Coord orig, dest;

  /// Funcao set de alteracao de valor
  void set(int i, int j, EstadoCel valor);
  void set(const Coord& C, EstadoCel valor) {set(C.lin, C.col, valor);}

public:
  /// Cria um mapa vazio
  Labirinto(): NLin(0), NCol(0), mapa(), orig(), dest() {}

  /// Cria um mapa com dimensoes dadas
  /// numL e numC sao as dimensoes do labirinto
  Labirinto(int numL, int numC) {gerar(numL, numC);}

  /// Cria um mapa com o conteudo do arquivo nome_arq
  /// Caso nao consiga ler do arquivo, cria mapa vazio
  Labirinto(const std::string& nome_arq) {ler(nome_arq);}

  /// Destrutor (nao eh obrigatorio...)
  ~Labirinto() {}

  /// Torna o mapa vazio
  void clear();

  /// Limpa um eventual caminho anteriormente calculado
  void limpaCaminho();

  /// Funcoes de consulta
  int getNumLin() const {return NLin;}
  int getNumCol() const {return NCol;}
  Coord getOrig() const {return orig;}
  Coord getDest() const {return dest;}

  /// Retorna o estado da celula correspondente ao i-j-esimo elemento do mapa
  EstadoCel at(int i, int j) const;
  /// Retorna o estado da celula C
  EstadoCel at(const Coord& C) const {return at(C.lin, C.col);}

  /// Operador() de consulta - usa o metodo "at"
  /// Retorna o estado da celula correspondente ao i-j-esimo elemento do mapa
  EstadoCel operator()(int i, int j) const {return at(i,j);}
  /// Retorna o estado da celula C
  EstadoCel operator()(const Coord& C) const {return at(C);}

  /// Testa se um mapa estah vazio
  bool empty() const {return mapa.empty();}
  /// Testa se um mapa tem origem e destino definidos
  bool origDestDefinidos() const {return celulaValidaLivre(orig) &&
                                         celulaValidaLivre(dest);}

  /// Testa se uma celula eh valida dentro das dimensoes de um mapa
  bool coordValida(const Coord& C) const;
  /// Testa se uma celula eh valida e estah livre (nao eh obstaculo) em um mapa
  bool celulaValidaLivre(const Coord& C) const;
  /// Testa se um movimento MovDe->MovPara eh valido
  bool movimentoValido(const Coord& MovDe, const Coord& MovPara) const;

  /// Fixa a origem do caminho a ser encontrado
  bool setOrigem(const Coord& C);
  /// Fixa o destino do caminho a ser encontrado
  bool setDestino(const Coord& C);

  /// Imprime o mapa no console
  void imprimir() const;

  /// Leh um mapa do arquivo nome_arq
  /// Caso nao consiga ler do arquivo, cria mapa vazio
  /// Retorna true em caso de leitura bem sucedida
  bool ler(const std::string& nome_arq);
  /// Salva um mapa no arquivo nome_arq
  /// Retorna true em caso de escrita bem sucedida
  bool salvar(const std::string& nome_arq) const;

  /// Gera um novo mapa aleatorio
  /// numL e numC sao as dimensoes do labirinto
  /// perc_obst eh o percentual de casas ocupadas (obstaculos) no mapa.
  void gerar(int numL=ALTURA_PADRAO_MAPA, int numC=LARGURA_PADRAO_MAPA,
             double perc_obst=PERC_PADRAO_OBST);

  /// Calcula o caminho entre a origem e o destino do labirinto usando o algoritmo A*
  ///
  /// Retorna o comprimento do caminho (<0 se nao existe)
  ///
  /// O parametro prof retorna o numero de nos (profundidade) do caminho encontrado
  /// ou <0 caso nao exista caminho.
  ///
  /// O parametro NA retorna o numero de nos em aberto ao termino do algoritmo A*
  /// O parametro NF retorna o numero de nos em fechado ao termino do algoritmo A*
  /// Mesmo quando nao existe caminho, esses parametros devem ser retornados
  double calculaCaminho(int& prof, int& NA, int& NF);
};

#endif // _LABIRINTO_H_
