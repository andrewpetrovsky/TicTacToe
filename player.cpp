#include "player.h"

Player::Player(QChar playerSymbol,QVector<float> targetFunctionWeightVector,QObject *parent) : QObject(parent)
{
    _symbol = playerSymbol;
    _weightVector = targetFunctionWeightVector;
}



bool Player::IsGameOver(QVector<QChar> board,QChar playerSymbol)
{
    QChar winner = '-';
         //Check Horizontals
         if((board[0] == board[1]) && (board[1]== board[2]))
             winner = board[0];
         else if ((board[3] == board[4]) && (board[4] == board[5]))
             winner = board[3];
         else if((board[6] == board[7]) && (board[7] == board[8]))
             winner = board[6];
         //Check Verticals
         else if((board[0] == board[3]) && (board[3] == board[6]))
             winner = board[0];
         else if((board[1] == board[4]) && (board[4] == board[7]))
             winner = board[1];
         else if((board[2] == board[5]) && (board[5] == board[8]))
             winner = board[2];
         //Check Diagnols
         else if((board[0] == board[4]) && (board[4] == board[8]))
                 winner = board[0];
         else if((board[2] == board[4]) && (board[4] == board[6]))
                 winner = board[2];

         if(board.contains('-') && winner == '-')
             return false;
         else
             return true;
}
QList<QVector<QChar>> Player::GetLegalMoves(QVector<QChar> board,QChar playerSymbol)
{
    QList<QVector<QChar>> movesList;
    for (int i = 0;i < board.count();++i )
    {
        if(board.at(i) == '-')
        {   QVector<QChar> tempBoard = board;
            tempBoard[i] = playerSymbol;
            movesList.append(tempBoard);
        }
    }
    return movesList;
}

QVector<float> Player::GetFeatureVector(QVector<QChar> board)
{
    QVector<float> p1featureVector = GetFeatureVector(board,'x');
    QVector<float> p2featureVector = GetFeatureVector(board,'o');
    for (int i = 0; i < p1featureVector.count(); ++i)
    {
        p1featureVector[i] += p2featureVector.at(i);
    }
    p1featureVector[0] = 1;
    return p1featureVector;
}
QVector<float> Player::GetFeatureVector(QVector<QChar> board,QChar player)
{
    //w1 = player(x) has 1 marker in otherwise open column/row
    //w2 = player(o) has 1 marker in otherwise open column/row
    //w3 = player(x) has 2 markers in otherwise open column/row
    //w4 = player(o) has 2 markers in otherwise open column/row
    //w5 = player(x) has 3 markers in column/row
    //w6 = player(o) has 3 markers in column/row
    QChar blank('-');
    QVector<float> featureVector(7,0);
    int offset = 0;
    if(player == 'o')
        offset = 1;

    //Fill in row values
    for (int i = 0; i < 9; i+=3)
    {
        //w1 or w2 if 1/3 position in otherwise empty row
        if(((board[i] == player) && (board[i+1] == blank) && (board[i+2] == blank)) ||
            ((board[i+2] == player) && (board[i] == blank) && (board[i+1] == blank))/* ||
            ((board[i] == blank) && (board[i+1] == player) && (board[i+2] == blank))*/)
                featureVector[offset + 1] += .5;
        //w3 or w4 if 2/3 positions in otherwise empty row
        if(((board[i] == player) && (board[i+1] == player) && (board[i+2] == blank)) ||
            ((board[i+2] == player) && (board[i] == blank) && (board[i+1] == player)))
                featureVector[offset + 3] += 1;
        //w5 or w6 all positions filled row
        if((board[i] == player) && (board[i+1] == player) && (board[i+2] == player))
            featureVector[offset + 5] += 2;
    }

    //Fill in column values
    for (int i = 0; i < 3; ++i)
    {
        //w1 or w2 if 1/3 position in otherwise empty column
        if((((board[i] == player) && (board[i+3] == blank) && (board[i+6] == blank))) ||
            ((board[i+6] == player) && (board[i] == blank) && (board[i+3] == blank))/* ||
            ((board[i] == blank) && (board[i+3] == player) && (board[i+6] == blank))*/)
                featureVector[offset+1] += .5;
        //w3 or w4 if 2/3 position in otherwise empty column
        if((((board[i] == player) && (board[i+3] == player) && (board[i+6] == blank))) ||
            ((board[i+6] == player) && (board[i] == blank) && (board[i+3] == player)))
                featureVector[offset+3] += 1;
        //w5 or w6 all positions filled column
        if((board[i] == player) && (board[i+3] == player) && (board[i+6] == player))
            featureVector[offset + 5] += 2;
    }
    featureVector[0] = 1;
    return featureVector;
}
float Player::GetNonFinalBoardScore(QVector<float> weightVector, QVector<float> featureVector)
{
    if(weightVector.size() != featureVector.size())
        return -1;
    float temp = 0;
    for (int i = 0; i < weightVector.size(); ++i)
        temp += weightVector[i]*featureVector[i];

    return temp;
}

QVector<QChar> Player::ChooseMove(QVector<QChar> board,QChar player1Symbol)
{
    QList<QVector<QChar>> legalMovesList = GetLegalMoves(board,player1Symbol);
    QVector<float> weightScoreList;
    QList<QVector<float>> featureVectorList;
    float highScore = 0;
    int chosenMove = 0;
    for (int i = 0; i < legalMovesList.count(); ++i)
    {

     QVector<float> featureVector = GetFeatureVector(legalMovesList.at(i));
     float tempScore = GetNonFinalBoardScore(_weightVector,featureVector);
     if(tempScore > highScore)
     {
       highScore = tempScore;
       chosenMove = i;
     }
    }
    return legalMovesList.at(chosenMove);
}

QVector<QChar> Player::ChooseRandomMove(QVector<QChar> board, QChar playerSymbol)
{
/*
    int chance;
    if(playerSymbol == 'x')
        chance = 10;
    else
        chance = 50;
    if(QRandomGenerator::system()->bounded(0,100) > chance)
        return ChooseMove(board,playerSymbol);*/
    QList<QVector<QChar>> legalMovesList = GetLegalMoves(board,playerSymbol);
    if(legalMovesList.count() == 1)
        return legalMovesList.at(0);
    int randomNumber = QRandomGenerator::system()->bounded(0,legalMovesList.count()-1);
    return legalMovesList.at(randomNumber);
}
