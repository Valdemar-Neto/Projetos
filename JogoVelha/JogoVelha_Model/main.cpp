#include "mainjogovelha.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainJogoVelha w;

  w.show();

  return a.exec();
}
