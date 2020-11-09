#include "playscene.h"
#include<QDebug>
#include<QMenu>
#include<QMenuBar>
#include<QPainter>
#include<mypushbutton.h>
#include<QTimer>
#include <QLabel>
#include <QPropertyAnimation>
#include <QSound>
#include"mycoin.h"
#include"dataconfig.h"
#include"testwindow.h"
PlayScene::PlayScene(QWidget *parent) : QMainWindow(parent)
{

}
PlayScene::PlayScene(int i)
{
//    qDebug() << "PlayScene全局变量测试："<<TestWindow::aaa++;
//    qDebug() << "当前关卡为："<<i;
//    extern int bbb;
//    qDebug() << "extern测试："<<bbb++;

    //初始化游戏数据
    this->initData (i);

    this->setFixedSize (320,588);
    this->setWindowIcon (QPixmap(":/res/Coin0001.png"));


    //创建菜单栏
    QMenuBar * bar = this->menuBar ();
    this->setMenuBar (bar);
    //创建开始菜单
    QMenu * startMenu = bar->addMenu("开始");
    QAction * quitAction = startMenu->addAction ("退出");
    connect (quitAction,&QAction::triggered,[=](){
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
    closeBtn->setParent(this);
    closeBtn->move(0,this->height()-closeBtn->height());
    //返回按钮音效
    QSound *backSound = new QSound(":/res/BackButtonSound.wav",this);
    //翻金币音效
    QSound *flipSound = new QSound(":/res/ConFlipSound.wav",this);
    //胜利按钮音效
    QSound *winSound = new QSound(":/res/LevelWinSound.wav",this);

    //返回按钮点击事件
    connect (closeBtn,&MyPushButton::clicked,[=](){
        if(TestWindow::flag_sound) backSound->play();

        QTimer::singleShot(500, this,[=](){
            this->hide();
            //触发自定义信号，关闭自身，该信号写到 signals下做声明
            emit this->chooseSceneBack();
             }
        );
    });


    //当前关卡标题
    this->titleLabel = new QLabel;
    this->titleLabel->setParent (this);
    QFont font;
    font.setFamily ("华文新魏");
    font.setPointSize (20);
    this->titleLabel->setFont (font);
    QString str = QString("Level:%1").arg (this->levelIndex);
    this->titleLabel->setText (str);
    this->titleLabel->setGeometry (QRect(this->width ()-120,20,120,50));//设置大小和位置

    //将胜利的图片提前创建好，如果胜利触发了，将图片弹下来即可
    this->winLabel = new QLabel;
    QPixmap tmpPix;
    tmpPix.load(":/res/LevelCompletedDialogBg.png");
    this->winLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
    this->winLabel->setPixmap(tmpPix);
    this->winLabel->setParent(this);
    //winLabel->setVisible (true);
    this->winLabel->move( (this->width() - tmpPix.width())*0.5 , -tmpPix.height());

    //下一关按钮
    MyPushButton * nextBtn = new MyPushButton(":/res/Next.png",":/res/Next0.png");
    nextBtn->setParent (this);
    nextBtn->move(this->width ()-nextBtn->width (),this->height()-nextBtn->height());
    //nextBtn->setVisible (false);
    //下一关按钮点击事件
    connect (nextBtn,&QPushButton::clicked,[=](){
        if(TestWindow::flag_sound) backSound->play();

        QTimer::singleShot(500, this,[=](){
            this->resetData (this->levelIndex+1);
            //nextBtn->setVisible (!nextBtn->isVisible ());
        }
        );
    });
    //创建金币的背景图片
      for(int i = 0 ; i < 4;i++)
      {
          for(int j = 0 ; j < 4; j++)
          {
             //绘制背景图片
              QLabel* label = new QLabel;
              label->setGeometry(0,0,50,50);
              label->setPixmap(QPixmap(":/res/BoardNode(1).png"));
              label->setParent(this);
              label->move(57 + i*50,200+j*50);

              //创建金币对象
              QString img;
              if(gameArray[i][j] == 1){//1表示正面
                  img = ":/res/Coin0001.png";
              }else{
                  img = ":/res/Coin0008.png";
              }
              MyCoin * coin = new MyCoin(img);
              coin->setParent (this);
              coin->move(59 + i*50,204+j*50);
              coin->posX = i; //记录x坐标
              coin->posY = j; //记录y坐标
              coin->flag =gameArray[i][j]; //记录正反标志

              coinBtn[i][j] = coin;//记录每个按钮的位置

              //监听每个按钮的点击效果，并翻转金币
              //TODO 金币点击出问题了！！！
              connect (coin,&MyCoin::clicked,[=](){
                  if(TestWindow::flag_sound) flipSound->play();

                  coin->changeFlag ();
                  //数组内部记录的标志同步修改
                  gameArray[i][j] = gameArray[i][j] == 0 ? 1 : 0;

                  //延时翻动其他周围金币
                  QTimer::singleShot(300, this,[=](){

                      //判断要不要翻右边的
                      if(coin->posX+1 <=3)
                      {
                          coinBtn[coin->posX+1][coin->posY]->changeFlag();
                          gameArray[coin->posX+1][coin->posY] = gameArray[coin->posX+1][coin->posY]== 0 ? 1 : 0;
                      }
                      //判断要不要翻左边的
                      if(coin->posX-1>=0)
                      {
                          coinBtn[coin->posX-1][coin->posY]->changeFlag();
                          gameArray[coin->posX-1][coin->posY] = gameArray[coin->posX-1][coin->posY]== 0 ? 1 : 0;
                      }
                      //判断要不要翻下边的
                      if(coin->posY+1<=3)
                      {
                          coinBtn[coin->posX][coin->posY+1]->changeFlag();
                          gameArray[coin->posX][coin->posY+1] = gameArray[coin->posX+1][coin->posY]== 0 ? 1 : 0;
                      }
                      //判断要不要翻上边的
                      if(coin->posY-1>=0)
                      {
                          coinBtn[coin->posX][coin->posY-1]->changeFlag();
                          gameArray[coin->posX][coin->posY-1] = gameArray[coin->posX+1][coin->posY]== 0 ? 1 : 0;
                      }

                      //翻完金币后判断是否胜利
                      this->isWIN ();
                      if(this->isWin)
                      {
                          if(TestWindow::flag_sound) winSound->play();
                          qDebug() << "胜利";
                          QPropertyAnimation * animation1 =  new QPropertyAnimation(winLabel,"geometry");
                          animation1->setDuration(1000);
                          animation1->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
                          animation1->setEndValue(QRect(winLabel->x(),winLabel->y()+114,winLabel->width(),winLabel->height()));
                          animation1->setEasingCurve(QEasingCurve::OutBounce);
                          animation1->start();

                          //如果胜利了就显示下一关按钮
                          nextBtn->setVisible(true);
                      }
                  });

              });
          }
      }

}
void PlayScene::resetData(int i){
    //如果大于20，不做处理
    if(i>20) return ;

     this->initData (i);
    qDebug() << "resetData???"<<levelIndex;
    this->titleLabel->setText (QString("Level:%1").arg(this->levelIndex));
    QPixmap tmpPix;
    tmpPix.load(":/res/LevelCompletedDialogBg.png");
    this->winLabel->move( (this->width() - tmpPix.width())*0.5 , -tmpPix.height());
    //TODO 改金币正反面数据coinBtn,通过gameArray里的数据修改coinBtn里的


    QPixmap pixmap0,pixmap1;
    pixmap0.load(":/res/Coin0008.png");
    pixmap1.load(":/res/Coin0001.png");
    for(int i = 0 ; i < 4;i++)
    {
        for(int j = 0 ; j < 4; j++)
        {
            //qDebug() << gameArray[i][j]<<" ";
            if(this->gameArray[i][j]==0){
                this->coinBtn[i][j]->changImg (pixmap0);
            }else{
                this->coinBtn[i][j]->changImg (pixmap1);
            }
            this->coinBtn[i][j]->flag =gameArray[i][j]; //记录正反标志
        }
        //qDebug() << "\n";
    }

}
void PlayScene::initData(int i){

    //初始化flag,游戏开始前肯定为false
    TestWindow::flag_win = false;
    //初始化游戏数据
    dataConfig config;
    this->levelIndex = i;

    for(int i = 0 ; i < 4;i++)
     {
         for(int j = 0 ; j < 4; j++)
         {
             gameArray[i][j] = config.mData[this->levelIndex][i][j];
         }
     }
    this->setWindowTitle ("翻金币--level "+QString::number (i));
}
void PlayScene::isWIN (){
    //判断是否胜利
    this->isWin = true;
    for(int i = 0 ; i < 4;i++)
    {
        for(int j = 0 ; j < 4; j++)
        {
            //qDebug() << coinBtn[i][j]->flag ;
            if( coinBtn[i][j]->flag == false)
            {
                this->isWin = false;
                break;
            }
        }
    }
    if(this->isWin){
        qDebug() << "WIN！";
        TestWindow::flag_win = true;
    }
}
void PlayScene::paintEvent(QPaintEvent *){
    //加载背景
    QPainter painter(this);
    QPixmap pix;
    pix.load (":/res/bg.png");
    painter.drawPixmap (0,0,this->width (),this->height (),pix);
    //加载标题
    pix.load (":/res/Title.png");
    painter.drawPixmap (10,30,pix.width (),pix.height (),pix);

}
