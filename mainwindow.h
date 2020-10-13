#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "critic.h"
#include "generalizer.h"
#include "performancesystem.h"
#include "player.h"
#include "gamebutton.h"
#include "critic.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void train(int epochs);

    void UpdateWeights();
public slots:
    void ComputersTurn();
    void PlayerTurn();
private slots:
    void on_pushButton_clicked();
    void on_startGameButton_clicked();

private:
    Ui::MainWindow *ui;
    void GenerateNewBoard();
    QList<GameButton*> _buttonList;
    QList<QVector<QChar>> _gameHistory;
    QVector<QChar> _boardState;
    int _playerTurn;
    Player *_computerPlayer;
    QVector<float> _computerWeights;
    QChar _computerSymbol;
    bool _gameOver;
};
#endif // MAINWINDOW_H
