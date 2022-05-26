#ifndef CONFIG_H
#define CONFIG_H


/******************    游戏配置    ******************/
#define GAME_TITLE "MyMario"                                             //标题
#define GAME_WIDTH 448                                                  //宽度//总计3392像素
#define GAME_HEIGHT 224                                                 //高度
#define GAME_GROUND_HEIGHT 24                                          //地面高度
#define SIZE 16                                                         //图片16*16
#define GAME_MR 3                                                       //放大系数MAGNIFY_RATIO
#define GAME_TIME 10                                                    //  10毫秒一刷
#define GAME_GRV  6                                                      //引力grvitation
#define GRV_V0 10                                                       //  初速度
#define TIME_COUNTS 10                                                  //10次
#define GAME_RES_PATH "./source.rcc"                                    //二进制资源文件路径
#define GAME_ICO ":/ico/res/ico/mario.ico"                               //窗口ico
#define GAME_BACK_GROUND ":/back_ground/res/graphics/level_1.png"          //背景
#define GAME_MUSIC "qrc:/music/res/music/main-theme.mp3"                    //背景音乐
#define GAME_GROUND1_X 0
#define GAME_GROUND2_X 1135
#define GAME_GROUND3_X 1423
#define GAME_GROUND4_X 2479

/******************    马里奥MLA配置    ******************/
#define MLA_G ":/g_sm/res/graphics/mario_sm/g.png"                          //  马里奥死亡
#define MLA_WALK_SM ":/walk_sm/res/graphics/mario_sm/walk1.png"             //马里奥走路
#define MLA_STAND_SM ":/stand_sm/res/graphics/mario_sm/stand.png"              //马里奥站立
#define MLA_JUMP_SM ":/jump_sm/res/graphics/mario_sm/jump.png"                 //马里奥跳跃




#endif // CONFIG_H
