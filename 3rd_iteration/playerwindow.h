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


class SettingWindow;
class PlayerWindow : public QMainWindow
{
    Q_OBJECT

public:
    PlayerWindow();
    void connect_signal();
    void contextMenuEvent ( QContextMenuEvent * event );
    bool find(QVector<QVector<int>>, int);
    QVideoWidget *videoWidget;

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

    QListWidget *list;

    QMenu* popMenu;
    QAction *video, *re_video, *de_video, *tag_location, *tag_time, *sort_location, *sort_time;
    int item;
    QInputDialog input;



    QMenu* menu;
    QAction* act_top_loc;
    QAction* act_top_t;

    SettingWindow* set;
    int lan;

    /* horizontal layout*/
    QHBoxLayout *hBoxLayout[4];
    /* horizontal container */
    QWidget *hWidget[4];
    /* vertical containder */
    QWidget *vWidget[3];
    /* vertical layout */
    QVBoxLayout *vBoxLayout[3];

    /* layout of the widget */
    void videoLayout();

    /* rewrite of the resize window function */
    void resizeEvent(QResizeEvent *event);

private slots:
    void play();
    void next();
    void volme_up();
    void volme_down();
    void fullscreen();
    void video_slider_re();
    void volume_slider_re();

    void select_video();
    void rename_video();
    void delete_video();
    void tag_loc();
    void tag_t();
    void sort_loc();
    void sort_t();
    void top_tag_loc();
    void top_tag_t();
    void change_lan();

    void play_or_not(QMediaPlayer::State state);
    void mlist_change(int);
    void vlength_change(qint64);
    void vposition_change(qint64);
    void click(QListWidgetItem*);

    void sortMenu();
    void sort_top(QAction *action);

    void to_set();
    void to_main();
};

#endif // PLAYERWINDOW_H
