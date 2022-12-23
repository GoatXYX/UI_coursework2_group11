#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H
#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QGridLayout>
#include <QLabel>

class PlayerWindow;
//The Widget of the settings
class SettingWindow: public QWidget
{
    Q_OBJECT

public:
    explicit SettingWindow(QWidget *parent = nullptr);
    ~SettingWindow();
    QLabel* label[4];
    //button go back to main window
    QPushButton* btn;
    //button to reset all settings
    QPushButton* reset;
    void connect_signal();

private:
    //settings sliders
    QSlider* bright_slider;
    QSlider* con_slider;
    QSlider* satu_slider;
    QSlider* hue_slider;

    QGridLayout *layout = new QGridLayout();
    PlayerWindow* playW;

    void settinglayout();

public slots:
    void sSlot();
    void bright_slider_re();
    void con_slider_re();
    void satu_slider_re();
    void hue_slider_re();
    void reset_value();
signals:
    void sSignal();
};

#endif // SETTINGWINDOW_H
