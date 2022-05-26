#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "config.h"
#include <QIcon>
#include<QPaintEvent>
#include<QPainter>
#include"pixload.h"
#include <QTimer>
#include <QMediaPlayer>
#include <QUrl>
#include <QRect>
#include "ground.h"
enum Key    //  按键枚举
{
    LEFT,   //  左方向
    RIGHT,  //  右方向
    SPACE,  //  跳跃
    ATTACK, //  攻击
    STOP    //  暂停
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void init_game();//初始化游戏
    void startgame();//开始游戏
    void GameOver();//游戏结束
    void updataip();//更新马里奥y坐标
    void collis();//碰撞检测
    void ActionDeal(Key dir);//人物移动

    void paintEvent(QPaintEvent *event) override;//绘画事件
    void keyPressEvent(QKeyEvent *event)override;//键盘事件
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    //QPainter  painter;//创建画家

    Pixload m_pixload;//创建pixload对象
    QTimer m_timer;//创建定时器对象
    QMediaPlayer *music;
    QRect m_mla;//创建马里奥矩形
    Ground m_ground;//创建ground

    int mlaMoveSpeed;//马里奥移动速度
    int enemyMoveSpeed;//敌人移动速度
    int backMoveSpeed;//背景移动速度

    int backCurrent_x;//背景x坐标
    int backCurrent_y;//背景y坐标
    int mlaCurrent_x;//马里奥x坐标
    int mlaCurrent_y;//马里奥y坐标
    int ground_move_tmp;//地图移动距离

    bool isleft=false;//动作状态
    bool isright=false;
    bool isspace=false;
    bool isstand=true;
    int counts;//计数

};
#endif // WIDGET_H
