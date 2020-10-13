#ifndef BOARD_H
#define BOARD_H

#include <QObject>

  class Board : public QObject
{
    Q_OBJECT
    public:
      explicit Board(QObject *parent = nullptr);
      ~Board(){};
      QVector<QChar>* GetBoard(){ return &_board;}
      Board* operator=(Board &b){ _board = b._board;
                                  return this;}
      void ClearBoard(){ _board.clear();}
      bool IsGameOver(QChar player, QChar* winner);
      QList<QVector<QChar>> GetLegalMoves(QChar player);
      QVector<float> GetFeatureVector(QChar player);
      QVector<float> GetFeatureVector();
      float GetNonFinalBoardScore(QVector<float> weightVector,
                                  QVector<float> featureVector);
      QVector<float> GetGameHistory();
      QVector<QChar> _board;


  signals:
};

#endif // BOARD_H
