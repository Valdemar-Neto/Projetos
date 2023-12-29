#include <iostream>
#include <iomanip>
#include <chrono>
#include "labirinto.h"

using namespace std;

int main(void)
{
  Labirinto L;
  int opcao;

  do
  {
    cout << endl;
    L.imprimir();
    do
    {
      cout << endl
           << "1 - Gerar mapa  "
           << "2 - Ler mapa  "
           << "3 - Definir origem  "
           << "4 - Definir destino  "
           << "5 - Calcular caminho  "
           << "0 - Sair"
           << endl;
      cout << "OPCAO: ";
      cin >> opcao;
    } while (opcao<0 || opcao>5);

    switch(opcao)
    {
    case 1:
      L.gerar();
      break;
    case 2:
      {
        string arq;
        do
        {
          cout << "ARQUIVO (sem espacos): ";
          cin >> arq;
        } while (arq == "");
        if (!L.ler(arq))
        {
          cerr << "Erro na leitura do arquivo " << arq << endl;
        }
      }
      break;
    case 3:
      if (L.empty())
      {
        cerr << "O mapa estah vazio..." << endl;
      }
      else
      {
        Coord O;
        do
        {
          cout << "ORIGEM (lin col): ";
          cin >> O;
        } while (!L.setOrigem(O));
      }
      break;
    case 4:
      if (L.empty())
      {
        cerr << "O mapa estah vazio..." << endl;
      }
      else
      {
        Coord D;
        do
        {
          cout << "DESTINO (lin col): ";
          cin >> D;
        } while (!L.setDestino(D));
      }
      break;
    case 5:
      if (L.empty() || !L.origDestDefinidos())
      {
        cerr << "Mapa ou caminho indefinido..." << endl;
      }
      else
      {
        double comprCaminho, deltaT;
        int profCaminho,numA,numF;
        {
          // Calcula o tempo de execucao do calculo do caminho
          using namespace chrono;
          // Relogio antes da execucao
          steady_clock::time_point t1 = steady_clock::now();
          // Calcula o caminho
          comprCaminho = L.calculaCaminho(profCaminho, numA, numF);
          // Relogio depois da execucao
          steady_clock::time_point t2 = steady_clock::now();
          // Diferenca entre os dois instantes de tempo
          duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
          // Converte para milessegundos
          deltaT = 1000*time_span.count();
        }
        cout << "Tempo=" << deltaT << "ms\t"
             << "\t Num nos ABERTO=" << numA
             << "\t Num nos FECHADO=" << numF
             << endl;
        if (comprCaminho >= 0.0) cout << "Caminho encontrado!";
        else cout << "Nao existe caminho!";
        cout << setprecision(4) << fixed
             << "\t Comprimento=" << comprCaminho
             << "\t Profundidade=" << profCaminho
             << endl;
      }
      break;
    default:
      break;
    }
  } while (opcao != 0);
}
