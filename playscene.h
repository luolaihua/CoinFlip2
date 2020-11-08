#ifndef PLAYSCENE_H
#define PLAYSCENE_H
#include"mycoin.h"
#include <QLabel>
#include <QMainWindow>

class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit PlayScene(QWidget *parent = nullptr);
    PlayScene(int i);
    int levelIndex;
    void paintEvent(QPaintEvent *);
    //用户记录当前关卡的二维数组
    int gameArray[4][4];
    MyCoin * coinBtn[4][4]; //金币按钮数组
    bool isWin = false;//是否胜利
    //当前关卡标题
    QLabel * titleLabel;
    //胜利的图片
    QLabel * winLabel;
    void isWIN();
    void initData(int i);
    void resetData(int i);
signals:
    void chooseSceneBack();
};

#endif // PLAYSCENE_H
