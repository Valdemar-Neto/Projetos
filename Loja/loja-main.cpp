#include <iostream>
#include <string>
#include <limits>
#include "loja.h"

using namespace std;

int main()
{
  Loja X;
  Livro L;
  CD C;
  DVD D;
  string A;
  int opcao(0);
  int id(0);

  cout << "CONTROLE DE ACERVO\n";
  do
  {
    do{
      cout << endl;
      cout << "0 - Sair\n";
      cout << "1/-1 - Incluir/excluir livro\n";
      cout << "2/-2 - Incluir/excluir CD\n";
      cout << "3/-3 - Incluir/excluir DVD\n";
      cout << "4 - Imprimir acervo em tela\n";
      cout << "5 - Ler acervo de arquivo\n";
      cout << "6 - Salvar acervo em arquivo\n";
      cout << "Opcao: ";
      cin >> opcao;
    } while (opcao<-3 || opcao>6);
    cin.ignore();

    // Entrada de dados, de acordo com a opcao
    switch (opcao)
    {
    case -3:
    case -2:
    case -1:
      cout << "Id do objeto a excluir: ";
      cin >> id;
      break;
    case 1:
      cout << "LIVRO>>\n";
      cin >> L;
      break;
    case 2:
      cout << "CD>>\n";
      cin >> C;
      break;
    case 3:
      cout << "DVD>>\n";
      cin >> D;
      break;
    case 4:
    default:
      break;
    case 5:
    case 6:
      cout << "Nome do arquivo: ";
      getline(cin,A);
      break;
    }

    // Funcao a ser chamada, de acordo com a opcao
    switch (opcao)
    {
    case -3:
      if (!X.excluirDVD(id))
      {
        cerr << "DVD " << id << " nao existe!\n";
      }
      break;
    case -2:
      if (!X.excluirCD(id))
      {
        cerr << "CD " << id << " nao existe!\n";
      }
      break;
    case -1:
      if (!X.excluirLivro(id))
      {
        cerr << "Livro " << id << " nao existe!\n";
      }
      break;
    case 0:
    default:
      break;
    case 1:
      X.incluirLivro(L);
      break;
    case 2:
      X.incluirCD(C);
      break;
    case 3:
      X.incluirDVD(D);
      break;
    case 4:
      cout << endl;
      X.imprimir();
      break;
    case 5:
      if (!X.ler(A))
      {
        cerr << "Erro ao ler arquivo " << A << endl;
      }
      break;
    case 6:
      if (!X.salvar(A))
      {
        cerr << "Erro ao salvar arquivo " << A << endl;
      }
      break;
    }
  } while (opcao!=0);
}
