#ifndef _MAINJOGOVELHA_H
#define _MAINJOGOVELHA_H

#include <QMainWindow>
#include <QPixmap>
#include <QLabel>
#include "jogovelhamodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainJogoVelha; }
QT_END_NAMESPACE

class MainJogoVelha : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainJogoVelha(QWidget *parent = 0);
  ~MainJogoVelha();

private slots:
  // Exibe as imagens do jogador da vez e do vitorioso e
  // os numeros de casas ocupados por cada jogador
  void slotExibirSituacao(char jogVez, char venc, int numX, int numO);

  void on_actionSair_triggered();

  void on_actionReiniciar_triggered();

  void on_jogoVelha_doubleClicked(const QModelIndex &index);

signals:
  void signalReiniciar();
  void signalFazerJogada(int I, int J);

private:
  Ui::MainJogoVelha *ui;

  // Modelo da QTableView
  JogoVelhaModel *JVModel;

  // Normalmente os widgets Qt que vao ser inseridos em outros widgets sao criados
  // como ponteiros e alocados dinamicamente, para que possam ter associacao parental
  // com o widget principal

  // Os numeros de casas ocupadas na barra de status
  QLabel *NumX;
  QLabel *NumO;
};

#endif // _MAINJOGOVELHA_H
