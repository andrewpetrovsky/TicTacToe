#ifndef GAMEBUTTON_H
#define GAMEBUTTON_H

#include <QPushButton>

class GameButton : public QPushButton
{
public:
    GameButton(QString text,quint16 pos,QWidget* parent)
        : QPushButton(text,parent)
    {
        position = pos;
    }
    quint16 position;
};

#endif // GAMEBUTTON_H
