#include <QBrush>
#include "jogovelhamodel.h"

JogoVelhaModel::JogoVelhaModel(QObject *parent)
  : QAbstractTableModel(parent)
  , JV()
{
}

int JogoVelhaModel::rowCount(const QModelIndex &/*parent*/) const
{
  return 3;
}

int JogoVelhaModel::columnCount(const QModelIndex &/*parent*/) const
{
  return 3;
}

QVariant JogoVelhaModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  switch(role)
  {
  case Qt::DisplayRole:
    // Conteudo (texto)
    return QString( JV.at(index.row(),index.column()) );
  case Qt::TextAlignmentRole:
    // Alinhamento do texto
    return Qt::AlignCenter;
  case Qt::BackgroundRole:
    // cor de fundo
    return QBrush(QColorConstants::Svg::lightcyan);
  case Qt::FontRole:
  default:
    return QVariant();
  }

  // Nunca deve chegar aqui
  return QVariant();
}

QVariant JogoVelhaModel::headerData(int /*section*/, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal)
  {
    // Cabecalho horizontal
    switch(role)
    {
    case Qt::DisplayRole:
    case Qt::TextAlignmentRole:
    case Qt::BackgroundRole:
    case Qt::FontRole:
    default:
      return QVariant();
    }
  }
  else
  {
    // Cabecalho vertical
    switch(role)
    {
    case Qt::DisplayRole:
    case Qt::TextAlignmentRole:
    case Qt::BackgroundRole:
    case Qt::FontRole:
    default:
      return QVariant();
    }
  }

  // Nunca deve chegar aqui
  return QVariant();
}

void JogoVelhaModel::slotReiniciar()
{
  JV.reiniciar();
  emit dataChanged(createIndex(0,0), createIndex(2,2), {Qt::DisplayRole});
  emit signalExibirSituacao(JV.jogadorVez(), JV.vencedor(),
                            JV.numX(), JV.numO());
}

void JogoVelhaModel::slotFazerJogada(int I, int J)
{
  if (JV.jogadaValida(I,J))
  {
    JV.fazerJogada(I,J);
    QModelIndex cell_IJ = createIndex(I,J);
    emit dataChanged(cell_IJ, cell_IJ, {Qt::DisplayRole});
    emit signalExibirSituacao(JV.jogadorVez(), JV.vencedor(),
                              JV.numX(), JV.numO());
  }
}


