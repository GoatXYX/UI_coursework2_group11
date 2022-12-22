//
// Created by twak on 11/11/2019.
//

//Got rid of the timer

#ifndef CW2_THE_PLAYER_H
#define CW2_THE_PLAYER_H


#include <QApplication>
#include <QMediaPlayer>
#include "the_button.h"
#include <vector>
#include <QTimer>



using namespace std;

class ThePlayer : public QMediaPlayer {

    Q_OBJECT

private:
    vector<TheButtonInfo>* infos;
    vector<TheButton*>* buttons;

public:
    ThePlayer() : QMediaPlayer(NULL) {
        setVolume(20);
        connect (this, SIGNAL (stateChanged(QMediaPlayer::State)), this, SLOT (playStateChanged(QMediaPlayer::State)) );

    }

    // all buttons have been setup, store pointers here
    void setContent(vector<TheButton*>* b, vector<TheButtonInfo>* i);

private slots:

    void playStateChanged (QMediaPlayer::State ms);


public slots:

    void jumpTo (TheButtonInfo* button);

};

#endif //CW2_THE_PLAYER_H
