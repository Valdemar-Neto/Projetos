#include <iostream>
#include <string>

#include "Poly.h"

using namespace std;

/// Funcao auxiliar para imprimir informacao
/// sobre o grau do polinomio
string printGrau(const Poly& P)
{
  string prov;

  if (!P.empty()) prov=" ";
  prov += '[';
  if (P.empty()) prov += "Vazio";
  else if (P.isZero()) prov += "Nulo";
  else
  {
    prov += "Grau ";
    prov += to_string(P.getGrau());
  }
  prov += ']';
  return prov;
}

int main(void)
{
  Poly P1;
  Poly P2(0);
  Poly Result(1);
  double x;
  int grau;
  char opcao;

  cout << "CALCULADORA DE POLINOMIOS\n";
  // Leh os valores da execucao anterior
  // Se nao existirem os arquivos, nao faz nada
  P1.ler("poly_P1.txt");
  P2.ler("poly_P2.txt");
  Result.ler("poly_result.txt");
  do
  {
    // Solicita a opcao
    do
    {
      cout << endl;

      cout << "P1:     " << P1 << printGrau(P1) << endl;
      cout << "P2:     " << P2 << printGrau(P2) << endl;
      cout << "Result: " << Result << printGrau(Result) << endl;

      cout << "Digite sua opcao:\n";
      cout << "< - Entrar um novo polinomio\n";
      cout << "+ - Somar os polinomios\n";
      cout << "- - Subtrair os polinomios\n";
      cout << "* - Multiplicar os polinomios\n";
      // Opcionais
      /*
      cout << "/ - Dividir os polinomios (retornar quociente)\n";
      cout << "% - Dividir os polinomios (retornar resto)\n";
      */
      cout << "x - Calcular o ultimo polinomio (Result) para um valor de x\n";
      cout << "i - Inverter o sinal do ultimo polinomio (Result)\n";
      cout << "t - Trocar os polinomios (P1->P2 P2->Result Result->P1)\n";
      cout << "q - Terminar\n";
      cin >> opcao;
      opcao = tolower(opcao);
    }
    while (opcao!='<' && opcao!='+' && opcao!='-' && opcao!='*' &&
           // Opcionais
           /*
           opcao!='/' && opcao!='%' &&
           */
           opcao!='x' && opcao!='i' && opcao!='t' && opcao!='q');

    // Rotaciona os polinomios no buffer
    if (opcao!='i' && opcao!='q') // Nao rotaciona nada se for i,q
    {
      if (opcao=='t') // Rotaciona no sentido inverso
      {
        Poly prov(Result);
        Result = P2;
        P2 = P1;
        P1 = prov;
      }
      else // Todas as opcoes menos i,t,q: rotaciona normalmente
      {
        P1 = P2;
        P2 = Result;
        // Novo resultado entra em result
      }
    }

    // Executa a opcao escolhida
    switch(opcao)
    {
    case '<':
      cout << "Grau do polinomio: "  ;
      cin >> grau;
      Result.recriar(grau);
      if (grau >= 0)
      {
        cout << "Coeficientes do polinomio:\n";
        cin >> Result;
      }
      break;
    case '+':
      Result = P1+P2;
      break;
    case '-':
      Result = P1-P2;
      break;
    case '*':
      Result = P1*P2;
      break;

    // Opcionais
    /*
    case '/':
      Result = P1/P2;
      break;
    case '%':
      Result = P1%P2;
      break;
    */

    case 'x':
      cout << "Valor de x: ";
      cin >> x;
      Result.recriar(0);
      Result.setCoef(0,P2(x));
      break;
    case 'i':
      Result = -Result;
      break;
    case 't':
    case 'q':
      break;
    default:
      cerr << "Opcao desconhecida\n";
      Result=Poly();
      break;
    }
    P1.salvar("poly_P1.txt");
    P2.salvar("poly_P2.txt");
    Result.salvar("poly_result.txt");
  }
  while (opcao != 'q');

  return 0;
}
