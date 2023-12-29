#include "coord.h"

/// Funcoes de E/S de coordenadas
std::istream& operator>>(std::istream& I, Coord& C)
{
  I >> C.lin >> C.col;
  return I;
}

std::ostream& operator<<(std::ostream& O, const Coord& C)
{
  O << C.lin << ';' << C.col;
  return O;
}
