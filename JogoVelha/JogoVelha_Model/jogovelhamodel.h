#ifndef JOGOVELHAMODEL_H
#define JOGOVELHAMODEL_H

#include <QAbstractTableModel>
#include "jogovelha.h"

class JogoVelhaModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  explicit JogoVelhaModel(QObject *parent = nullptr);

  // Geracao dos dados principais da tabela
  // Funcionalidades obrigatorias
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  // Geracao dos cabecalhos da tabela
  // Poderia ser dispensada nesse exemplo jah que a tabela nao tem cabecalhos
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

public slots:
  void slotReiniciar();
  void slotFazerJogada(int I, int J);

signals:
  void signalExibirSituacao(char jogVez, char venc, int numX, int numO);

private:
  JogoVelha JV;
};

#endif // JOGOVELHAMODEL_H
