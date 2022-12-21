#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <QMediaPlayer>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QImageReader>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QtWidgets>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QScrollArea>
#include "the_button.h"
#include "the_player.h"

using namespace std;

class Application: public QWidget
{
    Q_OBJECT

public:

    Application(vector<TheButtonInfo> videos);
// -------------------------------------------- change -------------------------------------
    void setScrollArea(QScrollArea *area){
        this->scrollArea=area;
    }
// -------------------------------------------- change -------------------------------------

private:
    void createWidgets();
    void createLayout();
    void autoPlay();
    void createConnections();
    void createThumbnails();
// -------------------------------------------- change -------------------------------------
    QScrollArea *scrollArea;
// -------------------------------------------- change -------------------------------------
    vector<TheButtonInfo> videos;
    QVideoWidget *videoWidget = new QVideoWidget;
    ThePlayer *player = new ThePlayer();
    QLabel* label = new QLabel("<h3></h3>");
    QLabel *volumeLabel = new QLabel;
    QStringList descriptions;
    QComboBox* filterBox = new QComboBox();
    QStringList elementsFilterBy;
    QLineEdit* searchField = new QLineEdit();

    //buttons and sliders
    QWidget *buttonWidget = new QWidget();
    vector<TheButton*> buttons;
//    QPushButton* fullScreenButton = new QPushButton();
    QPushButton* playPauseButton = new QPushButton;
    QSlider *volumeSlider = new QSlider(Qt::Horizontal);
    QSlider *slider;
    QPushButton* forward = new QPushButton;
    QPushButton* backward = new QPushButton;
    QPushButton* previous = new QPushButton;
    QPushButton* next = new QPushButton;
    QPushButton* search = new QPushButton;

//-------------------------------------change---------------------------------------------------------------------------
    QPushButton* full = new QPushButton;
    QLabel *time1 = new QLabel;
    QLabel *time2 = new QLabel;
    QPushButton* volume = new QPushButton;
//-------------------------------------change---------------------------------------------------------------------------


    //boolean values
    bool isVideoPlaying; //used for the play/pause button to check the status of the video
    bool isVideoFullScreen;
    bool isSilence;

    //pop up error messages for the missing functionality
    QMessageBox* messageNext = new QMessageBox();
    QMessageBox* messagePrev = new QMessageBox();
    QMessageBox* messageSearch = new QMessageBox();

    //thumbnails layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    QVBoxLayout *top = new QVBoxLayout();


//definitions of the functions used for the player's buttons
//they create the connection between the button and their functionality
private slots:
    void fullScreen();
    void playAndPause();
    void seekForward();
    void seekBackward();
//---------------------------------change----------------------------------------
    void SltMediaPlayerDurationChanged(qint64); // 媒体时长发生变化
    void SltMediaPlayerPositionChanged(qint64); // 媒体当前播放时间发生变化
    void Volume();
//---------------------------------change----------------------------------------
    void vidNext();
    void vidPrevious();
    void searchVideo();
};

#endif // APPLICATION_H
