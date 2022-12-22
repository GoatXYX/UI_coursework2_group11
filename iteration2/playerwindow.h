#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QCloseEvent>
#include <QDebug>


#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <QDialogButtonBox>

#include <QPainter>
#include <QGridLayout>
#include <QVector>
#include <QPair>


class PlayerWindow : public QMainWindow
{
    Q_OBJECT

public:
    PlayerWindow();
    void connect_signal();
    void contextMenuEvent ( QContextMenuEvent * event );
    bool find(QVector<QVector<int>>, int);

private:
    QString fileName;
    QString filePath;

    QPushButton *toppushbutton[5];

    QSlider *video_slider;
    QPushButton *playerpushbutton[6];
    QLabel *label[2];
    QSlider *volume_slider;

    QMediaPlayer *player;
    QMediaPlaylist *mlist;
    QVideoWidget *videoWidget;

    QListWidget *list;

    QMenu* popMenu;
    QAction *video, *re_video, *de_video, *tag_location, *tag_time, *sort_location, *sort_time;
    int item;
    QInputDialog input;

    QMenu* menu;
    QAction* act_top_loc;
    QAction* act_top_t;

    /* 水平布局 */
    QHBoxLayout *hBoxLayout[4];
    /* 水平容器 */
    QWidget *hWidget[4];
    /* 垂直容器 */
    QWidget *vWidget[3];
    /* 垂直界面 */
    QVBoxLayout *vBoxLayout[3];

    /* 视频布局函数 */
    void videoLayout();

    /* 主窗体大小重设大小函数重写 */
    void resizeEvent(QResizeEvent *event);

private slots:
    void play();
    void fullscreen();
    void video_slider_re();
    void volume_slider_re();

    void select_video();
    void rename_video();

    void play_or_not(QMediaPlayer::State state);
    void mlist_change(int);
    void vlength_change(qint64);
    void vposition_change(qint64);
    void click(QListWidgetItem*);

    void sortMenu();
    void sort_top(QAction *action);
};

#endif // PLAYERWINDOW_H
