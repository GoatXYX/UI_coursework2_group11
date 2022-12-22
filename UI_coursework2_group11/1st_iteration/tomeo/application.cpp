#include "application.h"


Application::Application(vector<TheButtonInfo> videos) : videos(videos){
    //windows' properties
    setWindowTitle("Tomeo");
    setMinimumSize(1200, 720);

    //functions used to create the application tomeo
    createWidgets();
    createThumbnails();
    createLayout();
    createConnections();
    fullScreen();
    Volume();
    autoPlay();
}

void Application::createWidgets(){
    //video description
    label->setAlignment(Qt::AlignLeft);
    label->hide();

    //full screen button
//    fullScreenButton->hide();
//    fullScreenButton->setBackgroundRole(QPalette::Light);

    // play/pause button
    playPauseButton->hide();
    playAndPause();

    //video timeline
    slider = new QSlider(Qt::Horizontal, this);
    slider->hide();

    //forward 10 seconds button
    forward->hide();
    forward->setIcon(QIcon(":/fast-forward.svg"));

    //go backwards 10 seconds button
    backward->hide();
    backward->setIcon(QIcon(":/rewind.svg"));

    //previous video button
    previous->hide();
    previous->setIcon(QIcon(":/back.svg"));

    //next video button
    next->hide();
    next->setIcon(QIcon(":/next.svg"));

//--------------------------change------------------------------------------------------------------
    full->hide();
//---------------------------change-----------------------------------------------------------------

    //search button
    search->setIcon(QIcon(":/magnifying-glass.svg"));
    search->setFixedHeight(40);
    searchField->setFixedHeight(40);
    filterBox->setFixedHeight(40);

    volume->hide();

    messageNext->setText("The 'next video' button is not currently working! The purpose of this button is to skip to the next video.");
    messageNext->hide();

    messagePrev->setText("The 'previous video' button is not currently working! The purpose of this button is to go to the previous video in the list");
    messagePrev->hide();

    messageSearch->setText("The 'search' button is not currently working! The purpose of this button is to search for videos that matched the specific filter chosen.");
    messageSearch->hide();

    //filter box elements
    elementsFilterBy<<"Filter by"<<"Country"<<"City"<<"Date";
    filterBox->setCurrentIndex(1);
    filterBox->addItems(elementsFilterBy);

    player->setVideoOutput(videoWidget);
    videoWidget->hide();

    descriptions<<"Video";
}

void Application::createThumbnails(){
    // a list of the buttons
    vector<TheButton*> buttons;
    // the buttons are arranged in a grid layout
    buttonWidget->setLayout(layout);
    int doubleDigits=1, increase=0;
    //create x buttons (for x no of videos)
    for ( unsigned i = 0; i < videos.size(); i++ ) {
        //creating a container layout inside the grid layout that contains the description and the video
        QLabel *description = new QLabel();
        if(i<9){
            description->setText(descriptions[0] + ' ' + (i + '0' + 1));
        }
        else{
            //doubleDigits is set to 1 and it remains constant representing the first number
            //increase represents the second digit and it increases after each loop
            description->setText(descriptions[0] + ' ' + (doubleDigits + '0') + ('0' + increase));
            increase++;
        }
        QFrame *container = new QFrame;
        container->setLayout(new QVBoxLayout);
        //creating the thumbnails for the videos
        TheButton *button = new TheButton(buttonWidget);
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo* ))); // when clicked, tell the player to play.
//        connect(button, SIGNAL(clicked()), this, SLOT(fullScreen()));
// -----------------------change 20 to 180-----------------------------------------------------
        button->setMinimumHeight(180);
// ----------------------------------------------------------------------------------------------
        buttons.push_back(button);
        description->setFixedHeight(30);
        description->setStyleSheet("font-size: 20px;height: 40px;width: 120px;");
        //adding the thumnails and the buttons to the container layout
        container->layout()->addWidget(button);
        container->layout()->addWidget(description);
        layout->addWidget(container);
        button->init(&videos.at(i));

    }
    // tell the player what buttons and videos are available
    player->setContent(&buttons, & videos);
}

//this function creates the connections between the buttons and their functionality
void Application::createConnections(){
//    connect(fullScreenButton, SIGNAL(clicked()), this, SLOT(fullScreen()));
    connect(playPauseButton, SIGNAL(clicked()), this, SLOT(playAndPause()));
    connect(player, &QMediaPlayer::durationChanged, slider, &QSlider::setMaximum);
    connect(player, &QMediaPlayer::positionChanged, slider, &QSlider::setValue);
    connect(slider, &QSlider::sliderMoved, player, &QMediaPlayer::setPosition);
    connect(forward, SIGNAL(clicked()), this, SLOT(seekForward()));
    connect(backward, SIGNAL(clicked()), this, SLOT(seekBackward()));
    connect(previous, SIGNAL(clicked()), this, SLOT(vidPrevious()));
    connect(next, SIGNAL(clicked()), this, SLOT(vidNext()));
    connect(search, SIGNAL(clicked()), this, SLOT(searchVideo()));
    connect(volumeSlider, &QSlider::valueChanged, player, &QMediaPlayer::setVolume);
//----------------------------------------------------------------------------------------------------------------
    connect(player, SIGNAL(durationChanged(qint64)),this,SLOT(SltMediaPlayerDurationChanged(qint64)));
    connect(player,SIGNAL(positionChanged(qint64)), this,SLOT(SltMediaPlayerPositionChanged(qint64)));
    connect(full, SIGNAL(clicked()), this, SLOT(fullScreen()));
    connect(volume, SIGNAL(clicked()), this, SLOT(Volume()));
//-----------------------------------------------------------------------------------------------------------------
}


