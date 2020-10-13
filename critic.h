#ifndef CRITIC_H
#define CRITIC_H

#include <QObject>

class Critic : public QObject
{
    Q_OBJECT
public:
    explicit Critic(QList<QVector<QChar>> gameHistory, QObject *parent = nullptr);
    QVector<float>        GetFeatureVector(QVector<QChar> board,QChar player);
    QVector<float>        GetFeatureVector(QVector<QChar> board);
    float                 GetNonFinalBoardScore(QVector<float> weightVector,
                                QVector<float> featureVector);
    int                   GetFinalBoardScore(QVector<QChar> board,QChar player1Symbol,QChar player2Symbol);
    bool isGameOver(QVector<QChar> board, QChar *winner);

    QList<QPair<QVector<float>, float>> GetTrainingSamples(QVector<float> weightVector, QChar player1Symbol, QChar player2Symbol);
private:
    QList<QVector<QChar>> _gameHistory;
};

#endif // CRITIC_H
