#include "playerwindow.h"
#include "settingwindow.h"
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
    toppushbutton[3]->setCheckable(true);
//    连接信号
    connect_signal();
}

//connect all signals
void PlayerWindow::connect_signal(){
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(play_or_not(QMediaPlayer::State)));
    connect(mlist, SIGNAL(currentIndexChanged(int)), this, SLOT(mlist_change(int)));
    connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(vlength_change(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(vposition_change(qint64)));

    connect(video_slider, SIGNAL(sliderReleased()), this, SLOT(video_slider_re()));
    connect(playerpushbutton[0], SIGNAL(clicked()), this, SLOT(play()));
    QAbstractButton::connect(playerpushbutton[1], SIGNAL(clicked()), this, SLOT(next()));
//    volumn
    connect(volume_slider, SIGNAL(sliderReleased()), this, SLOT(volume_slider_re()));
    connect(playerpushbutton[2], SIGNAL(clicked()), this, SLOT(volme_down()));
    connect(playerpushbutton[3], SIGNAL(clicked()), this, SLOT(volme_up()));
    connect(playerpushbutton[4], SIGNAL(clicked()), this, SLOT(fullscreen()));
    connect(playerpushbutton[5], SIGNAL(clicked()), this, SLOT(to_set()));
    connect(set, SIGNAL(sSignal()), this, SLOT(to_main()));

    connect(toppushbutton[0], SIGNAL(clicked()), this, SLOT(select_video()));
    connect(toppushbutton[1], SIGNAL(clicked()), this, SLOT(tag_loc()));
    connect(toppushbutton[2], SIGNAL(clicked()), this, SLOT(tag_t()));
    connect(toppushbutton[3], SIGNAL(clicked()), this, SLOT(change_lan()));





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

//display current moment of the player
void PlayerWindow::vlength_change(qint64 d){
    video_slider->setRange(0, d/1000);
    int second  = (d/1000)%60;
    int minute = ((d/1000)/60)%60;
    int hour = ((d/1000)/60)/60;

    QString right;
    right = QString("%1:%2:%3").arg(hour, 2, 10, QLatin1Char('0')).arg(minute, 2, 10, QLatin1Char('0')).arg(second, 2, 10, QLatin1Char('0'));

    label[1]->setText("/" + right);
}

//change the video slider position
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

//release the video slider
void PlayerWindow::video_slider_re()
{
    player->setPosition(video_slider->value() * 1000);
}

//play the video
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

//next video
void PlayerWindow::next()
{
    if(mlist->mediaCount() ==0){
        return;
    }
    player->stop();
    mlist->next();
    player->play();
}

//release volume slider
void PlayerWindow::volume_slider_re()
{
    player->setVolume(volume_slider->value());
}

//volume up 5
void PlayerWindow::volme_up()
{
    volume_slider->setValue(volume_slider->value() + 10);
    player->setVolume(volume_slider->value());
}

//volume down 5
void PlayerWindow::volme_down()
{
    volume_slider->setValue(volume_slider->value() - 10);
    player->setVolume(volume_slider->value());
}

//swith to fullscreen
void PlayerWindow::fullscreen()
{
    /* 全屏/非全屏操作 */
    vWidget[0]->setVisible(!playerpushbutton[4]->isChecked());
}

//import videos from local files
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

//rename the selected video
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

//delete the selected video
void PlayerWindow::delete_video(){
    player->stop();
    mlist->setCurrentIndex(item);
    QString filepath = mlist->currentMedia().request().url().toLocalFile();
    if (!filepath.isEmpty()){
        QFile::remove(filepath);
        QFile::remove(filepath.left(filepath.length()-3)+"png");
    }

    if(list->count() == 1){
        mlist->clear();
        list->clear();
    }
    else if(item == list->count()-1){
        mlist->setCurrentIndex(item-1);
        mlist->removeMedia(item);
        list->takeItem(item);
    }
    else{
        mlist->setCurrentIndex(item+1);
        mlist->removeMedia(item);
        list->takeItem(item);
    }
    player->play();
}

//tag location on the top
void PlayerWindow::top_tag_loc(){
    item = mlist->currentIndex();
    tag_loc();
}

//tag time on the top
void PlayerWindow::top_tag_t(){
    item = mlist->currentIndex();
    tag_t();
}

//tag location
void PlayerWindow::tag_loc(){
    player->pause();
    mlist->setCurrentIndex(item);
    QString filepath = mlist->currentMedia().request().url().toLocalFile();
    QFileInfo fileInfo = QFileInfo(filepath);
    QString filename = QString::fromUtf8(fileInfo.fileName().toUtf8().data());
    QString path = filepath.left(filepath.length() - filename.length());
    qDebug()<<filename;

    QString imgpath = filepath.left(filepath.length() - 3);
    imgpath = imgpath + "png";

    bool Ok = false;
    QString sName = QInputDialog::getText(NULL, "Tag Location", "Please enter the location of this video", QLineEdit::Normal, NULL, &Ok);

    if (Ok && !sName.isEmpty()) {
        mlist->removeMedia(item);
        list->takeItem(item);
        QString name;
        QString imgname;

        QStringList list1 = filename.split("_");
        qDebug()<<list1.length();
        for(int i=0; i<list1.length(); i++){
            qDebug()<<list1[i];
        }

        if(list1.length() == 5){
            name = path + sName + "_loc_" + list1[2] + "_time_" + list1[4];
            imgname = path + sName + "_loc_" + list1[2] + "_time_" + list1[4].left(list1[4].length()-3) + "png";
        }
        else if(list1.length() == 3)
        {
            if(list1[1].compare(QString::fromLocal8Bit("loc")) == 0){
                name = path + sName + "_loc_" + list1[2];
                imgname = path + sName + "_loc_" + list1[2].left(list1[2].length()-3) + "png";
            }
            else{
                name = path + sName + "_loc_" + list1[0] + "_time_" + list1[2];
                imgname = path + sName + "_loc_" + list1[0] + "_time_" + list1[2].left(list1[2].length()-3) + "png";
            }
        }
        else{
            name = path + sName + "_loc_" + list1[0];
            imgname = path + sName + "_loc_" + list1[0].left(list1[0].length()-3) + "png";
        }

        qDebug()<<name<<","<<imgname;

        QFile file(filepath);
        bool re_ok = file.rename(name);
        QFile imgfile(imgpath);
        bool reimg_ok = imgfile.rename(imgname);

        if(re_ok && reimg_ok)
        {
            QFileInfo fileInfo = QFileInfo(name);
            QString filename = QString::fromUtf8(fileInfo.fileName().toUtf8().data());

            mlist->insertMedia(item, QUrl::fromLocalFile(name));
            QListWidgetItem *i = new QListWidgetItem();
            i->setIcon(QIcon(imgname));
            i->setText(filename);
            i->setSizeHint(QSize(250, 230));
            list->insertItem(item, i);
        }
    }
}

//tag time
void PlayerWindow::tag_t(){
    player->pause();
    mlist->setCurrentIndex(item);
    QString filepath = mlist->currentMedia().request().url().toLocalFile();
    QFileInfo fileInfo = QFileInfo(filepath);
    QString filename = QString::fromUtf8(fileInfo.fileName().toUtf8().data());
    QString path = filepath.left(filepath.length() - filename.length());
    QString suffix = filepath.right(3);
    qDebug()<<filename;

    QString imgpath = filepath.left(filepath.length() - 3);
    imgpath = imgpath + "png";

    bool Ok = false;
    QInputDialog dialog;
    QString sName = dialog.getText(NULL, "Tag Time", "Please enter the time of this video", QLineEdit::Normal, NULL, &Ok);
    if (Ok && !sName.isEmpty()) {
        mlist->removeMedia(item);
        list->takeItem(item);
        QString name;
        QString imgname;
        QStringList list1 = filename.split("_");
        if(list1.length() == 5){
            name = path + list1[0] + "_loc_" + list1[2] + "_time_" + sName + "." + suffix;
            imgname = path + list1[0] + "_loc_" + list1[2] + "_time_" + sName + ".png";
        }
        else if(list1.length() == 3)
        {
            if(list1[1].compare(QString::fromLocal8Bit("loc")) == 0){
                name = path + list1[0] + "_loc_" + list1[2].left(list1[2].length()-4) + "_time_" + sName + "." + suffix;
                imgname = path + list1[0] + "_loc_" + list1[2].left(list1[2].length()-4) + "_time_" + sName + ".png";
            }
            else{
                name = path + list1[0] + "_time_" + sName + "." + suffix;
                imgname = path + list1[0] + "_time_" + sName + ".png";
            }
        }
        else{
            name = path + list1[0].left(list1[0].length()-4) + "_time_" + sName + "." + suffix;
            imgname = path  + list1[0].left(list1[0].length()-4) + "_time_" + sName + ".png";
        }

        qDebug()<<name<<","<<imgname;

        QFile file(filepath);
        bool re_ok = file.rename(name);
        QFile imgfile(imgpath);
        bool reimg_ok = imgfile.rename(imgname);

        if(re_ok && reimg_ok)
        {
            QFileInfo fileInfo = QFileInfo(name);
            QString filename = QString::fromUtf8(fileInfo.fileName().toUtf8().data());

            mlist->insertMedia(item, QUrl::fromLocalFile(name));
            QListWidgetItem *i = new QListWidgetItem();
            i->setIcon(QIcon(imgname));
            i->setText(filename);
            i->setSizeHint(QSize(250, 230));
            list->insertItem(item, i);
        }
    }

}

//sort videos by location
void PlayerWindow::sort_loc(){
    QStringList medialistpath;
    QStringList medialist;
    QStringList medialoc;
    QStringList mediat;
    for(int i = 0; i < mlist->mediaCount(); i++){
        QString filepath = mlist->media(i).request().url().toLocalFile();
        medialistpath.append(filepath);
        QFileInfo fileInfo = QFileInfo(filepath);
        QString filename = QString::fromUtf8(fileInfo.fileName().toUtf8().data());
        medialist.append(filename);
    }
    for(int i = 0; i < medialist.length(); i++){
        QStringList list2 = medialist[i].split("_");
        if(list2.length() == 5){
            medialoc.append(list2[0]);
            mediat.append(list2[4]);
        }
        else if(list2.length() == 3)
        {
            if(list2[1].compare(QString::fromLocal8Bit("loc")) == 0){
                medialoc.append(list2[0]);
                mediat.append("");
            }
            else{
                medialoc.append("");
                mediat.append(list2[2]);
            }
        }
        else{
            medialoc.append("");
            mediat.append("");
        }
    }


    player->pause();
    QStringList copy;
    QVector<QVector<int>> index;
    for(int i = 0; i < medialoc.length(); i++){
        copy.append(medialoc[i]);
    }

    for (int i = 0; i < copy.length(); i++){
        QVector<int> temp;
        if(!find(index, i) && copy[i].compare("") != 0){
            temp.append(i);
            for (int j = i + 1; j < copy.length(); j++){
                if (copy[i].compare(copy[j]) == 0)
                {
                    temp.append(j);
                }
            }
            index.append(temp);
        }
    }

    for(int i=0; i<index.length(); i++){
        QString folderPathloc = "D:\\desktop\\videos\\" + medialoc[index[i][0]];
        QString folderPatht = "D:\\desktop\\videos\\" + mediat[index[i][0]].left(mediat[index[i][0]].length()-4);
        QDir dir(folderPathloc);
        if (!dir.exists())
            dir.mkdir(folderPathloc);
        for(int j=0; j<index[i].length(); j++){
            mlist->removeMedia(index[i][j]);
            list->takeItem(index[i][j]);

            QFileInfo fileInfo = QFileInfo(medialistpath[index[i][j]]);
            QString filename = QString::fromUtf8(fileInfo.fileName().toUtf8().data());
            QString name, namet;
            QString imgname, imgnamet;

            name = folderPathloc + "\\" + filename;
            namet = folderPatht + "\\" + filename;
            imgname = name.left(name.length()-3) + "png";
            imgnamet = namet.left(namet.length()-3) + "png";
            QString img = medialistpath[index[i][j]].left(medialistpath[index[i][j]].length()-3) + "png";

            QFile ren(name);
            QFile reimgn(imgname);
            QFile rent(namet);
            QFile reimgnt(imgnamet);
            qDebug()<<name<<","<<imgname;
            qDebug()<<namet<<","<<imgnamet;
            qDebug()<<medialistpath[index[i][j]];
            qDebug()<<rent.exists()<<","<<reimgnt.exists();

            bool re_ok = 1, reimg_ok = 1;
            if(!ren.exists() && !reimgn.exists()){
                if(rent.exists() && reimgnt.exists()){
                    re_ok = QFile::copy(medialistpath[index[i][j]], name);
                    reimg_ok = QFile::copy(img, imgname);
                }
                else{
                    re_ok = QFile::rename(medialistpath[index[i][j]], name);
                    reimg_ok = QFile::rename(img, imgname);
                }
            }

            if(re_ok && reimg_ok)
            {
                mlist->insertMedia(index[i][j], QUrl::fromLocalFile(name));
                QListWidgetItem *listi = new QListWidgetItem();
                listi->setIcon(QIcon(imgname));
                listi->setText(filename);
                listi->setSizeHint(QSize(250, 230));
                list->insertItem(index[i][j], listi);
            }
        }
    }
}

//sort videos by time
void PlayerWindow::sort_t(){
    QStringList medialistpath;
    QStringList medialist;
    QStringList medialoc;
    QStringList mediat;
    for(int i = 0; i < mlist->mediaCount(); i++){
        QString filepath = mlist->media(i).request().url().toLocalFile();
        medialistpath.append(filepath);
        QFileInfo fileInfo = QFileInfo(filepath);
        QString filename = QString::fromUtf8(fileInfo.fileName().toUtf8().data());
        medialist.append(filename);
    }
    for(int i = 0; i < medialist.length(); i++){
        QStringList list2 = medialist[i].split("_");
        if(list2.length() == 5){
            medialoc.append(list2[0]);
            mediat.append(list2[4]);
        }
        else if(list2.length() == 3)
        {
            if(list2[1].compare(QString::fromLocal8Bit("loc")) == 0){
                medialoc.append(list2[0]);
                mediat.append("");
            }
            else{
                medialoc.append("");
                mediat.append(list2[2]);
            }
        }
        else{
            medialoc.append("");
            mediat.append("");
        }
    }

    player->pause();
    QStringList copy;
    QVector<QVector<int>> index;
    for(int i = 0; i < mediat.length(); i++){
        copy.append(mediat[i]);
    }

    for (int i = 0; i < copy.length(); i++){
        QVector<int> temp;
        if(!find(index, i) && copy[i].compare("") != 0){
            temp.append(i);
            for (int j = i + 1; j < copy.length(); j++){
                if (copy[i].compare(copy[j]) == 0)
                {
                    temp.append(j);
                }
            }
            index.append(temp);
        }
    }
    qDebug()<<index<<","<<index.length();

    for(int i=0; i<index.length(); i++){
        QString folderPathloc = "D:\\desktop\\videos\\" + medialoc[index[i][0]];
        QString folderPatht = "D:\\desktop\\videos\\" + mediat[index[i][0]].left(mediat[index[i][0]].length()-4);
        QDir dir(folderPatht);
        if (!dir.exists())
            dir.mkdir(folderPatht);
        for(int j=0; j<index[i].length(); j++){
            mlist->removeMedia(index[i][j]);
            list->takeItem(index[i][j]);

            QFileInfo fileInfo = QFileInfo(medialistpath[index[i][j]]);
            QString filename = QString::fromUtf8(fileInfo.fileName().toUtf8().data());
            QString name, nameloc;
            QString imgname, imgnameloc;

            name = folderPatht + "\\" + filename;
            nameloc = folderPathloc + "\\" + filename;
            imgname = name.left(name.length()-3) + "png";
            imgnameloc = nameloc.left(nameloc.length()-3) + "png";
            QString img = medialistpath[index[i][j]].left(medialistpath[index[i][j]].length()-3) + "png";

            QFile ren(name);
            QFile reimgn(imgname);
            QFile renloc(nameloc);
            QFile reimgnloc(imgnameloc);
            qDebug()<<name<<","<<imgname;
            qDebug()<<nameloc<<","<<imgnameloc;
            qDebug()<<medialistpath[index[i][j]];
            qDebug()<<renloc.exists()<<","<<reimgnloc.exists();

            bool re_ok = 1, reimg_ok = 1;
            if(!ren.exists() && !reimgn.exists()){
                if(renloc.exists() && reimgnloc.exists()){
                    re_ok = QFile::copy(medialistpath[index[i][j]], name);
                    reimg_ok = QFile::copy(img, imgname);
                }
                else{
                    re_ok = QFile::rename(medialistpath[index[i][j]], name);
                    reimg_ok = QFile::rename(img, imgname);
                }
            }

            if(re_ok && reimg_ok)
            {
                mlist->insertMedia(index[i][j], QUrl::fromLocalFile(name));
                QListWidgetItem *listi = new QListWidgetItem();
                listi->setIcon(QIcon(imgname));
                listi->setText(filename);
                listi->setSizeHint(QSize(250, 230));
                list->insertItem(index[i][j], listi);
            }
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

//show setting window
void PlayerWindow::to_set(){
    set->show();
}

//go back to main window
void PlayerWindow::to_main(){
    set->hide();
}

//switch language
void PlayerWindow::change_lan(){
    if(toppushbutton[3]->isChecked()){
        toppushbutton[0]->setText("导入");
        toppushbutton[1]->setText("标记地点");
        toppushbutton[2]->setText("标记时间");
        toppushbutton[3]->setText("切换语言");
        set->label[0]->setText("亮度");
        set->label[1]->setText("对比度");
        set->label[2]->setText("饱和度");
        set->label[3]->setText("色调");
        set->reset->setText("重置");
        set->btn->setText("返回");
        lan = 0;
    }
    else{
        toppushbutton[0]->setText("Import");
        toppushbutton[1]->setText("Tag Loc");
        toppushbutton[2]->setText("Tag Time");
        toppushbutton[3]->setText("Language");
        set->label[0]->setText("Brightness");
        set->label[1]->setText("Contrast");
        set->label[2]->setText("Saturation");
        set->label[3]->setText("Hue");
        set->reset->setText("Reset");
        set->btn->setText("Back to Main");
        lan = 1;
    }

}

//switch to another video in the list
void PlayerWindow::click(QListWidgetItem *item)
{
    player->stop();
    mlist->setCurrentIndex(list->row(item));
    player->play();
}

//the shortcut menu
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
    connect(de_video, SIGNAL(triggered()), this, SLOT(delete_video()));
    tag_location = new QAction("tag location", list);
    connect(tag_location, SIGNAL(triggered()), this, SLOT(tag_loc()));
    tag_time = new QAction("tag time", list);
    connect(tag_time, SIGNAL(triggered()), this, SLOT(tag_t()));

    sort_location = new QAction("sort by location", list);
    connect(sort_location, SIGNAL(triggered()), this, SLOT(sort_loc()));

    sort_time = new QAction("sort by time", list);
    connect(sort_time, SIGNAL(triggered()), this, SLOT(sort_t()));


    if(list->itemAt(mapFromGlobal(QCursor::pos())) != NULL)
    {
        item = list->row(list->itemAt(mapFromGlobal(QCursor::pos())));
        qDebug()<<list->row(list->itemAt(mapFromGlobal(QCursor::pos())));
        popMenu->addAction(re_video);
        popMenu->addAction(de_video);
        popMenu->addAction(tag_location);
        popMenu->addAction(tag_time);
    }
    if(x>300 && y>0 && y<this->height()-80){
        popMenu->addAction(sort_location);
        popMenu->addAction(sort_time);
        popMenu->addAction(video);
    }

    popMenu->exec(QCursor::pos());
}

//sort menu on the top
void PlayerWindow::sortMenu(){
    menu = new QMenu();
    menu->setObjectName("menu_sort");
    act_top_loc      = new QAction("By Loc");
    act_top_t   = new QAction("By Time");
    menu->addAction(act_top_loc);
    menu->addAction(act_top_t);
    connect(act_top_loc, SIGNAL(triggered()), this, SLOT(sort_loc()));
    connect(act_top_t, SIGNAL(triggered()), this, SLOT(sort_t()));
    menu->addSeparator();

    menu->setWindowFlags(menu->windowFlags() | Qt::FramelessWindowHint);
    menu->setAttribute(Qt::WA_TranslucentBackground);
    menu->setStyleSheet(" QMenu {border-radius:5px;font-family:'Microsoft Yahei';font-size:14px;color:#fff;}"
                            " QMenu::item {height:30px; width:100px;padding-left:20px;border: 1px solid none;}"
                            "QMenu::item:selected {background-color:rgb(0,120,215);\
                            padding-left:20px;border: 1px solid rgb(65,173,255);}");

    toppushbutton[4]->setMenu(menu);
}

//layout of this window
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
    toppushbutton[3]->setText("Language");


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

    set = new SettingWindow(this);
    set->setObjectName("set");
    set->setGeometry(this->width()-200, this->height()-290, 200, 210);
    set->raise();
    set->hide();
    lan = 1;

}

//rewrite of resize window function
void PlayerWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    vWidget[1]->setGeometry(0, this->height() - 80, this->width(), 80);
    vWidget[2]->setGeometry(0, 0, this->width()+20, 65);
    hWidget[0]->setGeometry(0, 65, this->width(), this->height()-65);
    set->setGeometry(this->width()-200, this->height()-290, 200, 210);

}

