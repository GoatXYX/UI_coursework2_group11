#include "settingwindow.h"
#include "playerwindow.h"
#include <QDebug>

SettingWindow::SettingWindow(QWidget *parent)
    : QWidget{parent}
{
    setWindowTitle("Settings");
    settinglayout();
    connect_signal();
}

SettingWindow::~SettingWindow(){
    delete btn;
    delete reset;
}

//connect all signals
void SettingWindow::connect_signal(){
    //sliders
    connect(bright_slider, SIGNAL(sliderReleased()), this, SLOT(bright_slider_re()));
    connect(con_slider, SIGNAL(sliderReleased()), this, SLOT(con_slider_re()));
    connect(satu_slider, SIGNAL(sliderReleased()), this, SLOT(satu_slider_re()));
    connect(hue_slider, SIGNAL(sliderReleased()), this, SLOT(hue_slider_re()));
    connect(reset,SIGNAL(clicked()),this,SLOT(reset_value()));
    connect(btn,SIGNAL(clicked()),this,SLOT(sSlot()));
}

//emit signal of going back to main window
void SettingWindow::sSlot(){
    emit sSignal();
}

//change brightness
void SettingWindow::bright_slider_re()
{
    playW->videoWidget->setBrightness(bright_slider->value());
}

//change contrast
void SettingWindow::con_slider_re()
{
    playW->videoWidget->setContrast(con_slider->value());
}

//change saturation
void SettingWindow::satu_slider_re()
{
    playW->videoWidget->setSaturation(satu_slider->value());
}

//change hue
void SettingWindow::hue_slider_re()
{
    playW->videoWidget->setHue(hue_slider->value());
}

//reset all parameters
void SettingWindow::reset_value()
{
    playW->videoWidget->setBrightness(0);
    playW->videoWidget->setContrast(0);
    playW->videoWidget->setSaturation(0);
    playW->videoWidget->setHue(0);
    bright_slider->setValue(0);
    con_slider->setValue(0);
    satu_slider->setValue(0);
    hue_slider->setValue(0);
}

//layout of this window
void SettingWindow::settinglayout(){
    /*initialize all controls*/
    btn = new QPushButton();
    reset = new QPushButton();
    bright_slider = new QSlider(Qt::Horizontal);
    con_slider = new QSlider(Qt::Horizontal);
    satu_slider = new QSlider(Qt::Horizontal);
    hue_slider = new QSlider(Qt::Horizontal);

    /*label content & style*/
    QFont font;
    QPalette pal;
    font.setPixelSize(18);
    pal.setColor(QPalette::WindowText, Qt::white);
    for(int i=0; i<4; i++){
        label[i] = new QLabel();
        label[i]->setFont(font);
        label[i]->setPalette(pal);
    }

    playW =(PlayerWindow*)parentWidget();

    label[0]->setText("Brightness");
    label[1]->setText("Contrast");
    label[2]->setText("Saturation");
    label[3]->setText("Hue");

    // sliders content & style
    con_slider->setObjectName("bright");
    bright_slider->setObjectName("bright");
    satu_slider->setObjectName("bright");
    hue_slider->setObjectName("bright");

    bright_slider->setRange(-50, 50);
    con_slider->setRange(-50, 50);
    satu_slider->setRange(-50, 50);
    hue_slider->setRange(-50, 50);

    bright_slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    con_slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    satu_slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    hue_slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    btn->setParent(this);
    reset->setParent(this);
    btn->setText("Back to Main");
    reset->setText("Reset");


    // set spacing
    layout->setSpacing(10);
    // add widgets
    layout->addWidget(bright_slider, 0, 0);
    layout->addWidget(label[0], 0, 1);
    layout->addWidget(con_slider, 1, 0);
    layout->addWidget(label[1], 1, 1);
    layout->addWidget(satu_slider, 2, 0);
    layout->addWidget(label[2], 2, 1);
    layout->addWidget(hue_slider, 3, 0);
    layout->addWidget(label[3], 3, 1);
    layout->addWidget(reset, 4, 0, 1, 2);
    layout->addWidget(btn, 5, 0, 1, 2);

    //set row & column scale factor
    layout->setRowStretch(0, 1);
    layout->setRowStretch(1, 1);
    layout->setRowStretch(2, 1);
    layout->setRowStretch(3, 1);
    layout->setRowStretch(4, 1);
    layout->setRowStretch(5, 1);
    layout->setColumnStretch(0, 3);
    layout->setColumnStretch(1, 1);

    this->setLayout(layout);


    bright_slider->setValue(playW->videoWidget->brightness());
    con_slider->setValue(playW->videoWidget->contrast());
    satu_slider->setValue(playW->videoWidget->saturation());
    hue_slider->setValue(playW->videoWidget->hue());
}
