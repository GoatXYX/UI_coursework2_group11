#include "playerwindow.h"
#include <QScrollBar>
#include <QMessageBox>
PlayerWindow::PlayerWindow()
{
    videoLayout();
    player = new QMediaPlayer(this);
    mlist = new QMediaPlaylist(this);
    player->setPlaylist(mlist);
    mlist->setPlaybackMode(QMediaPlaylist::Loop);
    player->setVolume(40);
    player->setVideoOutput(videoWidget);

    playerpushbutton[0]->setCheckable(true);
    playerpushbutton[4]->setCheckable(true);
//    连接信号
    connect_signal();
}

void PlayerWindow::connect_signal(){
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(play_or_not(QMediaPlayer::State)));
    connect(mlist, SIGNAL(currentIndexChanged(int)), this, SLOT(mlist_change(int)));
    connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(vlength_change(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(vposition_change(qint64)));

    connect(video_slider, SIGNAL(sliderReleased()), this, SLOT(video_slider_re()));
    connect(playerpushbutton[0], SIGNAL(clicked()), this, SLOT(play()));
//    volumn
    connect(volume_slider, SIGNAL(sliderReleased()), this, SLOT(volume_slider_re()));
    connect(playerpushbutton[4], SIGNAL(clicked()), this, SLOT(fullscreen()));

    connect(toppushbutton[0], SIGNAL(clicked()), this, SLOT(select_video()));

    connect(list, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(click(QListWidgetItem*)));

}

void PlayerWindow::play_or_not(QMediaPlayer::State state){
//    StoppedState
    if(state == 0) {
        playerpushbutton[0]->setChecked(false);
    }
//    PlayingState
    else if(state == 1){
        playerpushbutton[0]->setChecked(true);
    }
//    PausedState
    else if(state == 2){
        playerpushbutton[0]->setChecked(false);
    }
}

void PlayerWindow::mlist_change(int index){
    if (index == -1)
        return;
    list->setCurrentRow(index);
}

void PlayerWindow::vlength_change(qint64 d){
    video_slider->setRange(0, d/1000);
    int second  = (d/1000)%60;
    int minute = ((d/1000)/60)%60;
    int hour = ((d/1000)/60)/60;

    QString right;
    right = QString("%1:%2:%3").arg(hour, 2, 10, QLatin1Char('0')).arg(minute, 2, 10, QLatin1Char('0')).arg(second, 2, 10, QLatin1Char('0'));

    label[1]->setText("/" + right);
}

void PlayerWindow::vposition_change(qint64 now){
    if (!video_slider->isSliderDown())
        video_slider->setValue(now/1000);
    int second  = (now/1000)%60;
    int minute = ((now/1000)/60)%60;
    int hour = ((now/1000)/60)/60;

    QString left;
    left = QString("%1:%2:%3").arg(hour, 2, 10, QLatin1Char('0')).arg(minute, 2, 10, QLatin1Char('0')).arg(second, 2, 10, QLatin1Char('0'));

    label[0]->setText(left);
}

void PlayerWindow::video_slider_re()
{
    player->setPosition(video_slider->value() * 1000);
}

void PlayerWindow::play()
{
//    StoppedState
    if(player->state() == 0) {
        player->play();
    }
//    PlayingState
    else if(player->state() == 1){
        player->pause();
    }
//    PausedState
    else if(player->state() == 2){
        player->play();
    }
}

void PlayerWindow::volume_slider_re()
{
    player->setVolume(volume_slider->value());
}

void PlayerWindow::fullscreen()
{
    /* 全屏/非全屏操作 */
    vWidget[0]->setVisible(!playerpushbutton[4]->isChecked());
}

void PlayerWindow::select_video()
{
    QString OpenFile;
    OpenFile = QFileDialog::getOpenFileName(this, tr("open a file."), "D:/", tr("video files(*.avi *.mp4 *.wmv);;All files(*.*)"));
    if (!OpenFile.isEmpty()) {
        QFileInfo fileInfo = QFileInfo(OpenFile);
        fileName = QString::fromUtf8(fileInfo.fileName().toUtf8().data());
        filePath = QString::fromUtf8(OpenFile.toUtf8().data());
        QString imgfilepath = filePath.left(filePath.length() - 3);
        imgfilepath = imgfilepath + "png";
        if (mlist->addMedia(QUrl::fromLocalFile(filePath))) {
            QListWidgetItem *imageItem = new QListWidgetItem;
            imageItem->setIcon(QIcon(imgfilepath));
            imageItem->setText(fileName);
            imageItem->setSizeHint(QSize(250, 230));
            list->addItem(imageItem);
        }
    }
}

