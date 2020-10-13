#include "critic.h"

Critic::Critic(QList<QVector<QChar> > gameHistory, QObject *parent) : QObject(parent)
{
    _gameHistory = gameHistory;

}
QVector<float> Critic::GetFeatureVector(QVector<QChar> board)
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
QVector<float> Critic::GetFeatureVector(QVector<QChar> board,QChar player)
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
float Critic::GetNonFinalBoardScore(QVector<float> weightVector, QVector<float> featureVector)
{
    if(weightVector.size() != featureVector.size())
        return -1;
    float temp = 0;
    for (int i = 0; i < weightVector.size(); ++i)
        temp += weightVector[i]*featureVector[i];

    return temp;
}

int Critic::GetFinalBoardScore(QVector<QChar> board, QChar player1Symbol, QChar player2Symbol)
{
    QChar winner('-');
    isGameOver(board,&winner);
    if(winner == player1Symbol)
        return 100;
    else if(winner == player2Symbol)
        return -200;
    else
        return 300;

}
bool Critic::isGameOver(QVector<QChar> board,QChar *winner)
{
       /* //Check Horizontals
        if((board[0] == board[1] == board[2] == player) ||
           (board[3] == board[4] == board[5] == player) ||
           (board[6] == board[7] == board[8] == player) ||
        //Check Verticals
           (board[0] == board[3] == board[6] == player) ||
           (board[1] == board[4] == board[7] == player) ||
           (board[2] == board[5] == board[8] == player) ||
        //Check Diagnols
           (board[0] == board[4] == board[8] == player) ||
           (board[2] == board[4] == board[6] == player))
        //If not completetly filled
        {

            return true;

        }*/
        *winner = '-';
        //Check Horizontals
        if((board[0] == board[1]) && (board[1]== board[2]))
            *winner = board[0];
        else if ((board[3] == board[4]) && (board[4] == board[5]))
            *winner = board[3];
        else if((board[6] == board[7]) && (board[7] == board[8]))
            *winner = board[6];
        //Check Verticals
        else if((board[0] == board[3]) && (board[3] == board[6]))
            *winner = board[0];
        else if((board[1] == board[4]) && (board[4] == board[7]))
            *winner = board[1];
        else if((board[2] == board[5]) && (board[5] == board[8]))
            *winner = board[2];
        //Check Diagnols
        else if((board[0] == board[4]) && (board[4] == board[8]))
                *winner = board[0];
        else if((board[2] == board[4]) && (board[4] == board[6]))
                *winner = board[2];

        if(board.contains('-') && *winner == '-')
            return false;
        else
            return true;
}
QList<QPair<QVector<float>,float>> Critic::GetTrainingSamples(QVector<float> weightVector,QChar player1Symbol, QChar player2Symbol)
{
    QList<QPair<QVector<float>,float>> trainingExamples;
    for (int i = 0; i < _gameHistory.count()-1; ++i)
    {
        QVector<float> tempFeatureVector = GetFeatureVector(_gameHistory.at(i));
        float tempScore = GetNonFinalBoardScore(weightVector,tempFeatureVector);
        trainingExamples.append(QPair<QVector<float>,float>(tempFeatureVector,tempScore));
    }
    QVector<float> tempfinalFeatureVector = GetFeatureVector(_gameHistory.last());
    float tempFinalScore = GetFinalBoardScore(_gameHistory.last(),player1Symbol,player2Symbol);
    trainingExamples.append(QPair<QVector<float>,float>(tempfinalFeatureVector,tempFinalScore));

    return trainingExamples;

}
