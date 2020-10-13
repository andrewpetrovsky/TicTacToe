#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->trainingProgressBar->setValue(0);
    _computerPlayer = nullptr;
    _playerTurn = 0;
    _computerSymbol = 'x';
    _computerWeights.resize(7);
    _computerWeights.fill(0.5);
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::train(int epochs)
{
    int trainingGameCount = 0;
    QPair<QChar,QChar> playerSymbols('x','o');
    QPair<QVector<float>,QVector<float>> playerWeightVectors(_computerWeights,QVector<float>(7,0.5));

    ui->trainedLabel->setText("Training..");
    while(trainingGameCount != epochs)
    {
        QVector<QChar> board(9,'-');
        PerformanceSystem performanceSys(board,playerWeightVectors.first,playerWeightVectors.second,
                                         playerSymbols.first,playerSymbols.second);
        QList<QVector<QChar>> gameHistory = performanceSys.GetGameHistory();

        Critic critic(gameHistory);

        QList<QPair<QVector<float>,float>> p1TrngSamples = critic.GetTrainingSamples(playerWeightVectors.first,
                                                                                    playerSymbols.first,playerSymbols.second);
        QList<QPair<QVector<float>,float>> p2TrngSamples = critic.GetTrainingSamples(playerWeightVectors.second,
                                                                                    playerSymbols.first,playerSymbols.second);
        Generalizer gener(p1TrngSamples);
        playerWeightVectors.first = gener.UpdateWeightVector(playerWeightVectors.first,0.25);
        Generalizer gener2(p2TrngSamples);
        playerWeightVectors.second = gener2.UpdateWeightVector(playerWeightVectors.second,0.40);

        trainingGameCount++;
        float percent = (static_cast<float>(trainingGameCount)/epochs) * 100.0;

        ui->trainingProgressBar->setValue(percent);
        QApplication::processEvents();
    }
    _computerWeights = playerWeightVectors.first;
    ui->trainedLabel->setText("Trained!");

}


void MainWindow::on_pushButton_clicked()
{
    if(ui->epochsLineEdit->text().isEmpty())
        return;
    emit train(ui->epochsLineEdit->text().toUInt());
}
void MainWindow::GenerateNewBoard()
{
    _buttonList.clear();
    for (int i = 0; i < 9; ++i)
    {
        GameButton *tempButton = new GameButton(" ",i,this);
        tempButton->setFixedHeight(100);
        int row = i / 3;
        int column = i % 3;
        connect(tempButton,SIGNAL(clicked()),this,SLOT(PlayerTurn()));
        ui->boardLayout->addWidget(tempButton,row,column,1,1);
        _buttonList.append(tempButton);
    }
}

void MainWindow::on_startGameButton_clicked()
{
    _gameOver = false;
    if(_computerPlayer != nullptr)
        delete _computerPlayer;
    _computerPlayer = new Player(_computerSymbol,_computerWeights,this);
    _gameHistory.clear();
    _boardState.clear();
    _boardState.resize(9);
    _boardState.fill('-');
    GenerateNewBoard();
    ComputersTurn();
    /*
    QList<QVector<QChar>> test;
    Critic *myCritic = new Critic(test);
    //Winner horiz with open slot
    QVector<QChar> board = {'x','x','x',
                            'o','-','x',
                            'x','o','x'};
    //Winner vert with open slot
    QVector<QChar> board2 = {'x','o','x',
                            'x','-','x',
                            'x','o','x'};
    //Winner diag with open slot
    QVector<QChar> board3 = {'x','o','-',
                            '-','x','x',
                            'x','o','x'};
    //No winner no open slot
    QVector<QChar> board4 = {'x','x','o',
                            'o','o','x',
                            'x','o','x'};
    //Winner no open slots
    QVector<QChar> board5 = {'x','x','x',
                            'o','o','x',
                            'x','o','x'};

    //No Winner open slots
    QVector<QChar> board6 = {'x','-','-',
                            'o','o','x',
                            'x','o','x'};

    QChar winner = 't';
    if(myCritic->isGameOver(board,&winner))
        if(winner == 'x')
            ui->statusbar->showMessage(tr("Good"));

    if(myCritic->isGameOver(board2,&winner))
        if(winner == 'x')
            ui->statusbar->showMessage(tr("Good"));

    if(myCritic->isGameOver(board3,&winner))
        if(winner == 'x')
            ui->statusbar->showMessage(tr("Good"));

    if(myCritic->isGameOver(board4,&winner))
        if(winner == '-')
            ui->statusbar->showMessage(tr("Good"));

    if(myCritic->isGameOver(board5,&winner))
        if(winner == 'x')
            ui->statusbar->showMessage(tr("Good"));

    if(!myCritic->isGameOver(board6,&winner))
        if(winner == 'x')
            ui->statusbar->showMessage(tr("Good"));

*/
}

void MainWindow::ComputersTurn()
{
    QVector<QChar> tempBoard = _computerPlayer->ChooseMove(_boardState,_computerSymbol);
    for (int i = 0; i < tempBoard.count(); ++i)
    {
        if(tempBoard.at(i) != _boardState.at(i))
        {
            GameButton *button = _buttonList.at(i);
            QPalette pal = button->palette();
            pal.setColor(QPalette::Button,QColor(Qt::red));
            button->setAutoFillBackground(true);
            button->setPalette(pal);
            button->setText(_computerSymbol.toUpper());
            button->update();
            break;
        }
    }
    _boardState = tempBoard;
    _gameHistory.append(tempBoard);
    if(_computerPlayer->IsGameOver(_boardState,_computerSymbol))
    {
        ui->statusbar->showMessage(tr("AI WINS!!"));
        _gameOver = true;
    }
    else
        ui->statusbar->showMessage(tr("Your Turn..."));
}

void MainWindow::PlayerTurn()
{
    if(_gameOver)
        return;
    GameButton* button = reinterpret_cast<GameButton*>(QObject::sender());
    if(button->text() == _computerSymbol.toUpper() || button->text() == 'O')
        return;

    QPalette pal = button->palette();
    pal.setColor(QPalette::Button,QColor(Qt::green));
    button->setAutoFillBackground(true);
    button->setPalette(pal);
    button->setText("O");
    button->update();
    _boardState.replace(button->position,'o');
    _gameHistory.append(_boardState);
    if(_computerPlayer->IsGameOver(_boardState,'o'))
    {
        ui->statusbar->showMessage(tr("You WIN!!..yay"));
        UpdateWeights();
        _gameOver = true;
    }
    else
        ComputersTurn();
}

void MainWindow::UpdateWeights()
{
    Critic critic(_gameHistory);

    QList<QPair<QVector<float>,float>> p1TrngSamples = critic.GetTrainingSamples(_computerWeights,
                                                                                _computerSymbol,'o');
    Generalizer gener(p1TrngSamples);
    _computerWeights = gener.UpdateWeightVector(_computerWeights,.31345);
}
