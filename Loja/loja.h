#ifndef _LOJA_H_
#define _LOJA_H_

#include <iostream>
#include <string>
#include <vector>

class Produto
{
private:
  std::string nome;
  double preco;
public:
  /// Construtor default
  Produto(const std::string& N="", double P=0);

  /// Funcoes de consulta
  std::string getNome() const;
  double getPreco() const;

  /// Metodos de entrada/saida
  void digitar();
  void imprimir() const;

  bool ler(std::istream& I);
  void salvar(std::ostream& O) const;
};

/// Operator >> e <<
std::istream& operator>>(std::istream& I, Produto& P);
std::ostream& operator<<(std::ostream& O, const Produto& P);

class Livro: public Produto /// Herdou de produto todos os atributos publicos, ou seja, nome e preco
{
private:
  std::string autor;
public:
  /// Construtor
  Livro(const std::string& N="", double P=0, const std::string& A="");

  /// Funcoes de consulta
  std::string getAutor() const;

  /// Funcoes de entrada/saida
  void digitar();
  void imprimir() const;
/// Essas funçoes precisam se implementadas, pois o ler e salvar do produto são diferentes para o livro,
/// logo é necessario criar uma função para cada classe que irá herdar algo de produto
  bool ler(std::istream& I);
  void salvar(std::ostream& O) const;
};

std::istream& operator>>(std::istream& I, Livro& L);
std::ostream& operator<<(std::ostream& O, const Livro& L);

class CD: public Produto
{
private:
  int nfaixas;
public:
  /// Construtor
  CD(const std::string& N="", double P=0, int NF=0);

  /// Funcoes de consulta
  int getNumFaixas() const;

  /// Funcoes de entrada/saida
  void digitar();
  void imprimir() const;

  bool ler(std::istream& I);
  void salvar(std::ostream& O) const;
};

std::istream& operator>>(std::istream& I, CD& C);
std::ostream& operator<<(std::ostream& O, const CD& C);

class DVD: public Produto
{
private:
  int duracao;
public:
  /// Construtor
  DVD(const std::string& N="", double P=0, int D=0);

  /// Funcoes de consulta
  int getDuracao() const;

  /// Funcoes de entrada/saida
  void digitar();
  void imprimir() const;

  bool ler(std::istream& I);
  void salvar(std::ostream& O) const;
};

std::istream& operator>>(std::istream& I, DVD& D);
std::ostream& operator<<(std::ostream& O, const DVD& D);

class Loja
{
private:
  std::vector<Livro> LL;
  std::vector<CD>    LC;
  std::vector<DVD>   LD;
public:
  /// Construtor
  Loja();

  /// Funcoes de consulta
  int getNumLivro() const;
  int getNumCD() const;
  int getNumDVD() const;
  Livro getLivro(int id) const; /// Chamou a função get livro da classe Livro o mesmo aconteceu para as duas proximas funcoes
  CD getCD(int id) const;
  DVD getDVD(int id) const;

  /// Funcoes de manipulacao (inclusao/exclusao) de itens do estoque
  void incluirLivro(const Livro& X);
  bool excluirLivro(int id);
  void incluirCD(const CD& X);
  bool excluirCD(int id);
  void incluirDVD(const DVD& X);
  bool excluirDVD(int id);

  /// Funcoes de entrada/saida
  bool ler(const std::string& arq);
  bool salvar(const std::string& arq) const;
  void imprimir() const;
};
#endif // _LOJA_H_
