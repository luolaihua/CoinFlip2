#ifndef MYCOIN_H
#define MYCOIN_H

#include <QPushButton>

class MyCoin : public QPushButton
{
    Q_OBJECT
public:
    //explicit MyCoin(QWidget *parent = nullptr);
    MyCoin(QString butImg);//代表图片路径
    //这三个属性分别代表了，该金币在二维数组中 x的坐标，y的坐标，以及当前的正反标志。
    int posX; //x坐标
    int posY; //y坐标
    bool flag; //正反标志
    void changeFlag();//改变标志,执行翻转效果
    QTimer *timer1; //正面翻反面 定时器
    QTimer *timer2; //反面翻正面 定时器
    int min = 1; //最小图片
    int max = 8; //最大图片
    bool isAnimation  = false; //做翻转动画的标志
    void mousePressEvent(QMouseEvent *);
    //默认设置为true，只要有一个反面的金币，就将该值改为false，视为未成功。
    bool isWin = false;//胜利标志

signals:

};

#endif // MYCOIN_H
