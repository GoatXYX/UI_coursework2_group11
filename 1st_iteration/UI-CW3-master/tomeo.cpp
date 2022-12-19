/*
 *
 *    ______
 *   /_  __/___  ____ ___  ___  ____
 *    / / / __ \/ __ `__ \/ _ \/ __ \
 *   / / / /_/ / / / / / /  __/ /_/ /
 *  /_/  \____/_/ /_/ /_/\___/\____/
 *              video for sports enthusiasts...
 *
 *  2811 cw3 : twak
 */


#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QLabel>
#include <QScrollArea>
#include "the_player.h"
#include "the_button.h"
#include "application.h"

// read in videos and thumbnails to this directory - don't really wanna mess with this tbh
vector<TheButtonInfo> getInfoIn (string loc) {

    vector<TheButtonInfo> out =  vector<TheButtonInfo>();
    QDir dir(QString::fromStdString(loc) );//Overall iterates through directory, converts string to unicode as intermediary process
    QDirIterator it(dir);

    while (it.hasNext()) { //while there is another file

        QString f = it.next(); //unicode character string = next item in directory

            if (f.contains(".")) //presumably ignores files with no file extension?

#if defined(_WIN32)
            if (f.contains(".wmv"))  {
#else
            if (f.contains(".mp4") || f.contains("MOV"))  {
#endif

            QString thumb = f.left( f .length() - 4) +".png";
            if (QFile(thumb).exists()) { // if a png thumbnail exists
                QImageReader *imageReader = new QImageReader(thumb); // reads image from file
                    QImage sprite = imageReader->read(); // read the thumbnail
                    if (!sprite.isNull()) {
                        QIcon *ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button - new scalable icon object pixelmaps as paint device from sprite to QIcon
                        QUrl *url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url - pretty much this
                        out . push_back(TheButtonInfo( url , ico  ) ); // add to the output list - passes information into vector of button infos(icon and location)
                    }
                    else
                        qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb;//self explanatory - not sprite was not null
            }
            else
                qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb;// thumbnail doesn't exist
        }
    }

    return out;
}


int main(int argc, char *argv[]) {

    qDebug() << "Qt version: " << QT_VERSION_STR;// check Qt is operational

    QApplication app(argc, argv);// create Qt Application

    vector<TheButtonInfo> videos;// vector of button info objects? to store all vids in folder

    if (argc == 2)
        videos = getInfoIn( string(argv[1]) );//calls method to read all the files in specified directory and store videos in vector videos

    if (videos.size() == 0) {

        const int result = QMessageBox::information(
                    NULL,
                    QString("Tomeo"),
                    QString("no videos found! Add command line argument to \"quoted\" file location."));
        exit(-1);
    }

    
    Application window(videos);

    // showtime!
    window.show();

    return app.exec();//starts main event loop
}

