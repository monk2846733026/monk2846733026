#include "pixload.h"

Pixload::Pixload()
{
    //加载图片
    game_back_ground.load(QString(GAME_BACK_GROUND));
    game_back_ground=game_back_ground.scaled(GAME_WIDTH*GAME_MR,GAME_HEIGHT*GAME_MR,Qt::KeepAspectRatioByExpanding);

    mla_g.load(QString(MLA_G));
    mla_g=mla_g.scaled(SIZE*GAME_MR,SIZE*GAME_MR,Qt::IgnoreAspectRatio);

    mla_jump_sm.load(QString(MLA_JUMP_SM));
    mla_jump_sm=mla_jump_sm.scaled(SIZE*GAME_MR,SIZE*GAME_MR,Qt::IgnoreAspectRatio);

    mla_walk_sm.load(QString(MLA_WALK_SM));
    mla_walk_sm=mla_walk_sm.scaled(SIZE*GAME_MR,SIZE*GAME_MR,Qt::IgnoreAspectRatio);

    mla_stand_sm.load(QString(MLA_STAND_SM));
    mla_stand_sm=mla_stand_sm.scaled(SIZE*GAME_MR,SIZE*GAME_MR,Qt::IgnoreAspectRatio);//大小

}
