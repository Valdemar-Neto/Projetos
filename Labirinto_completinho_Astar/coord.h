#ifndef _COORD_H_
#define _COORD_H_
#include <iostream>
#include <cmath>

/// As coordenadas de uma celula do mapa
struct Coord
{
public:
  int lin,col;

  /// Construtores
  Coord(): lin(-1), col(-1) {}
  Coord(int L, int C): lin(L), col(C) {}

  // Comparacao
  bool operator==(const Coord& C) const {return lin==C.lin && col==C.col;}
  bool operator!=(const Coord& C) const {return !operator==(C);}

  // Valor absoluto de uma coordenada
  Coord abs() const {return Coord(::abs(lin), ::abs(col));}

  // Modulo
  double modulo() const {return sqrt(lin*lin + col*col);}

  // Soma e subtracao
  Coord operator+(const Coord& C) const {return Coord(lin+C.lin, col+C.col);}
  Coord operator-(const Coord& C) const {return Coord(lin-C.lin, col-C.col);}

  // Produto escalar
  double operator*(const Coord& C) const {return lin*C.lin + col*C.col;}
};

/// Formas alternativas de uso de funcoes membros como funcoes classicas
inline Coord abs(const Coord& C) {return C.abs();}
inline double modulo(const Coord& C) {return C.modulo();}

/// Funcoes de E/S de coordenadas
std::istream& operator>>(std::istream& I, Coord& C);
std::ostream& operator<<(std::ostream& O, const Coord& C);

#endif // _COORD_H_
