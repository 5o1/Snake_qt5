#include "Snake.h"
#include "ui_Snake.h"

bool isRun;          //运行状态
bool isWin;          //是否胜利 1胜利 0失败 ！待优化
double speed;        //蛇形速度，体现在scene刷新频率上
long score;          //积分， ！计分函数待优化
long maxscore;       //最高分数
int den[40][40];     //密度
int maxden;          //最大密度=tail_density
int level;               //难度系数
int dir, tempdir;    //按键
int leng;            //身体长度，可以替代maxden
int tx,ty;           //尾部坐标
int hx, hy;          //头部坐标


Snake::Snake(QWidget  * parent) :
    QMainWindow(parent),
    ui(new Ui::Snake)
{
    ui -> setupUi(this);

     //只在初始化运行：加载主界面图片
    pix.load(":/images/Menu.png");

     //建立初始化timer1对象，用于刷新screen
    timer1 = new QTimer(this);
    connect(timer1,SIGNAL(timeout()),this,SLOT(screenupdate()));
    timer1 -> start(33);              //帧数
}


Snake::~Snake()
{
    delete ui;
}

void Snake::paintEvent(QPaintEvent  * )
{

    QPainter painter(this);


    if(!isRun)
    {
         //加载图片
    pix = pix.scaled(528,352,Qt::IgnoreAspectRatio);
    painter.drawPixmap(0,0,528,352,pix);
    }
    else
    {
         //画上scene： ( head || wall )1 or   - 1 =black
         //           body >=2 = darkgary
         //           food   - 2 = small  - red
         //           blank 0 = while
         //           空缺 绘图用：  - 3 = red
        for (int i = 0; i <= 32; ++i)
            for (int j = 0; j <=21; ++j)
                if(den[i][j] ==   - 1|| den[i][j] == 1)
                {
                    painter.setPen(Qt::black);
                    painter.setBrush(Qt::black);
                    painter.drawRect(i * 16,j * 16,16,16);
                }
                else if(den[i][j] >= 2)
                {

                    painter.setPen(Qt::darkGray);
                    painter.setBrush(Qt::darkGray);
                    painter.drawRect(i * 16,j * 16,16,16);
                }
                else if(den[i][j] ==   - 2)
                {
                    painter.setPen(Qt::white);
                    painter.setBrush(Qt::white);
                    painter.drawRect(i * 16,j * 16,16,16);
                    painter.setPen(Qt::red);
                    painter.setBrush(Qt::red);
                    painter.drawRect(i * 16 + 5,j * 16 + 5,6,6);
                }
                else if(den[i][j] == 0)
                {

                    painter.setPen(Qt::white);
                    painter.setBrush(Qt::white);
                    painter.drawRect(i * 16,j * 16,16,16);
                }
                else if(den[i][j]==   - 3)
                {

                    painter.setPen(Qt::red);
                    painter.setBrush(Qt::red);
                    painter.drawRect(i * 16,j * 16,16,16);
                }
    }
}


void Snake::keyPressEvent(QKeyEvent  * event)
{

    QKeyEvent  * key = (QKeyEvent * )event;
    switch(key -> key())
    {
    case Qt::Key_R:
        if(isRun)
            Snake::gameover();
        Snake::gamestart();
        break;
    case Qt::Key_Up:
        tempdir = 1;
        break;
    case Qt::Key_Right:
        tempdir = 2;
        break;
    case Qt::Key_Down:
        tempdir = 3;
        break;
    case Qt::Key_Left:
        tempdir = 4;
        break;
    default:
        break;
    }

}

void Snake::screenupdate()
{
    repaint();
}

void Snake::sceneupdate()
{
    hx = 0; hy = 0;
    maxden = 1;
    switch(tempdir)
    {
    case 1:
        if(dir != 3)
            dir = 1;
        break;
    case 2:
        if(dir != 4)
            dir = 2;
        break;
    case 3:
        if(dir != 1)
            dir = 3;
        break;
    case 4:
        if(dir != 2)
            dir = 4;
        break;
    default:
        break;
    }
    for (int i = 1;i <= 31; ++i)    //全部身体密度 + 1
    {
        for (int j = 1;j<=20; ++j)
        {
            if(den[i][j] == 1)
            {
                hx = i;hy = j;
            }
            if(den[i][j] >= 1)
            {
                ++den[i][j];
                if(den[i][j] >= maxden)
                {
                    tx = i;
                    ty = j;
                    maxden = den[i][j];
                }
            }

        }

    }
    den[tx][ty] = 0;
     //判断头部
    switch (dir)
    {
    case 1:
        Snake::head(hx,hy  - 1);
        break;
    case 2:
        Snake::head(hx + 1,hy);
        break;
    case 3:
        Snake::head(hx,hy + 1);
        break;
    case 4:
        Snake::head(hx  - 1,hy);
        break;
    default:
        break;
    }

    ui -> label_3 -> setText(QString::number(maxscore));
    ui -> label_4 -> setText(QString::number(score));
    ui -> label_6 -> setText(QString::number(speed));

    timer2 -> setInterval((int)((100 * (7  - level))  - (speed  - 1) * (((100 * (7  - level)  - (235  - 35 * level))) / 19)));    //((上限)  - (speed  - 1) * (((上限  - 下限) / 19)
}

int Snake::random(int x)
{
    srand((int)time(0));
    return ((int)rand()%x);
}

void Snake::food()
{
    int x,y;
    if(den[x = (Snake::random(31)) + 1][y = (Snake::random(20)) + 1] == 0)
        den[x][y] =   - 2;
    else
        Snake::food();
}



void Snake::head(int x,int y)   //判断头部函数
{
    if(den[x][y] == 0)
        den[x][y] = 1;
    else if(den[x][y] ==   - 2)
    {
        den[tx][ty] = maxden;
        den[x][y] = 1;
        ++leng;
        score = score + leng * speed / random(10);
        if (score >= maxscore)
            maxscore = score;
        Snake::food();
        if(speed <= 20)
            speed = speed + 0.475;
        else
            speed = 20;
    }
    else
    {
        Snake::gameover();
    }
}


void Snake::gamestart()
{
    score = 0;
    speed = 1;
    leng = 1;
    level = 1;    //变量初始化

    level = (int)(ui -> comboBox -> currentIndex()) + 1;     //读取难度

    for (int i = 1;i <= 31; ++i)
    {
        for(int j = 1;j <= 20; ++j)
            den[i][j] = 0;
    }
    for (int i = 0;i <= 32; ++i)
    {
        if(i == 0||i == 32)
            for(int j = 0;j <= 21; ++j)
                den[i][j] =   - 1;
        else
            for(int j = 0;j <= 21; j = j + 21)
                den[i][j] =   - 1;
    }
    den[(Snake::random(12)) + 10][(Snake::random(12)) + 6] = 1;   //在6  - 17格（中间）放蛇

    Snake::food();
    dir = Snake::random(4) + 1;

    isRun = 1;
    timer2 = new QTimer(this);
    connect(timer2,SIGNAL(timeout()),this,SLOT(sceneupdate()));
    timer2 -> start(500);
}

void Snake::gameover()
{
    timer2 -> stop();
    isRun = 0;
    if(leng >= 620)
    {
        isWin = 1;
        pix.load(":/images/win.png");
    }
    else
    {
        pix.load(":/images/gameover.png");
    }
}
