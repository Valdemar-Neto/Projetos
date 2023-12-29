#include <iostream>
#include "Poly.h"
#include <cmath>
#include  <fstream>
/*
Autor: Valdemar Gonçalves Pereira Neto

Calculadora de Polinomios
    Este é o primeiro trabalho referente a disciplina de Programação Avançada, dada pelo Prof. Dr. Adelardo Medeiros.
    Tem como principal objetivo usar os  conhecimentos obtidos em sala de aula durante a explanação do professor, uti-
    lizando boas técnicas de programação e implementar Programação Orientada  a Objeto.
*/
using namespace std;

// Construtor default
Poly::Poly(): grau(-1),a(nullptr){}

//Construtor por copia
Poly:: Poly(const Poly& P): grau(P.grau), a(nullptr){
    // Criando ao partir do construtor por copia um polinomio de grau
    if(grau>=0) {
        a = new double[P.grau + 1];
        for (int i = 0;i<=grau; ++i){
            a[i] = P.a[i];
        }
        return;
    }
}

// Construtor por movimento
 Poly:: Poly(Poly&& P): grau(P.grau), a(P.a){
    P.grau = -1;
    P.a = nullptr;
 }

 //Destrutor

 Poly:: ~Poly(){
    delete[ ] a;
 }

//Construtor especifico
Poly:: Poly(int g): grau(g), a(nullptr) {

    if (grau<0) grau=-1;
    else if(grau == 0){
        a = new double[1];
        a[0] = 0.0;
    }
    else {
        a = new double[grau+1];
        for (int i =0; i<=grau;++i){
            a[i] = 0.0;
        }
        a[grau] = 1.0;
    }
}

//Operador de atribuicao por copia
Poly& Poly::operator=(const Poly  &P) {

    if(this != &P){
                //Limpa conteudo anterior
                delete[ ] a;

                //Aloca a nova �rea
                grau = P.grau;

                // Condicao para a construcao do Polinomio
                if(grau>=0) a = new double[grau+1];
                // Se grau nao for maior que ), cria uma polinomio coeficientes correspondentes ao seus indices
                else a = nullptr;

                //copia dos dados
                for(int i = 0; i<=grau; ++i){
                    a[i] = P.a[i];
                }
    }
    return *this;
}

//Operador de atribuicao por movimento
Poly& Poly::operator=(Poly  &&P) {
    // Deleta a area antiga
    delete[ ] a;

    // Volta para o padrao estabelecido
    grau = -1;
    a = nullptr;

    swap(grau, P.grau);
    swap(a, P.a);

    return *this;
}

// M�todo que ir� retornar o valor do grau do polinomio
int Poly::getGrau() const {
    return grau;
}

// Metodo de consulta que ir� retornar o valor do i-esimo coeficiente do polinomio
double Poly::getCoef(int i) const  {
    if (i<0 || i > grau){
        return 0.0;
    };
    return a[i];
}

// Mettodo de alteracao
void Poly::setCoef(int i, double newCoef){
    if (i < 0 || i > grau) {
        cout << "ERRO: INDICE INVALIDO" << endl;
        return;
    }

    if (newCoef == 0 && i == grau &&  grau >0) {
        cout << "ERRO: COEFICIENTE DE MAIOR GRAU NAO PODE SER ZERO" <<endl;
        return;
    }

    a[i] = newCoef;
    return;
}

//AQUI INICIA O TESTE 2
// Metodo getcoef com sobrecarga de operador

double Poly:: operator [] (int i) const {
    return getCoef(i);
}

//Metodo recriar

void Poly:: recriar(int novoGrau){
   *this= Poly(novoGrau);
}

//Sobrecarga de operador de atribuição (==)
bool Poly:: operator==(const Poly &other) const {
    if (grau!=other.grau){
        return false;
    }

    for(int i=0; i<=grau; ++i){
        if(a[i] != other.a[i]) return false;
    }
    return true;
}

// Sobrecargaf do operador de atribuicao (!=)

bool Poly:: operator!=(const Poly &other) const{
    return !(*this ==other);
}

// Para polinomio vazios
bool Poly:: empty() const{
    if (grau < 0) return true;
    else return false;
}

// se o polinomio for de grau zero e seu unico coeficiente zero, retorna um polinomio nulo

bool Poly:: isZero() const {
    if (grau == 0 && a[grau] == 0.0) return true;
    else return false;
}

// Valor do Polinomio com x atribuido

double Poly:: getValor(double x) const{
    if (empty ()){
        return 0.0;
    }
    double Valor = 0.0;

    for (int i; i<= grau; ++i){
        Valor += a[i]*pow(x,i);
    }

    return Valor;
}

// valor do polinomio com sobrecarga de operador ()
double Poly:: operator()(double x) const{
    return getValor(x);
}

