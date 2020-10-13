#include "board.h"

Board::Board(QObject *parent) : QObject(parent)
{
    _board.clear();
    _board.fill(QChar('-'),9);
}



QList<QVector<QChar>> Board::GetLegalMoves(QChar player)
{
    QList<QVector<QChar>> movesList;
    for (int i = 0;i < 9;++i )
    {
        if(_board[i] != '-')
        {
            QVector<QChar> tempBoard = _board;
            tempBoard[i] = player;
            movesList.append(tempBoard);
        }
    }
    return movesList;
}

QVector<float> Board::GetFeatureVector()
{
    QVector<float> featureVector;

    featureVector = GetFeatureVector('x');
    featureVector += GetFeatureVector('o');
    featureVector[0] = 1;
    return featureVector;
}

// Returns a dot product of the two vectors
float Board::GetNonFinalBoardScore(QVector<float> weightVector, QVector<float> featureVector)
{
    if(weightVector.size() != featureVector.size())
        return -1;
    float temp = 0;
    for (int i = 0; i < weightVector.size(); ++i)
        temp += weightVector[i]*featureVector[i];

    return temp;
}


QVector<float> Board::GetFeatureVector(QChar player)
{
    //w1 = player(x) has 1 marker in otherwise open column/row
    //w2 = player(o) has 1 marker in otherwise open column/row
    //w3 = player(x) has 2 markers in otherwise open column/row
    //w4 = player(o) has 2 markers in otherwise open column/row
    //w5 = player(x) has 3 markers in column/row
    //w6 = player(o) has 3 markers in column/row
    QChar blank('-');
    QVector<float> featureVector;
    featureVector.fill(0,7);
    int offset = 0;
    if(player == 'o')
        offset = 1;

    //Fill in row values
    for (int i = 0; i < 9; i+=3)
    {
        //w1 or w2
        if((((_board[i] == player) && (_board[i+1] == _board[i+2] == blank))) ||
            ((_board[i+2] == player) && (_board[i] == _board[i+1] == blank)))
                featureVector[offset + 1] += 1;
        //w5 or w6
        if(_board[i] == _board[i+1] == _board[i+2] == player)
            featureVector[offset + 5] += 1;
    }

    //Fill in column values
    for (int i = 0; i < 3; ++i)
    {
        //w3 or w4
        if((((_board[i] == player) && (_board[i+3] == _board[i+6] == blank))) ||
            ((_board[i+6] == player) && (_board[i] == _board[i+3] == blank)))
                featureVector[offset+1] += 1;
    }
    return featureVector;
}
