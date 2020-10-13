#include "performancesystem.h"


PerformanceSystem::PerformanceSystem(QVector<QChar> initialboard, QVector<float> player1weightvector,
                                     QVector<float> player2weightvector, QChar player1symbol,
                                     QChar player2symbol, QObject *parent)  : QObject(parent)

{
    board = initialboard;
    player1Weight = player1weightvector;
    player2Weight = player2weightvector;
    player1Symbol = player1symbol;
    player2Symbol = player2symbol;
}

bool PerformanceSystem::IsGameOver(QVector<QChar> board,QChar playerSymbol)
{
    /*
    for (int i = 0; i < 9; i+=3)
    {
        //Row check
        if((board[i] == playerSymbol) && (board[i+1] == playerSymbol) && (board[i+2] == playerSymbol))
                return true;
    }
    for (int i = 0; i < 3; ++i)
    {
        //Column check
        if((board[i] == playerSymbol) && (board[i+3] == playerSymbol) && (board[i+6] == playerSymbol))
            return true;
    }
    if(((board[0] == playerSymbol) && (board[4] == playerSymbol) && (board[8] == playerSymbol)) ||
       ((board[2] == playerSymbol) && (board[4] == playerSymbol) && (board[6] == playerSymbol)))
        return true;
    return false;*/
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

QList<QVector<QChar>> PerformanceSystem::GetGameHistory()
{
    QList<QVector<QChar>> gameHistory;
    bool gameNotOver = true;
    Player player1(player1Symbol,player1Weight);
    Player player2(player2Symbol,player2Weight);

    //Segfault
    QVector<QChar> tempBoard= board;

    while(gameNotOver)
    {
        //tempBoard = player1.ChooseRandomMove(tempBoard,player1Symbol);
        tempBoard = player1.ChooseMove(tempBoard,player1Symbol);
        gameHistory.append(tempBoard);
        if(IsGameOver(tempBoard,player1Symbol))
        {
            gameNotOver = false;
            break;
        }
        //tempBoard = player2.ChooseMove(tempBoard,player2Symbol);
        tempBoard = player2.ChooseRandomMove(tempBoard,player2Symbol);
        gameHistory.append(tempBoard);
        if(IsGameOver(tempBoard,player2Symbol))
        {
            gameNotOver = false;
            break;
        }
    }
    return gameHistory;

}
