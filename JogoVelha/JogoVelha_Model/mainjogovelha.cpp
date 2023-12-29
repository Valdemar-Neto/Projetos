#include "mainjogovelha.h"
#include "ui_mainjogovelha.h"

MainJogoVelha::MainJogoVelha(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainJogoVelha),
  JVModel(nullptr),
  NumX(nullptr),
  NumO(nullptr)
{
  ui->setupUi(this);

  // Cria o modelo da QTableView
  JVModel = new JogoVelhaModel(this);
  // Fixa o modelo da QTableView
  ui->jogoVelha->setModel(JVModel);

  // Conecta os sinais e slots
  connect(this, &MainJogoVelha::signalReiniciar,
          JVModel, &JogoVelhaModel::slotReiniciar);
  connect(this, &MainJogoVelha::signalFazerJogada,
          JVModel, &JogoVelhaModel::slotFazerJogada);
  connect(JVModel, &JogoVelhaModel::signalExibirSituacao,
          this, &MainJogoVelha::slotExibirSituacao);

  // Fixa as propriedades dos QLabel do jogador da vez e do vitorioso
  // Centralizado
  ui->imgVez->setAlignment(Qt::AlignCenter);
  ui->imgVencedor->setAlignment(Qt::AlignCenter);
  // Cor de fundo
  ui->imgVez->setStyleSheet("background: white");
  ui->imgVencedor->setStyleSheet("background: white");

  // Cria os widgets da barra de status
  NumX = new QLabel("-");
  NumO = new QLabel("-");

  // Insere os widgets da barra de status
  statusBar()->insertWidget(0,new QLabel("Jogador X: "));
  statusBar()->insertWidget(1,NumX);
  statusBar()->insertWidget(2,new QLabel("   Jogador O: "));
  statusBar()->insertWidget(3,NumO);

  // Reexibe toda a interface, enviando sinal de reiniciar para o modelo
  emit signalReiniciar();
}

void MainJogoVelha::slotExibirSituacao(char jogVez, char venc, int numX, int numO)
{
  // Imagem do jogador da vez
  ui->imgVez->setText(QString(jogVez));

  // Imagem do jogador vitorioso
  ui->imgVencedor->setText(QString(venc));

  // Numero de casas ocupadas por cada jogador
  NumX->setText(QString::number(numX));
  NumO->setText(QString::number(numO));
}

MainJogoVelha::~MainJogoVelha()
{
  delete ui;
}

void MainJogoVelha::on_actionSair_triggered()
{
  QCoreApplication::quit();
}

void MainJogoVelha::on_actionReiniciar_triggered()
{
  emit signalReiniciar();
}

void MainJogoVelha::on_jogoVelha_doubleClicked(const QModelIndex &index)
{
  emit signalFazerJogada(index.row(), index.column());
}