// impressao feito com base no algoritmo implementado pelo o professor  na especificacao do projeto
ostream& operator << (ostream& apr, const Poly &P){
     bool primeiro_termo= true;
    for (int i = P.getGrau(); i >= 0; i--) {
        double coef = P.getCoef(i);
        if (coef != 0.0) {
            if (!primeiro_termo) {
                if (coef > 0) apr << "+";
                else apr<< "-";
                coef = abs(coef);
            } else {
                primeiro_termo = false;
                if (coef == -1) {
                    if (i > 0) apr << "-";
                    coef = 1;
                }
            }
            if (i > 0) {
                if (coef != 1.0) apr<< coef << "*";
                apr<< "x";
            } else apr << coef;
            if (i > 1) apr << "^" << i;
        }
    }
    if (!P.empty() && primeiro_termo) apr << "0";
    return apr;
}


//AQUI COMECA O TESTE 3
//entrada de dados com sobrecarga de operador
istream& operator >> (istream& ent, Poly& P){
    if(P.empty()){
        cout<< "Polinomio Vazio" << endl;
        return ent;
    }

    double coef;

    for(int i =P.getGrau() ; i>=0;  i--){
            cout << "x^" << i<<":"<< endl;
            do
            {
                 ent >> coef;
                 if( i==P.getGrau() && coef == 0 && P.getGrau() >0){
                    cout <<"O coeficiente não pode ser nulo para esse  polinomio. Digite novamente, por gentileza!" << endl;
                 } else {
                     P.setCoef(i, coef);
                     break;
                }
            }
            while (true);
    }
    return ent;
}

// Metodo para salvar
bool Poly:: salvar(const string& nomedoarquivo )const{
    ofstream arquivo(nomedoarquivo);
    if(!arquivo.is_open()) return false;

    arquivo << "POLY  " << grau << endl;

    if(grau>=0){
        for(int i =0; i <=grau; i++) arquivo << a[i] << "  ";
        arquivo << endl;
    }
    arquivo.close();
    return true;
}

//AQUI COMECA O TESTE 4

//Metodo de Leitura
bool Poly::ler(const string&  nomearquivo) {

    ifstream arquivo(nomearquivo);

    if (!arquivo.is_open()) return false; // Falha ao abrir o arquivo

    string tag;
    arquivo >> tag;

    if (tag != "POLY") return false; // Formato inválido: falta a palavra reservada POLY

    int grau_lido;
    arquivo >> grau_lido;
    if(!arquivo.good())return false;

    if(grau_lido<0){
        *this = Poly();
        return true;
    }

    Poly prov(grau_lido);
    double new_coef;
        for (int i = 0; i <= grau_lido; i++) {

            arquivo >> new_coef;

            if (arquivo.good()) {
                if(new_coef == 0 && i == grau_lido &&  grau_lido >0)return false;
                prov.setCoef(i, new_coef);
            }
            else {
                return false;
            }
        }

    *this = prov;
    return true; // Leitura bem-sucedida
}

// AQUI COMECA O TESTE 5
//operador unariok
Poly Poly:: operator-() const {
    if(empty()) return Poly();
    if(isZero()) return Poly(0);

    Poly result(grau);

    for(int i=0; i<=grau; i++){
        result.setCoef(i, -a[i]);
    }
    return result;
}

// operador +

Poly Poly:: operator+(const Poly& P) const{
    // Essas são as primeiras condiçoes para o programa retornar o resultado da adiçao
    if(empty()) return P; // Se o primeiro polinomio for vazio, retorna retorna o segundo polinomio
    if (P.empty()) return *this; // Se o segundo polinomio for vazio, retorna ele mesmo.

    int novograu = max(grau, P.grau);
    Poly result(novograu);

    for(int i =0; i<=novograu; i++){
        result.a[i] = getCoef(i) + P.getCoef(i);
    }

    while(result.grau>0 && result.a[novograu] == 0.0){
        result.grau --;

    }

    if (result.grau != grau){
        Poly aux(result.grau);
        for (int i =0; i <= result.grau; i++){
            aux.setCoef(i, result.getCoef(i));
        }
        result = aux;
    }


    return result;
}

// Sobrecarga do operador (-) para a subtração do polinomio, utilizando o operador +, uma vez que o polinomio é passo usando operador unário para  a negação
Poly Poly:: operator-(const Poly& P) const{
    return *this + (-P);
}

//  AQUI COMECA O TESTE 6
// Sobrecarga do operador de multiplicação
Poly Poly:: operator*(const Poly& P) const {
    // Essas são as primeiras condiçoes para o programa retornar o resultado da multiplicação
    if(empty()) return Poly(); // Se  o primeiro for vazio, retorna vazio
    if(P.empty()) return Poly(); // Se  o segundoo for vazio, retorna vazio
    if(isZero()) return Poly(0); // Se  o primeiro for nulo, retorna vazio
    if(P.isZero()) return Poly(0); // Se  o primeiro for nulo, retorna vazio

    int novoGrau = grau+P.grau; //Em um multiplicacao de polinomios, o novo grau é a soma de seus respectivos coeficientes;

    Poly result(novoGrau); // Criação de um novo polinomio com o novo grau obtido

    //Esse é o laco responsável para a multiplicação do seus coeficientes
    for (int k = 0; k<=novoGrau; k++){
        result.a[k] = 0.0;
        for(int i = 0;  i <= grau; i++){
            for ( int j =0; j<=P.grau; j++){
                if(i + j==k){
                    result.a[k] += a[i]*P.a[j];
                }
            }
        }
    }

    return result;
}