void PlayerWindow::rename_video(){
    player->stop();
    mlist->setCurrentIndex(item);
    QString filepath = mlist->currentMedia().request().url().toLocalFile();

    QFileInfo fileInfo = QFileInfo(filepath);
    QString filename = QString::fromUtf8(fileInfo.fileName().toUtf8().data());

    QString suffix = filepath.right(3);
    QString path = filepath.left(filepath.length() - filename.length());

    QString imgpath = filepath.left(filepath.length() - 3);
    imgpath = imgpath + "png";

    bool Ok = false;
    QString sName = QInputDialog::getText(NULL, "re", "Please rename the file", QLineEdit::Normal, filename.left(filename.length()-4), &Ok);

    if (Ok && !sName.isEmpty()) {
        mlist->removeMedia(item);
        list->takeItem(item);

        QString name = path + sName + "." + suffix;
        QString imgname = path + sName + ".png";
        QFile file(filepath);
        bool re_ok = file.rename(name);
        QFile imgfile(imgpath);
        bool reimg_ok = imgfile.rename(imgname);

        if(re_ok && reimg_ok)
        {
            mlist->insertMedia(item, QUrl::fromLocalFile(name));
            QListWidgetItem *i = new QListWidgetItem();
            i->setIcon(QIcon(imgname));
            i->setText(name.right(sName.length()+4));
            i->setSizeHint(QSize(250, 230));
            list->insertItem(item, i);
        }
    }
}


bool PlayerWindow::find(QVector<QVector<int>> a, int target){
    bool flag = 0;
    for(int i=0; i<a.length(); i++){
        for(int j=0; j<a[i].length(); j++){
            if(a[i][j] == target){
                flag = 1;
            }
        }
    }
    return flag;
}

void PlayerWindow::sort_top(QAction *action)
{
    if(action == act_top_loc)
    {      
    }
    else if(action == act_top_t)
    {
    }
}
void PlayerWindow::click(QListWidgetItem *item)
{
    player->stop();
    mlist->setCurrentIndex(list->row(item));
    player->play();
}



void PlayerWindow::contextMenuEvent ( QContextMenuEvent * event )
{
    int x = this->mapFromGlobal(QCursor().pos()).x();
    int y = this->mapFromGlobal(QCursor().pos()).y();

    popMenu = new QMenu(list);
    video = new QAction("select video", list);
    connect(video, SIGNAL(triggered()), this, SLOT(select_video()));
    re_video = new QAction("rename video", list);
    connect(re_video, SIGNAL(triggered()), this, SLOT(rename_video()));
    de_video = new QAction("delete video", list);


    if(list->itemAt(mapFromGlobal(QCursor::pos())) != NULL)
    {
        item = list->row(list->itemAt(mapFromGlobal(QCursor::pos())));
        qDebug()<<list->row(list->itemAt(mapFromGlobal(QCursor::pos())));
        popMenu->addAction(re_video);
    }
    if(x>300 && y>0 && y<this->height()-80){
        popMenu->addAction(video);
    }

    popMenu->exec(QCursor::pos());
}

void PlayerWindow::sortMenu(){
    menu = new QMenu(list);
    menu->setObjectName("menu_sort");
    act_top_loc      = new QAction("By Tag", list);
    act_top_t   = new QAction("By Time", list);
    menu->addAction(act_top_loc);
    menu->addAction(act_top_t);
    menu->addSeparator();

    menu->setWindowFlags(menu->windowFlags() | Qt::FramelessWindowHint);
    menu->setAttribute(Qt::WA_TranslucentBackground);
    menu->setStyleSheet(" QMenu {border-radius:5px;font-family:'Microsoft Yahei';font-size:14px;color:#fff;}"
                            " QMenu::item {height:30px; width:100px;padding-left:20px;border: 1px solid none;}"
                            "QMenu::item:selected {background-color:rgb(0,120,215);\
                            padding-left:20px;border: 1px solid rgb(65,173,255);}");

    toppushbutton[4]->setMenu(menu);
    connect(menu, SIGNAL(triggered()), this, SLOT(sort_top()));
}

