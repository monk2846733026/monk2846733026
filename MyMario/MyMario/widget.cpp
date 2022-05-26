#include "widget.h"
#include <QDebug>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    init_game();//初始化
}

Widget::~Widget()
{
}

void Widget::init_game()//初始化
{
    this->setFixedSize(GAME_WIDTH*GAME_MR,GAME_HEIGHT*GAME_MR);//设置窗口大小
    this->setWindowTitle(GAME_TITLE);//设置窗口大小
    this->setWindowIcon(QIcon(GAME_ICO));//设置窗口图标

    //音乐播放
    music = new QMediaPlayer(this);
    music->setMedia(QUrl(GAME_MUSIC));
    music->play();

    m_mla.setWidth(SIZE*GAME_MR);
    m_mla.setHeight(SIZE*GAME_MR);
    m_mla.moveTo(GAME_WIDTH-200,(GAME_HEIGHT-GAME_GROUND_HEIGHT-SIZE)*GAME_MR);

    //初始化坐标
    backCurrent_x=0;
    backCurrent_y=0;
    mlaCurrent_x=GAME_WIDTH-200;
    //mlaCurrent_y=(GAME_HEIGHT-GAME_GROUND_HEIGHT-SIZE)*GAME_MR;
    mlaCurrent_y=0;

    //初始化速度
    mlaMoveSpeed=2;
    backMoveSpeed=10;
    ground_move_tmp=0;//初始化地面移动距离
    counts=20;

    m_timer.setInterval(GAME_TIME);//设置定时器间隔
    startgame();//开始游戏
}

void Widget::startgame()//开始游戏
{
    m_timer.start();//启动定时器
    connect(&m_timer,&QTimer::timeout,[=](){
        updataip();//更新马里奥y坐标
        update();//刷新painter
        collis();//碰撞检测
    });
}

void Widget::GameOver()//游戏结束
{
    m_timer.stop();
}

void Widget::updataip()//更新马里奥y坐标
{

    //if(mlaCurrent_y<(GAME_HEIGHT-GAME_GROUND_HEIGHT-SIZE)*GAME_MR)
    mlaCurrent_y+=GAME_GRV/2;
    m_mla.moveTo(mlaCurrent_x,mlaCurrent_y);
}

void Widget::collis()
{
    //if(mlaCurrent_y<(GAME_HEIGHT-GAME_GROUND_HEIGHT-SIZE)*GAME_MR)
    //mlaCurrent_y+=GAME_GRV/2;
    if(m_ground.m_ground1.intersects(m_mla))
    {
        mlaCurrent_y=(GAME_HEIGHT-GAME_GROUND_HEIGHT-SIZE)*GAME_MR+1;
        //qDebug()<<true;
    }
    else
        return;
}

void Widget::paintEvent(QPaintEvent *event)//绘画事件
{

    QPainter m_Painter(this);
    m_Painter.drawPixmap(backCurrent_x,backCurrent_y,m_pixload.game_back_ground );
    m_Painter.drawPixmap(mlaCurrent_x,mlaCurrent_y,m_pixload.mla_stand_sm);


}
void Widget::keyPressEvent(QKeyEvent *event)//键盘事件
{
    switch (event->key())
    {
    case Qt::Key_A://左移
        ActionDeal(LEFT);
        break;
    case Qt::Key_D://右移
        ActionDeal(RIGHT);
        //qDebug()<<"key_D被按下";
        break;
    case Qt::Key_J://攻击
        //ActionDeal(ATTACK);
        break;
    case Qt::Key_Space://跳跃
        ActionDeal(SPACE);
        isspace=true;
        isstand=false;
        break;
    case Qt::Key_P://暂停
        //ActionDeal(STOP);
        break;
    default:
        break;
    }
}

void Widget::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Space://跳跃
        ActionDeal(SPACE);
        isspace=false;
        //isstand=false;
        break;

    }
}
void Widget::ActionDeal(Key dir)//按键处理
{
    switch (dir) {
    case LEFT://左
        backCurrent_x+=backMoveSpeed;

//        if(mlaCurrent_x<=0)
//            mlaCurrent_x=0;
//        if(mlaCurrent_x<GAME_WIDTH*GAME_MR)
//        mlaCurrent_x-=2;

        //qDebug()<<backCurrent_x;
        break;
    case RIGHT://右
//        backCurrent_x-=backMoveSpeed;
//        ground_move_tmp+=backMoveSpeed;
//        qDebug()<<ground_move_tmp;
//        m_ground.m_ground1.moveTo(-GAME_GROUND1_X*3-ground_move_tmp,200*3);
//        if(mlaCurrent_x>=(GAME_WIDTH-SIZE)*GAME_MR)
//            mlaCurrent_x=(GAME_WIDTH-SIZE)*GAME_MR;
//        if(mlaCurrent_x<(GAME_WIDTH-SIZE)*GAME_MR)
//        mlaCurrent_x+=2;
        //qDebug()<<backCurrent_x;
        break;
    case ATTACK:

        break;
    case SPACE:
        if(isspace)
        {

        }
        else if(false==isspace)
            counts=20;




        break;
    case STOP:

        break;
    default:
        break;
    }
}
