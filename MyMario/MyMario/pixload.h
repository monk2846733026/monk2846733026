#ifndef PIXLOAD_H
#define PIXLOAD_H
#include"config.h"
#include<QPixmap>
#include<QString>
class Pixload
{
public:
    Pixload();

    QPixmap game_back_ground;//游戏背景

    QPixmap mla_g;//马里奥死亡
    QPixmap mla_jump_sm;//马里奥跳跃
    QPixmap mla_stand_sm;//马里奥站立
    QPixmap mla_walk_sm;//马里奥走
};

#endif // PIXLOAD_H
