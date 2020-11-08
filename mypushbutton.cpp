#include "mypushbutton.h"
#include <QDateTime>
#include<QDebug>
#include<QPropertyAnimation>
//MyPushButton::MyPushButton(QWidget *parent) : QPushButton(parent)
//{

//}
MyPushButton::MyPushButton(QString normalImg,QString pressImg ){
    normalImgPath = normalImg;
    pressedImgPath = pressImg;
    QPixmap pixmap;
    bool ret = pixmap.load(normalImgPath);
    if(!ret){
        qDebug() << "加载图片失败";
    }
    this->setFixedSize (pixmap.width (),pixmap.height ());
    this->setStyleSheet ("QPushButton{border:0px;}");
    this->setIcon (pixmap);
    this->setIconSize (QSize(pixmap.width (),pixmap.height ()));

}
void MyPushButton::zoom1(){
    //创建动画对象
    QPropertyAnimation * animation1 = new QPropertyAnimation(this,"geometry");
    animation1->setDuration (200);
    animation1->setStartValue (QRect(this->x (),this->y (),this->width (),this->height ()));
    animation1->setEndValue (QRect(this->x (),this->y ()+10,this->width (),this->height ()));
    animation1->setEasingCurve (QEasingCurve::OutBounce);
    animation1->start ();
}
void MyPushButton::zoom2(){
    //创建动画对象
    QPropertyAnimation * animation1 = new QPropertyAnimation(this,"geometry");
    animation1->setDuration (200);
    animation1->setStartValue (QRect(this->x (),this->y ()+10,this->width (),this->height ()));
    animation1->setEndValue (QRect(this->x (),this->y ()-10,this->width (),this->height ()));
    animation1->setEasingCurve (QEasingCurve::OutBounce);
    animation1->start ();
}
void MyPushButton::mousePressEvent(QMouseEvent *e){
    if(pressedImgPath != ""){
        QPixmap pixmap;
        bool ret = pixmap.load (pressedImgPath);
        if(!ret){
            qDebug() << "加载图片失败";
        }
        this->setFixedSize (pixmap.width (),pixmap.height ());
        this->setStyleSheet ("QPushButton{border:0px;}");
        this->setIcon (pixmap);
        this->setIconSize (QSize(pixmap.width (),pixmap.height ()));
    }
    //按下时间
//    QDateTime current_date_time = QDateTime::currentDateTime();
//    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm::ss.zzz");
//    qDebug() <<"按下事件处理时间："<< current_date.toUtf8 ().data ();

    return QPushButton::mousePressEvent (e);
}
void MyPushButton::mouseReleaseEvent(QMouseEvent *e){
    if(pressedImgPath != ""){
        QPixmap pixmap;
        bool ret = pixmap.load (normalImgPath);
        if(!ret){
            qDebug()<<normalImgPath << "加载图片失败?";
        }
        this->setFixedSize (pixmap.width (),pixmap.height ());
        this->setStyleSheet ("QPushButton{border:0px;}");
        this->setIcon (pixmap);
        this->setIconSize (QSize(pixmap.width (),pixmap.height ()));
    }
//    //交给父类执行按下时间
//    QDateTime current_date_time = QDateTime::currentDateTime();
//    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm::ss.zzz");
//    qDebug() <<"释放按钮事件处理时间："<< current_date.toUtf8 ().data ();
    //交给父类执行
    return QPushButton::mouseReleaseEvent (e);
}
