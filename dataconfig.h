#ifndef DATACONFIG_H
#define DATACONFIG_H

#include <QObject>
#include <QMap>
#include <QVector>
class dataConfig : public QObject
{
    Q_OBJECT
public:
    explicit dataConfig(QObject *parent = 0);

public:
    /*
     * QMap<int,QVector<QVector<int>>>mData;都记录着每个关卡中的数据。
     * 其中，int代表对应的关卡 ，也就是QMap中的key值，而value值就是对应的二维数组，
     * 利用的是 QVector<QVector<int>>来记录着其中的二维数组。
*/
    QMap<int, QVector< QVector<int> > >mData;



signals:

public slots:
};

#endif // DATACONFIG_H