void PlayerWindow::videoLayout()
{
//    this->setGeometry(0, 0, 800, 480);
    this->setMinimumSize(1000, 600);
    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::white);

    for (int i = 0; i < 4; i++) {
        /* 水平容器 */
        hWidget[i] = new QWidget();
        hWidget[i]->setAutoFillBackground(true);
        /* 水平布局 */
        hBoxLayout[i] = new QHBoxLayout();
    }

    for (int i = 0; i < 3; i++) {
        /* 垂直容器 */
        vWidget[i] = new QWidget();
        vWidget[i]->setAutoFillBackground(true);
        /* 垂直布局 */
        vBoxLayout[i] = new QVBoxLayout();
    }

    for (int i = 0; i < 2; i++) {
        label[i] = new QLabel();
    }

    for (int i = 0; i < 6; i++) {
        playerpushbutton[i] = new QPushButton();
        playerpushbutton[i]->setMaximumSize(44, 44);
        playerpushbutton[i]->setMinimumSize(44, 44);
    }

    for (int i = 0; i < 4; i++) {
        toppushbutton[i] = new QPushButton();
        toppushbutton[i]->setMinimumSize(129, 44);
        toppushbutton[i]->setObjectName("btn_video");
    }
    toppushbutton[4] = new QPushButton();
    toppushbutton[4]->setMinimumSize(44, 44);
    toppushbutton[4]->setMaximumSize(44, 44);

    /* 设置 */
    vWidget[0]->setObjectName("vWidget0");
    vWidget[1]->setObjectName("vWidget1");
    hWidget[1]->setObjectName("hWidget1");
    hWidget[2]->setObjectName("hWidget2");
    hWidget[3]->setObjectName("hWidget3");
    playerpushbutton[0]->setObjectName("btn_play");
    playerpushbutton[1]->setObjectName("btn_next");
    playerpushbutton[2]->setObjectName("btn_volumedown");
    playerpushbutton[3]->setObjectName("btn_volumeup");
    playerpushbutton[4]->setObjectName("btn_screen");
    playerpushbutton[5]->setObjectName("btn_set");
    toppushbutton[4]->setObjectName("sort");


    QFont font;

    font.setPixelSize(18);
    label[0]->setFont(font);
    label[1]->setFont(font);

    pal.setColor(QPalette::WindowText, Qt::white);
    label[0]->setPalette(pal);
    label[1]->setPalette(pal);


    for(int i=0; i<5; i++){
        toppushbutton[i]->setFont(font);
        toppushbutton[i]->setPalette(pal);
    }
    label[0]->setText("00:00:00");
    label[1]->setText("/00:00:00");
    toppushbutton[0]->setText("Import");
    toppushbutton[1]->setText("Tag Loc");
    toppushbutton[2]->setText("Tag Time");
    toppushbutton[3]->setText("Add Sub");


    video_slider = new QSlider(Qt::Horizontal);
    video_slider->setMaximumHeight(15);
    video_slider->setObjectName("durationSlider");

    volume_slider = new QSlider(Qt::Horizontal);
    volume_slider->setRange(0, 100);
    volume_slider->setMaximumWidth(80);
    volume_slider->setObjectName("volumeSlider");
    volume_slider->setValue(50);

    list = new QListWidget();
    list->setViewMode(QListWidget::IconMode);//显示模式
    list->setIconSize(QSize(250, 250));//设置图片大小
    list->setSpacing(10);//间距
    list->setResizeMode(QListView::Adjust);//适应布局调整
    list->setMovement(QListView::Static);

    list->setObjectName("listWidget");
    list->verticalScrollBar()->setStyleSheet(QString::fromUtf8(
            "QScrollBar{background-color:grey; width:14px; margin:1px 2px 1px 2px;}"   //sliding area
            "QScrollBar::handle{background-color:white; width:10px; border-radius:5px;}"  //sliding block
            "QScrollBar::handle:hover{background:rgb(61,65,81);}"
            "QScrollBar::add-line,QScrollBar::sub-line{border:none;}"    //up arrow and
    ));
    list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //listWidget->setFocusPolicy(Qt::NoFocus);
    videoWidget = new QVideoWidget();
    videoWidget->setStyleSheet("border-image: none;"
                               "background: transparent;"
                               "border:none");
    sortMenu();

    /* H0布局 */
    vWidget[0]->setMinimumSize(300, 480);
    vWidget[0]->setMaximumWidth(300);
    videoWidget->setMinimumSize(500, 480);

    hBoxLayout[0]->addWidget(vWidget[0]);
    hBoxLayout[0]->addWidget(videoWidget);

    hWidget[0]->setLayout(hBoxLayout[0]);
    hWidget[0]->setParent(this);
    hWidget[0]->setGeometry(0, 65, this->width(), this->height()-65);
    hBoxLayout[0]->setContentsMargins(0, 0, 0, 0);

    takeCentralWidget();
    setCentralWidget(hWidget[0]);


    /* V0布局 */
    QSpacerItem *vSpacer0 = new
            QSpacerItem(0, 80,
                        QSizePolicy::Minimum,
                        QSizePolicy::Maximum);
    vBoxLayout[0]->addWidget(list);
    vBoxLayout[0]->addSpacerItem(vSpacer0);
    vBoxLayout[0]->setContentsMargins(0, 0, 0, 0);

    vWidget[0]->setLayout(vBoxLayout[0]);

    /* V1布局 */
    /* 底板部件布局 */
    hWidget[1]->setMaximumHeight(15);
    hWidget[2]->setMinimumHeight(65);
    vBoxLayout[1]->addWidget(hWidget[1]);
    vBoxLayout[1]->addWidget(hWidget[2]);
    vBoxLayout[1]->setAlignment(Qt::AlignCenter);

    vWidget[1]->setLayout(vBoxLayout[1]);
    vWidget[1]->setParent(this);
    vWidget[1]->setGeometry(0, this->height() - 80, this->width(), 80);
    vBoxLayout[1]->setContentsMargins(0, 0, 0, 0);
    /* 位于最上层 */
    vWidget[1]->raise();

    /* H1布局 */
    hBoxLayout[1]->addWidget(video_slider);
    hBoxLayout[1]->setContentsMargins(0, 0, 0, 0);
    hWidget[1]->setLayout(hBoxLayout[1]);

    /* H2布局 */
    QSpacerItem *hSpacer0 = new
            QSpacerItem(300, 30,
                        QSizePolicy::Expanding,
                        QSizePolicy::Maximum);

    hBoxLayout[2]->addSpacing(20);
    hBoxLayout[2]->addWidget(playerpushbutton[0]);
    hBoxLayout[2]->addSpacing(10);
    hBoxLayout[2]->addWidget(playerpushbutton[1]);
    hBoxLayout[2]->addSpacing(10);
    hBoxLayout[2]->addWidget(label[0]);
    hBoxLayout[2]->addWidget(label[1]);
    hBoxLayout[2]->addSpacerItem(hSpacer0);
    hBoxLayout[2]->addSpacing(10);
    hBoxLayout[2]->addWidget(playerpushbutton[2]);
    hBoxLayout[2]->addWidget(volume_slider);
    hBoxLayout[2]->addWidget(playerpushbutton[3]);
    hBoxLayout[2]->addSpacing(20);
    hBoxLayout[2]->addWidget(playerpushbutton[4]);
    hBoxLayout[2]->addSpacing(20);
    hBoxLayout[2]->addWidget(playerpushbutton[5]);
    hBoxLayout[2]->addSpacing(20);
    hBoxLayout[2]->setContentsMargins(0, 0, 0, 0);
    hBoxLayout[2]->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    hWidget[2]->setLayout(hBoxLayout[2]);

    /* H3布局 */
    hWidget[3]->setMinimumHeight(65);
    vBoxLayout[2]->addWidget(hWidget[3]);
    vWidget[2]->setLayout(vBoxLayout[2]);
    vWidget[2]->setParent(this);
    vWidget[2]->setGeometry(0, 0, this->width()+20, 65);
    vBoxLayout[2]->setContentsMargins(0, 0, 0, 0);

    QSpacerItem *hSpacer1 = new
            QSpacerItem(300, 20,
                        QSizePolicy::Expanding,
                        QSizePolicy::Maximum);

    hBoxLayout[3]->addSpacing(20);
    hBoxLayout[3]->addWidget(toppushbutton[0]);
    hBoxLayout[3]->addSpacing(20);
    hBoxLayout[3]->addWidget(toppushbutton[1]);
    hBoxLayout[3]->addSpacing(20);
    hBoxLayout[3]->addWidget(toppushbutton[2]);
    hBoxLayout[3]->addSpacerItem(hSpacer1);
    hBoxLayout[3]->addWidget(toppushbutton[3]);
    hBoxLayout[3]->addSpacing(20);
    hBoxLayout[3]->addWidget(toppushbutton[4]);
    hBoxLayout[3]->addSpacing(40);
    hBoxLayout[3]->setContentsMargins(0, 0, 0, 0);
    hBoxLayout[3]->setAlignment(Qt::AlignLeft);

    hWidget[3]->setLayout(hBoxLayout[3]);

    /* 位于最上层 */
    vWidget[2]->raise();

}
void PlayerWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    vWidget[1]->setGeometry(0, this->height() - 80, this->width(), 80);
    vWidget[2]->setGeometry(0, 0, this->width()+20, 65);
    hWidget[0]->setGeometry(0, 65, this->width(), this->height()-65);

}