void Application::createLayout(){
    //layout for the player's buttons
    QHBoxLayout* buttonsLayout = new QHBoxLayout();
//-------------------------------------------------------------------------------------------
    buttonsLayout->addWidget(time1);
    buttonsLayout->addWidget(time2);
//--------------------------------------------------------------------------------------------

//    buttonsLayout->addWidget(fullScreenButton);
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(previous);
    buttonsLayout->addWidget(backward);
    buttonsLayout->addWidget(playPauseButton);
    buttonsLayout->addWidget(forward);
    buttonsLayout->addWidget(next);


//-------------------------------------change---------------------------------------------------------------------------
    buttonsLayout->addStretch(1);
//-------------------------------------change---------------------------------------------------------------------------


    buttonsLayout->addWidget(volume); // voice
    buttonsLayout->addWidget(volumeSlider); //voice 
    buttonsLayout->addWidget(full);
    buttonsLayout->addStretch(); //positions the widgets on the left
    // ============================change=========================
    auto scrollArea = new QScrollArea(this);
    //Initializes the size of the scroll area
    scrollArea->setGeometry (QRect(0,0,1000,1000));
    scrollArea->verticalScrollBar();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(buttonWidget) ;
    scrollArea->setWidgetResizable(true);
    setScrollArea(scrollArea);
    // =====================================================

    //layout for the search bar, search button and filter combo box
    QHBoxLayout* searchLayout = new QHBoxLayout();
    searchLayout->addWidget(filterBox);
    searchLayout->addWidget(searchField);
    searchLayout->addWidget(search);

    QVBoxLayout* descriptionLayout = new QVBoxLayout();
// ------------------------change--------------------------
    descriptionLayout->addWidget(scrollArea);
// ------------------------change--------------------------

    QHBoxLayout* all = new QHBoxLayout;
    all->addLayout(descriptionLayout);
    top->addWidget(videoWidget);
    //"Location/Description" of the video playing
    top->addWidget(label);
    top->addWidget(slider);
    top->addLayout(buttonsLayout);
    all->addLayout(top);

    setLayout(all);
//    setLayout(scrollArea);

}


//full screen mode function
void Application::fullScreen() {
  // make videoWidget full screen
  if (isVideoFullScreen==false) {
      window()->showNormal();
      full->setIcon(QIcon(":/full.svg"));
      //Enables fullscreen mode with media buttons
      videoWidget->show();
      buttonWidget->show();
// ------------------------change--------------------------
      scrollArea->show();
// ------------------------change--------------------------
      videoWidget->setMaximumHeight(2000);
// ------------------------change--------------------------
      scrollArea->setMinimumWidth(300);
      scrollArea->setMaximumWidth(600);
// ------------------------change--------------------------
      isVideoFullScreen=true;
  //    fullScreenButton->setIcon(QIcon(":/list.svg"));
//      autoPlay();
      playPauseButton->setMaximumWidth(200);
      playPauseButton->show();
      forward->setMaximumWidth(70);
      forward->show();
      backward->setMaximumWidth(70);
      backward->show();
      slider->setMaximumWidth(2000);
      slider->show();
  //    fullScreenButton->setMaximumWidth(120);
  //    fullScreenButton->show();
      label->setMaximumWidth(200);
      label->setMaximumHeight(40);
      label->show();
      volume->setMaximumWidth(70);
      volume->show();
  //-------------------------change 250 to 150-------------------------------------------------------------------
      volumeSlider->setMinimumWidth(150);
  //------------------------------------------------------------------------------------------------------------
//      Volume();
      previous->setMaximumWidth(70);
      previous->show();
      next->setMaximumWidth(70);
      next->show();
      filterBox->hide();
      searchField->hide();
      search->hide();

  //----------------------------------change-------------------------------------------------------------
      full->setMaximumWidth(70);
      full->show();
      QFont font;
      font.setPixelSize(18); // 字体大小
      time1->setFont(font);
      time2->setFont(font);
      QPalette pal;
      pal.setColor(QPalette::WindowText, Qt::black);
      time1->setPalette(pal);
      time2->setPalette(pal);
      time1->setText("00:00"); // 播放时间显示
      time2->setText("/00:00");
      time1->show();
      time2->show();
  //----------------------------------change-------------------------------------------------------------
  }
  else {
    window()->showFullScreen();
    full->setIcon(QIcon(":/small.svg"));
    //Enables fullscreen mode with media buttons
    videoWidget->show();
    buttonWidget->hide();
    scrollArea->hide();
    videoWidget->setMaximumHeight(2000);
    isVideoFullScreen=false;
//    fullScreenButton->setIcon(QIcon(":/list.svg"));
//    autoPlay();
    playPauseButton->setMaximumWidth(200);
    playPauseButton->show();
    forward->setMaximumWidth(70);
    forward->show();
    backward->setMaximumWidth(70);
    backward->show();
    slider->setMaximumWidth(2000);
    slider->show();
//    fullScreenButton->setMaximumWidth(120);
//    fullScreenButton->show();
    label->setMaximumWidth(200);
    label->setMaximumHeight(40);
    label->show();
    volume->setMaximumWidth(70);
    volume->show();
//-------------------------change 250 to 150-------------------------------------------------------------------
    volumeSlider->setMinimumWidth(150);
//------------------------------------------------------------------------------------------------------------
//    Volume();
    previous->setMaximumWidth(70);
    previous->show();
    next->setMaximumWidth(70);
    next->show();
    filterBox->hide();
    searchField->hide();
    search->hide();

//----------------------------------change-------------------------------------------------------------
    full->setMaximumWidth(70);
    full->show();
    QFont font;
    font.setPixelSize(18); // 字体大小
    time1->setFont(font);
    time2->setFont(font);
    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::black);
    time1->setPalette(pal);
    time2->setPalette(pal);
    time1->setText("00:00"); // 播放时间显示
    time2->setText("/00:00");
    time1->show();
    time2->show();
//----------------------------------change-------------------------------------------------------------
  }
}

