#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QMainWindow>

namespace Ui {
class TestWindow;
}

class TestWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TestWindow(QWidget *parent = nullptr);
    ~TestWindow();
    static bool flag_sound ;
    static bool flag_win;

private:
    Ui::TestWindow *ui;
};

#endif // TESTWINDOW_H
