#ifndef POLY_H
#define POLY_H
#include <iostream>
#include <fstream>

using namespace std;


/*
Autor: Valdemar Gonçalves Pereira Neto

Calculadora de Polinomios
    Este é o primeiro trabalho referente a disciplina de Programação Avançada, dada pelo Prof. Dr. Adelardo Medeiros.
    Tem como principal objetivo usar os  conhecimentos obtidos em sala de aula durante a explanação do professor, uti-
    lizando boas técnicas de programação e implementar Programação Orientada  a Objeto.
*/
class Poly{
private:
    int grau;
    double *a;
public:

    // Construtor default com alocacao dinamica de memoria
    Poly();
    // Contrutor por cópia com alocacao dinamica de memoria
    Poly(const Poly& P);
    // Construtor por movimento com alocacao dinamica de memoria
    Poly(Poly&& P);
    // Explícito para conversao
    explicit Poly(int grau);
    // Destrutor
    ~Poly();
    // Operadores de atribuicao com sobrecarga de operador
    Poly &operator = (const Poly  &P); // Copia
    Poly &operator= (Poly  &&P); //Movimento

    //Metodo de consulta
    int getGrau() const ;//grau do polinomio
    double getCoef( int i ) const ; // coeficiente do polinomio

    // Metodo de alteracao
    void setCoef (int i, double newCoef);

    // Sobrecarga de operador para o metodo getcoef

    double operator [ ] (int i) const;

    //Recriar
    void recriar (int novoGrau);

    //Sobrecarga de operador ==

    bool operator== (const Poly &other) const;

    //Sobrecarga de operador

    bool operator!=(const Poly & other) const;

    //Empty

    bool empty() const;

    // Se o polinomio realmente é zero
    bool isZero() const;

    // Valor do polinomio com x atribuido

    double getValor(double x) const;

    //sobrecarga de operador para relacionar com getValor

    double operator()(double x) const;

    // Saída de dados
    friend ostream& operator<<(ostream& x, const Poly& P);

    // Entrada de dados
    friend istream& operator>>(istream& ent, Poly& P);

    //Salvar um arquivo
    bool salvar(const string& nomedoarquivo) const;

    // Ler um arquivo
    bool ler(const string& nome_arquivo);

    //Operador unário
    Poly operator-() const;

    // Operador +
    Poly operator+(const Poly& P) const;

    //Operator -

    Poly operator-(const Poly& P) const;

    //Operador *

    Poly operator*(const Poly& P) const;
};
#endif
