#include "chooselevelscene.h"
#include <QLabel>
#include<QMenu>
#include<QMenuBar>
#include<QPainter>
#include"mypushbutton.h"
#include<QTimer>
#include<QDebug>
#include <QSound>
#include"testwindow.h"
ChooseLevelScene::ChooseLevelScene(QWidget *parent) : QMainWindow(parent)
{
//    qDebug() << "ChooseLevelScene全局变量测试："<<TestWindow::aaa++;
//    extern int bbb;
//    qDebug() << "ChooseLevelScene extern测试："<<bbb++;
    this->setFixedSize (320,588);
    this->setWindowIcon (QPixmap(":res/Coin0001.png"));
    this->setWindowTitle ("选择关卡");

    //选择关卡按钮音效
    QSound *chooseSound = new QSound(":/res/TapButtonSound.wav",this);
    //返回按钮音效
    QSound *backSound = new QSound(":/res/BackButtonSound.wav",this);


    //创建菜单栏
    QMenuBar * bar = this->menuBar ();
    this->setMenuBar (bar);
    //创建开始菜单
    QMenu * startMenu = bar->addMenu("开始");
    //创建按钮菜单项
    QAction * quitAction = startMenu->addAction ("退出");
    //点击退出，退出游戏
    connect (quitAction,&QAction::triggered ,[=](){
        this->close ();
    });
    //创建音效开关
    QString flag_sound  = TestWindow::flag_sound ? "音效：开" :"音效：关";
    QMenu * soundMenu = bar->addMenu (flag_sound);
    QAction * openSound = soundMenu->addAction ("开");
    QAction * closeSound = soundMenu->addAction ("关");
    connect (openSound,&QAction::triggered ,[=](){
        TestWindow::flag_sound = true;
        soundMenu->setTitle (TestWindow::flag_sound ? "音效：开" :"音效：关");

    });
    connect (closeSound,&QAction::triggered ,[=](){
        TestWindow::flag_sound = false;
        soundMenu->setTitle (TestWindow::flag_sound ? "音效：开" :"音效：关");
    });


    //创建返回按钮
    MyPushButton * closeBtn = new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    closeBtn->setParent (this);
    //移动到左下角
    closeBtn->move (0,this->height ()-closeBtn->height ());

    //返回按钮的功能实现,
    //延时0.3后隐藏自身，并且发送自定义信号，告诉外界自身已经选择了返回按钮。
    connect (closeBtn,&MyPushButton::clicked ,this,[=](){
        if(TestWindow::flag_sound) backSound->play();

        QTimer::singleShot(300,this,[=](){
            this->hide();
            //触发自定义信号，关闭自身，
            emit this->chooseSceneBack ();
        });
    });


   //创建选择关卡按钮
    for(int i = 0; i<20; i++){
        MyPushButton * menuBtn = new MyPushButton(":/res/LevelIcon.png");
        menuBtn->setParent (this);
        menuBtn->move(25 + (i%4)*70, 130 + (i/4)*70);

        //按钮上显示的文字
        QLabel * label = new QLabel;
        label->setParent (this);
        label->setFixedSize (menuBtn->width (),menuBtn->height ());
        label->setText (QString::number (i+1));
        label->setAlignment (Qt::AlignHCenter | Qt::AlignVCenter);
        label->move (25 + (i%4)*70, 130 + (i/4)*70);
        //鼠标事件穿透
        label->setAttribute (Qt::WA_TransparentForMouseEvents,true);
        //监听每个按钮的点击事件
        connect (menuBtn,&MyPushButton::clicked,[=](){
             //qDebug() << "select: " << i;
             //游戏场景最好不用复用，直接移除掉创建新的场景

            //播放音效
            if(TestWindow::flag_sound) chooseSound->play();

            if(pScene == NULL){
                this->hide ();
                //将选择的关卡号 传入给PlayerScene
                pScene = new PlayScene(i+1);

                pScene->setGeometry (this->geometry ());
                pScene->show ();

                //监听PlayScene的返回信号
                connect (pScene,&PlayScene::chooseSceneBack ,[=](){
                    this->setGeometry (pScene->geometry ());
                    this->show ();
                    delete pScene;
                    pScene = NULL;
                });
            }
        });
    }




}
void ChooseLevelScene::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPixmap pix;
    pix.load (":/res/OtherSceneBg.png");
    painter.drawPixmap (0,0,this->width (),this->height (),pix);
    pix.load (":/res/Title.png");
    painter.drawPixmap ((this->width ()-pix.width ())*0.5,30,pix.width (),pix.height (),pix);
}
