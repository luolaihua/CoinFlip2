#include "mainscene.h"
#include "ui_mainscene.h"
#include<QPainter>
#include"mypushbutton.h"
#include"chooselevelscene.h"
#include<QTimer>
#include<QDateTime>
#include<QDebug>
#include<QSound>
#include"testwindow.h"
MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
//    qDebug() << "全局变量测试："<<TestWindow::aaa;
//    extern int bbb;
//    qDebug() << "extern测试："<<bbb++;
    ui->setupUi(this);
    this->setFixedSize (320,588);
    this->setWindowIcon (QPixmap(":/res/Coin0001.png"));
    this->setWindowTitle ("来华金币翻翻乐");
    //开始菜单中关闭选项
    connect (ui->actionexit,&QAction::triggered,[=](){
        this->close ();
    });
    //设置音效的开关
    connect (ui->actionclose_2,&QAction::triggered,[=](){
        ui->soundMenu->setTitle ("音效：关");
        TestWindow::flag_sound = false;
    });
    connect (ui->actionopen_2,&QAction::triggered,[=](){
        ui->soundMenu->setTitle ("音效：开");
        TestWindow::flag_sound = true;
    });

    //创建选择关卡场景
    ChooseLevelScene *chooseScene = new ChooseLevelScene;

    //设置点击开始按钮的音效
    QSound * startSound = new QSound(":/res/TapButtonSound.wav",this);
    MyPushButton * startBtn = new MyPushButton(":/res/MenuSceneStartButton.png");
    startBtn->setParent (this);
    startBtn->move (this->width ()*0.5-startBtn->width ()*0.5,this->height ()*0.7);
    connect (startBtn,&MyPushButton::pressed,[=](){
        startBtn->zoom1 ();
        if(TestWindow::flag_sound){
            startSound->play(); //开始音效
        }
        //startBtn->zoom2 ();
    });
    connect (startBtn,&MyPushButton::released ,[=](){
        //startBtn->zoom1 ();
        if(TestWindow::flag_sound){
            startSound->play(); //开始音效
        }

        startBtn->zoom2 ();
        //延时0.2秒后进入选择场景
        QTimer::singleShot(200,this,[=](){
            this->hide ();
            //设置下一个场景的位置与上一个场景相同
            chooseScene->setGeometry (this->geometry ());
            chooseScene->show ();
        });
    });
    //点击开始按钮显示选择关卡的同时，监听选择关卡的返回按钮消息
    connect (chooseScene,&ChooseLevelScene::chooseSceneBack ,[=](){
        //设置下一个场景的位置与上一个场景相同
        chooseScene->setGeometry (this->geometry ());
        this->show ();
    });


    //创建一个测试场景
//    TestWindow *tw  = new TestWindow;
//    tw->show ();
//    QDateTime current_date_time = QDateTime::currentDateTime();
//    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm::ss.zzz");
//    qDebug() <<"起始时间："<< current_date.toUtf8 ().data ();

}

MainScene::~MainScene()
{
    delete ui;
}
void MainScene::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPixmap pix;
    pix.load (":/res/PlayLevelSceneBg.png");
    painter.drawPixmap (0,0,this->width (),this->height (),pix);
    pix.load (":/res/Title.png");
    pix = pix.scaled (pix.width ()*0.5,pix.height ()*0.5);
    painter.drawPixmap (10,30,pix.width (),pix.height (),pix);
}
