#ifndef SNAKE_H
#define SNAKE_H

#include <QMainWindow>  //窗口主类
#include <QTimer>       //计时触发器
#include <QKeyEvent>    //键盘按键事件
#include <QPainter>     //窗口绘图
#include <stdlib.h>     //随机数
#include <time.h>       //获取系统时间用
#include <QString>      //Label要求QString类型
#include <QLabel>       //标签
#include <QComboBox>    //下拉框


namespace Ui {
class Snake;
}

class Snake : public QMainWindow
{
    Q_OBJECT
public:
    explicit Snake(QWidget *parent = 0);
    ~Snake();
protected:
    void paintEvent(QPaintEvent*);
    void keyPressEvent(QKeyEvent *key);
private:
    Ui::Snake *ui;
    QTimer *timer1;
    QTimer *timer2;
    QPixmap pix;
    QLabel *label_3;
    QLabel *label_4;
    QComboBox *comboBox;
    void food();
    void gameover();
    void head(int x,int y);
    void gamestart();
    int random(int x);
private slots:
    void screenupdate();
    void sceneupdate();
};



#endif // SNAKE_H