void Application::autoPlay() {
    playPauseButton->setIcon(QIcon(":/pause.svg"));
    player->play();
    isVideoPlaying = true;
}

// play/pause button connection
//we change the vlaue of the boolean variable isVideoPlaying
//in order to keep track of the video's status (played/paused)
void Application::playAndPause() {
  if (isVideoPlaying == false) {
    player->pause();
    playPauseButton->setIcon(QIcon(":/play-button.svg"));
    isVideoPlaying = true;
  }
  else {
    player->play();
    playPauseButton->setIcon(QIcon(":/pause.svg"));
    isVideoPlaying = false;
  }
}

//forward 2 seconds button connection
void Application::seekForward(){
    player->setPosition(round((double)slider->value() * 2 ));
}

//backward 2 seconds button connection
void Application::seekBackward(){
    player->setPosition(round((double)slider->value() / 2));
}


//--------------------------------------change---------------------------------------------------------------


void Application::SltMediaPlayerDurationChanged(qint64 duration)
{
    int second = duration / 1000;
    int minute = second / 60;
    second %= 60;

    QString mediaDuration;
    mediaDuration.clear();

    if (minute >= 10)
        mediaDuration = QString::number(minute, 10);
    else
        mediaDuration = "0" + QString::number(minute, 10);

    if (second >= 10)
        mediaDuration = mediaDuration + ":" + QString::number(second, 10);
    else
        mediaDuration = mediaDuration + ":0" + QString::number(second, 10);

    /* 显示媒体总长度时间 */
    time2->setText("/" + mediaDuration);
}
//媒体当前播放时间发生变化
void Application::SltMediaPlayerPositionChanged(qint64 position)
{
    int second = position / 1000;
    int minute = second / 60;
    second %= 60;

    QString mediaPosition;
    mediaPosition.clear();

    if (minute >= 10)
        mediaPosition = QString::number(minute, 10);
    else
        mediaPosition = "0" + QString::number(minute, 10);

    if (second >= 10)
        mediaPosition = mediaPosition + ":" + QString::number(second, 10);
    else
        mediaPosition = mediaPosition + ":0" + QString::number(second, 10);

    /* 显示现在播放的时间 */
    time1->setText(mediaPosition);
}

void Application::Volume(){
    if (isSilence == false) {
      player->setVolume(0);
      volumeSlider->show();
      volumeSlider->setRange(0, 100);
      volumeSlider->setValue(player->volume());
      volume->setIcon(QIcon(":/speaker.svg"));
      isSilence = true;
    }
    else {
      player->setVolume(0);
      volumeSlider->hide();
      volume->setIcon(QIcon(":/silence.svg"));
      isSilence = false;
    }
}
//--------------------------------------change---------------------------------------------------------------



//following 3 functions will display error messages when their corresponding button is clicked
void Application::vidNext(){
    messageNext->show();
}

void Application::vidPrevious(){
    messagePrev->show();
}
//shows the error message for the search button
void Application::searchVideo(){
    messageSearch->show();
}
