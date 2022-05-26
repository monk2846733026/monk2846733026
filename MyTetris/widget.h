#ifndef WIDGET_H
#define WIDGET_H

#include <QTime>
#include <QDebug>
#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QMessageBox>
#define BLOCK_SIZE 25    //单个方块单元的边长
#define MARGIN 5         //场景边距
#define AREA_ROW 20      //场景行数
#define AREA_COL 12      //场景列数

// 枚举 按键
enum Direction
{
    UP,         //  上方向
    DOWN,       //  下方向
    LEFT,       //  左方向
    RIGHT,      //  右方向
    SPACE,      //  空格
    STOP,       //  S停止
    RESTART,    //  R重开
    CLOSE,      //  C关闭
    NOTSPIN     //  N停止旋转功能
};

//定义边界信息
struct Border//边界
{
    int ubound;     //  up 上边界
    int dbound;     //  down 下边界
    int lbound;     //  left 左边界
    int rbound;     //  right 右边界
};


//坐标
struct block_point
{
    int pos_x;
    int pos_y;
};


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void InitGame(); //初始化
    void StartGame();//开始游戏
    void GameOver();//结束游戏

    void CreateBlock(int block[4][4],int block_id); //创建矩阵
    void ResetBlock(); //重置
    void block_cpy(int yblock[4][4],int bblock[4][4]);//复制函数
    void BlockMove(Direction dir); //矩阵变动
    void GetBorder(int block[4][4],Border &border); //计算边界
    bool IsCollide(int x,int y,Direction dir); //判断是否会碰撞
    void ConvertStable(int x,int y); //转换为稳定方块//因为以矩阵左上角为锚点,所以只用传入左上角的锚点
    void BlockRotate(int block[4][4]); //矩阵旋转

    void paintEvent(QPaintEvent *event); //场景刷新事件
    void timerEvent(QTimerEvent *event); //定时器事件
    void keyPressEvent(QKeyEvent *event); //键盘响应

private slots:


private:
    Ui::Widget *ui;
    int game_area[AREA_ROW+1][AREA_COL+1];
    //行*列 空为0,活动为1,固定为2
    //必须多设一行，防止最后一行下移时game_area[AREA_ROW][AREA_COL]会因未定义赋值而出错
    block_point block_pos; //当前方块坐标
    int cur_block[4][4]; //当前方块形状
    Border cur_border; //当前方块边界
    int next_block[4][4]; //下一个方块形状
    bool isStable; //当前方块是否稳定了
    int score;  //游戏分数
    int game_timer; //方块下落计时器
    int paint_timer; //渲染刷新计时器
    int speed_ms; //下落时间间隔
    int refresh_ms; //刷新时间间隔
    bool isstop=false;//是否暂停游戏
    bool isspin=true;//是否开启旋转功能

};
#endif // WIDGET_H
