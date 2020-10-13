#ifndef PERFORMANCESYSTEM_H
#define PERFORMANCESYSTEM_H

#include <QObject>
#include "player.h"

class PerformanceSystem : public QObject
{
    Q_OBJECT
public:
    explicit PerformanceSystem(QVector<QChar> initialboard,QVector<float> player1weightvector,
                                                    QVector<float> player2weightvector,
                                                    QChar player1symbol,
                                                    QChar player2symbol,
                                                    QObject *parent = nullptr);
    QVector<QChar> board;
    QVector<float> player1Weight;
    QVector<float> player2Weight;
    QChar          player1Symbol;
    QChar          player2Symbol;

    bool IsGameOver(QVector<QChar> board,QChar playerSymbol);
    QList<QVector<QChar>> GetGameHistory();

signals:

};

#endif // PERFORMANCESYSTEM_H
