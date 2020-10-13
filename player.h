#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QRandomGenerator>
#include "board.h"

  class Player : public QObject
{
    Q_OBJECT
    public:
      explicit Player(QChar playerSymbol,QVector<float> targetFunctionWeightVector, QObject *parent = nullptr);
      bool                  IsGameOver(QVector<QChar> board,QChar playerSymbol);
      QList<QVector<QChar>> GetLegalMoves(QVector<QChar> board,QChar playerSymbol);
      QVector<float>        GetFeatureVector(QVector<QChar> board,QChar player);
      QVector<float>        GetFeatureVector(QVector<QChar> board);
      float                 GetNonFinalBoardScore(QVector<float> weightVector,
                                  QVector<float> featureVector);
      QVector<QChar>        ChooseMove(QVector<QChar> board, QChar player1Symbol);
      QVector<QChar>        ChooseRandomMove(QVector<QChar> board,QChar playerSymbol);
      QChar _symbol;
      QVector<float> _weightVector;


};

#endif // PLAYER_H
