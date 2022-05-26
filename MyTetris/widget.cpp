#include "widget.h"
#include "ui_widget.h"
#include<QDebug>
//定义图案 4*4为矩阵,1*1为方块
//田字
int icon1[4][4]=
{
    {0,0,0,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0}
};
//右L
int icon2[4][4]=
{
    {0,1,0,0},
    {0,1,0,0},
    {0,1,1,0},
    {0,0,0,0}
};
//左L
int icon3[4][4]=
{
    {0,0,1,0},
    {0,0,1,0},
    {0,1,1,0},
    {0,0,0,0}
};
//S
int icon4[4][4]=
{
    {0,1,0,0},
    {0,1,1,0},
    {0,0,1,0},
    {0,0,0,0}
};
//倒T形
int icon5[4][4]=
{
    {0,0,0,0},
    {0,0,1,0},
    {0,1,1,1},
    {0,0,0,0}
};
//I条
int icon6[4][4]=
{
    {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0}
};

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("MyTetris");//窗口重命名
    //调整窗口尺寸布局
    this->resize(AREA_COL*BLOCK_SIZE + MARGIN*4 + 8*BLOCK_SIZE,  AREA_ROW*BLOCK_SIZE + MARGIN*2);
    //12列*单个方块边长(游戏界面区域) + 边距*4像素点(边距) + 单个方块边长*8(右边绘画下一个图形当前图形和得分区域);
    InitGame();//初始化

    //for(int i=0;i<4;i++)
    //qDebug()<<icon6[i][0]<<icon6[i][1]<<icon6[i][2]<<icon6[i][3];
}

Widget::~Widget()
{
    delete ui;
}
void Widget::paintEvent(QPaintEvent *event) //场景刷新
{
    qDebug()<<"painterevent";
    QPainter painter(this);//创建画家
    //画游戏场景边框
    painter.setBrush(QBrush(Qt::white,Qt::SolidPattern));//画刷//白色且全填充
    painter.drawRect(MARGIN,MARGIN,AREA_COL*BLOCK_SIZE,AREA_ROW*BLOCK_SIZE);
    painter.setBrush(QBrush(QColor(190,190,190),Qt::SolidPattern));//画刷//灰色且全填充
    painter.drawRect(MARGIN,MARGIN,AREA_COL*BLOCK_SIZE,2*BLOCK_SIZE);
    painter.setBrush(QBrush(Qt::white,Qt::SolidPattern));//画刷//白色且全填充
    //游戏画边框,边界为margin,画区域 (12列*单个方块边长,20行*单个方块边长)
    //画游戏方格
    for(int i=1;i<=AREA_COL;++i)
        for (int j=1;j<=AREA_ROW;++j) {
            painter.drawLine(MARGIN+BLOCK_SIZE*i,MARGIN,MARGIN+BLOCK_SIZE*i,AREA_ROW*BLOCK_SIZE+MARGIN);//行
            painter.drawLine(MARGIN,MARGIN+j*BLOCK_SIZE,AREA_COL*BLOCK_SIZE+MARGIN,MARGIN+j*BLOCK_SIZE);//列
    }
    //下一方块预报区(12列*单个方块边长+2*边界margin,2行*单个方块边长+边界margin,4*单个方块边长,4*单个方块边长)(x,y,w,h)
    painter.drawRect(MARGIN*2+AREA_COL*BLOCK_SIZE,MARGIN+2*BLOCK_SIZE,4*BLOCK_SIZE,4*BLOCK_SIZE);

    //当前方块显示区(12列*单个方块边长+3*边界margin+4*单个方块边长,2行*单个方块边长+边界margin,4*单个方块边长,4*单个方块边长)(x,y,w,h)
    painter.drawRect(MARGIN*3+AREA_COL*BLOCK_SIZE+4*BLOCK_SIZE,MARGIN+2*BLOCK_SIZE,4*BLOCK_SIZE,4*BLOCK_SIZE);

    //画方块
    painter.setBrush(QBrush(Qt::blue,Qt::SolidPattern));
    for(int i=0;i<4;i++)//在绘图预报区绘制下一(next)方块
        for(int j=0;j<4;j++)
            if(next_block[j][i]==1)
                painter.drawRect(MARGIN*2+AREA_COL*BLOCK_SIZE+i*BLOCK_SIZE,
                                 MARGIN+2*BLOCK_SIZE+j*BLOCK_SIZE,
                                 BLOCK_SIZE,BLOCK_SIZE);
                //(12列*单个方块边长+2*边界margin,2行*单个方块边长+边界margin,4*单个方块边长,4*单个方块边长)(宽，高，边长，边长）

    for(int i=0;i<4;i++)//在绘图预报区绘制当前(cur)方块
        for(int j=0;j<4;j++)
            if(cur_block[j][i]==1)
                painter.drawRect(MARGIN*3+AREA_COL*BLOCK_SIZE+4*BLOCK_SIZE+i*BLOCK_SIZE,
                                 MARGIN+2*BLOCK_SIZE+j*BLOCK_SIZE,
                                 BLOCK_SIZE,BLOCK_SIZE);
                //(12列*单个方块边长+3*边界margin+4*单个方块边长,2行*单个方块边长+边界margin,4*单个方块边长,4*单个方块边长)(宽，高，边长，边长）

    //绘制得分
    painter.drawText(QRect(MARGIN*3+AREA_COL*BLOCK_SIZE+2*BLOCK_SIZE,MARGIN+5*BLOCK_SIZE,BLOCK_SIZE*4,BLOCK_SIZE*4),
                     Qt::AlignCenter,"得分: "+QString::number(score));//字符串拼接+(得分:score)
    painter.drawText(QRect(MARGIN+AREA_COL*BLOCK_SIZE,MARGIN,BLOCK_SIZE*4,BLOCK_SIZE*2),
                     Qt::AlignCenter,"下一图案：");
    painter.drawText(QRect(MARGIN+(4+AREA_COL)*BLOCK_SIZE,MARGIN,BLOCK_SIZE*4,BLOCK_SIZE*2),
                     Qt::AlignCenter,"当前图案：");
    painter.drawText(QRect(MARGIN*2+AREA_COL*BLOCK_SIZE,BLOCK_SIZE*(AREA_ROW-4),BLOCK_SIZE*8,BLOCK_SIZE*4),
                     Qt::AlignCenter,"提示：使用S可暂停游戏\n"
                                     "N不旋转挑战\n"
                                     "R重新开始\n"
                                     "C关闭游戏\n");


    //绘制下落方块和稳定方块
    for(int i=0;i<AREA_ROW;i++)//行
        for(int j=0;j<AREA_COL;j++)//列
        {
            if(game_area[i][j]==1)//绘制活动方块
            {
                painter.setBrush(QBrush(Qt::red,Qt::SolidPattern));//红色全填充
                painter.drawRect(j*BLOCK_SIZE+MARGIN,
                                 i*BLOCK_SIZE+MARGIN,
                                 BLOCK_SIZE,BLOCK_SIZE);
            }
            else if(game_area[i][j]==2)//绘制稳定方块
            {
                painter.setBrush(QBrush(Qt::green,Qt::SolidPattern));//绿色全填充
                painter.drawRect(j*BLOCK_SIZE+MARGIN,
                                 i*BLOCK_SIZE+MARGIN,
                                 BLOCK_SIZE,BLOCK_SIZE);
            }
        }
    //处理消行，整个场景上面的行依次下移
    int i=AREA_ROW-1;
    int line_count=0;
    //记消行数
    while(i>=1)
    {
        bool is_line_full=true;
        for(int j=0;j<AREA_COL;j++)
            if(game_area[i][j]!=2)
            {
                is_line_full=false;
                i--;
                break;
            }
        if(is_line_full)
        {
            for(int k=i;k>=1;k--)
                for(int j=0;j<AREA_COL;j++)
                    game_area[k][j]=game_area[k-1][j];
            line_count++;//每次增加消行的行数
        }
    }
    score+=line_count*100; //得分

}

void Widget::InitGame()//初始化
{
    memset(game_area,0,sizeof (game_area));//二维数组初始化
    speed_ms=800;//初始化游戏timer
    refresh_ms=30;//初始化界面刷新timer//1秒刷新33次,
    srand(time(NULL));//初始化随机数种子
    score=0;//分数清0
    StartGame();//开始游戏
}
void Widget::StartGame()//开始游戏
{
    game_timer=startTimer(speed_ms); //开启游戏timer
    paint_timer=startTimer(refresh_ms); //开启界面刷新timer
    //产生初始的第一个的矩阵
    int block_id=rand()%6;
    //产生矩阵
    memset(next_block,0,sizeof (next_block));
    CreateBlock(next_block,block_id);// int next_block[4][4](下一个矩阵形状)定义默认值为全0数组
    ResetBlock();//重置矩阵
}
void Widget::GameOver()//游戏结束
{
    killTimer(speed_ms);
    killTimer(refresh_ms);
    memset(next_block,0,sizeof (next_block));
    QMessageBox messagebox;
    int a = messagebox.question(this,"GameOver","游戏结束辣\n是否重新开始",
                          QMessageBox::Yes,QMessageBox::No);
    switch (a) {
    case QMessageBox::Yes:
        InitGame();
        break;
    case QMessageBox::No:
        //BlockMove(STOP);
        this->close();
        break;
    default:
        break;

    }

}
void Widget::CreateBlock(int block[4][4],int block_id)//矩阵,随机图案
{
    switch (block_id) {
    case 0:
        block_cpy(block,icon1);
        break;
    case 1:
        block_cpy(block,icon2);
        break;
    case 2:
        block_cpy(block,icon3);
        break;
    case 3:
        block_cpy(block,icon4);
        break;
    case 4:
        block_cpy(block,icon5);
        break;
    case 5:
        block_cpy(block,icon6);
        break;
    default:
        break;
    }
}
void Widget::ResetBlock(){
    block_cpy(cur_block,next_block);//cpy给当前矩阵
    GetBorder(cur_block,cur_border);//获取当前矩阵边界

    //产生下一个矩阵
    int block_id=rand()%6;
    CreateBlock(next_block,block_id);

    //设置初始方块坐标,以矩阵左上角为锚点
    block_point start_point;
    start_point.pos_x=AREA_COL/2-2;//第4格为起始坐标
    start_point.pos_y=-1;
    //判断游戏是否结束
    for(int j=0;j<AREA_COL;j++)
        if(game_area[0][j]==2) //最顶端也有稳定方块
            GameOver();
    //防止最后一次发生与前面方块之间融合
    for(int i=0;i<AREA_COL;++i)
        if(game_area[2][i]==2) //最顶端也有稳定方块
        {
            start_point.pos_y=-3;//直接出游戏区//防止消掉最上排的方块
            GameOver();
            break;
        }
    block_pos=start_point;
}

//拷贝矩阵中方块图案，
void Widget::block_cpy(int yblock[4][4],int bblock[4][4])
{
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            yblock[i][j]=bblock[i][j];
}

void  Widget::timerEvent(QTimerEvent *event) //定时器事件
{
    //if(event->timerId()==game_timer)//方块下落
        //BlockMove(DOWN);

    if(event->timerId()==paint_timer)//刷新画面
        update();
}

void Widget::GetBorder(int block[4][4],Border &border){ //计算边界
    //计算上下左右边界
    //按理来说应该是上下左右,但算出来的结果是下上右左
    //不知道为什么旋转了180度
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(block[i][j]==1)
            {
                border.dbound=i;
                break;
            }
    for(int i=3;i>=0;i--)
        for(int j=0;j<4;j++)
            if(block[i][j]==1)
            {
                border.ubound=i;
                //qDebug()<<i;
                break;
            }
    for(int j=0;j<4;j++)
        for(int i=0;i<4;i++)
            if(block[i][j]==1)
            {
                border.rbound=j;
                break;
            }
    for(int j=3;j>=0;j--)
        for(int i=0;i<4;i++)
            if(block[i][j]==1)
            {
                border.lbound=j;
                break;
            }
    qDebug()<<cur_border.ubound<<cur_border.dbound<<cur_border.lbound<<cur_border.rbound;
}

void Widget::keyPressEvent(QKeyEvent *event){ //键盘响应
    switch(event->key())
    {
    case Qt::Key_Up:
        BlockMove(UP);
        break;
    case Qt::Key_Down:
        BlockMove(DOWN);
        break;
    case Qt::Key_Left:
        BlockMove(LEFT);
        break;
    case Qt::Key_Right:
        BlockMove(RIGHT);
        break;
    case Qt::Key_Space:
        BlockMove(SPACE);
        break;
    case Qt::Key_S:
        BlockMove(STOP);
        break;
    case Qt::Key_R:
        BlockMove(RESTART);
        break;
    case Qt::Key_C:
        BlockMove(CLOSE);
    case Qt::Key_N:
        BlockMove(NOTSPIN);
    default:
        break;
    }
}

//移动方块
void Widget::BlockMove(Direction dir){
    switch (dir) {

    case UP:
        if(true==isstop)//如果是停止,不操作,break
            break;
        if(false==isspin)//如果无旋转功能,不操作,break
            break;
        if(IsCollide(block_pos.pos_x,block_pos.pos_y,UP))//先判断旋转是否会碰撞
            break;
        //逆时针旋转90度
        BlockRotate(cur_block);
        //防止旋转后bug,i和j从0到4重新设置方块,否则显示的方块会包括之前的方块
        for(int i=0;i<4;i++)
            for(int j=0;j<4;j++)
                if(game_area[block_pos.pos_y+i][block_pos.pos_x+j]!=2)
                  game_area[block_pos.pos_y+i][block_pos.pos_x+j]=cur_block[i][j];

    GetBorder(cur_block,cur_border);//重新计算旋转后的边界

    case DOWN:
        if(true==isstop)//停止,不操作,break
            break;

        //方块到达底部边界则不再移动
        if(block_pos.pos_y+cur_border.dbound==AREA_ROW-1)
        {
            ConvertStable(block_pos.pos_x,block_pos.pos_y);
            ResetBlock();
            break;
        }
        //碰撞检测，计算下边界，尝试走一格，如果碰撞底部则稳定方块后跳出
        if(IsCollide(block_pos.pos_x,block_pos.pos_y,DOWN))
        {
            //只有最终不能下落才转成稳定方块
            ConvertStable(block_pos.pos_x,block_pos.pos_y);
            ResetBlock();
            break;
        }

        //恢复方块上场景,清除移动过程中的方块残留//上擦除
        for(int j=cur_border.lbound;j<=cur_border.rbound;j++)
            if(game_area[block_pos.pos_y+cur_border.ubound][block_pos.pos_x+j]!=2)// !2,不检测稳定方块会擦掉稳定方块
                game_area[block_pos.pos_y+cur_border.ubound][block_pos.pos_x+j]=0;

        //没有碰撞则下落一格
        block_pos.pos_y+=1;
        //qDebug()<<block_pos.pos_x<<block_pos.pos_y;
        //方块下降一格，拷贝到game_area,注意不能只考虑上下边界,还要左右边界
        for(int i=cur_border.ubound;i<=cur_border.dbound;i++)
            for(int j=cur_border.lbound;j<=cur_border.rbound;j++)
                if(block_pos.pos_y+i<=AREA_ROW-1&&game_area[block_pos.pos_y+i][block_pos.pos_x+j]!=2)
                    game_area[block_pos.pos_y+i][block_pos.pos_x+j]=cur_block[i][j];
        break;

    case LEFT:
        if(true==isstop)//停止,不操作,break
            break;

        //到左边界或者碰撞则不再往左,break
        if(block_pos.pos_x+cur_border.lbound==0||IsCollide(block_pos.pos_x,block_pos.pos_y,LEFT))
            break;

        //恢复方块右场景,清除移动过程中的方块残留
        for(int i=cur_border.ubound;i<=cur_border.dbound;i++)
            if(game_area[block_pos.pos_y+i][block_pos.pos_x+cur_border.rbound]!=2)
            game_area[block_pos.pos_y+i][block_pos.pos_x+cur_border.rbound]=0;
        block_pos.pos_x-=1;

        //方块左移一格，拷贝到game_area
        for(int i=cur_border.ubound;i<=cur_border.dbound;i++)
            for(int j=cur_border.lbound;j<=cur_border.rbound;j++)
                if(block_pos.pos_x+j>=0&&game_area[block_pos.pos_y+i][block_pos.pos_x+j]!=2)
                    game_area[block_pos.pos_y+i][block_pos.pos_x+j]=cur_block[i][j];
        break;

    case RIGHT:
        if(true==isstop)//停止,不操作,break
            break;
         //到右边界或者碰撞则不再往右,break
        if(block_pos.pos_x+cur_border.rbound==AREA_COL-1||IsCollide(block_pos.pos_x,block_pos.pos_y,RIGHT))
            break;
        //恢复方块左场景,清除移动过程中的方块残留
        for(int i=cur_border.ubound;i<=cur_border.dbound;i++)
            if(game_area[block_pos.pos_y+i][block_pos.pos_x+cur_border.lbound]!=2)
            game_area[block_pos.pos_y+i][block_pos.pos_x+cur_border.lbound]=0;
        block_pos.pos_x+=1;
        //方块右移一格，拷贝到game_area
        for(int i=cur_border.ubound;i<=cur_border.dbound;i++)
            for(int j=cur_border.lbound;j<=cur_border.rbound;j++)
                if(block_pos.pos_x+j<=AREA_COL-1&&game_area[block_pos.pos_y+i][block_pos.pos_x+j]!=2)
                    game_area[block_pos.pos_y+i][block_pos.pos_x+j]=cur_block[i][j];
        break;

    case SPACE://就是一直down
        if(true==isstop)
            break;
        while(block_pos.pos_y+cur_border.dbound<AREA_ROW-1&&!IsCollide(block_pos.pos_x,block_pos.pos_y,DOWN))
             {
                    //恢复方块上场景,清除移动过程中的方块残留
                    for(int j=cur_border.lbound;j<=cur_border.rbound;j++)
                        if(game_area[block_pos.pos_y+cur_border.ubound][block_pos.pos_x+j]!=2)
                        game_area[block_pos.pos_y+cur_border.ubound][block_pos.pos_x+j]=0;
                    //没有碰撞则下落一格
                    block_pos.pos_y+=1;
                    //方块下降一格，拷贝到game_area
                    for(int i=cur_border.ubound;i<=cur_border.rbound;i++)
                        for(int j=cur_border.lbound;j<=cur_border.rbound;j++)
                            if(block_pos.pos_y+i<=AREA_ROW-1&&game_area[block_pos.pos_y+i][block_pos.pos_x+j]!=2) //注意场景数组不越界,而且不会擦出稳定的方块
                                game_area[block_pos.pos_y+i][block_pos.pos_x+j]=cur_block[i][j];
             }
                ConvertStable(block_pos.pos_x,block_pos.pos_y);
                ResetBlock();
        break;

    case NOTSPIN:
        if(true==isstop)//停止,不操作,break
            break;

        if(true==isspin)
            isspin=false;
        else
            isspin=true;
        break;

    case RESTART:
        InitGame();
        break;

    case STOP:
        if(isstop==false)
        {
            killTimer(game_timer);
            killTimer(paint_timer);
            isstop = true ;
        }
        else
        {
            game_timer=startTimer(speed_ms); //开启游戏timer
            paint_timer=startTimer(refresh_ms); //开启界面刷新timer
            isstop = false;
        }
        break;

    case CLOSE:
        BlockMove(STOP);

        QMessageBox messagebox;
        int a = messagebox.question(this,"GameOver","结术游戏\n是否关闭",
                              QMessageBox::Yes,QMessageBox::No);
        switch (a) {
        case QMessageBox::Yes:
            this->close();
            break;
        case QMessageBox::No:
            BlockMove(STOP);

            break;
        default:
            break;

        }
        break;
    }
}
bool Widget::IsCollide(int x,int y,Direction dir) //判断是否会碰撞
{
    //不能用原始矩阵做判断,只能用拷贝的临时矩阵做判断
    int temp_block[4][4];
    block_cpy(temp_block,cur_block);
    Border temp_border;//临时矩阵边界
    GetBorder(temp_block,temp_border);

    //先尝试按照某方向走一格
    switch(dir)
    {
    case UP:
        BlockRotate(temp_block);
        GetBorder(temp_block,temp_border); //旋转后要重新计算边界
        break;
    case DOWN:
        y+=1;
        break;
    case LEFT:
        x-=1;
        break;
    case RIGHT:
        x+=1;
        break;
    default:
        break;
    }

    for(int i=temp_border.ubound;i<=temp_border.dbound;i++)
        for(int j=temp_border.lbound;j<=temp_border.rbound;j++)
            if((game_area[y+i][x+j]==2&&temp_block[i][j]==1)||x+temp_border.lbound<0||x+temp_border.rbound>AREA_COL-1||y+temp_border.dbound>AREA_ROW-1)
            //==2说明有稳定方块,==1说明这个位置有temp方块,俩重叠,那一定是发生碰撞,不能只检测==2,因为是边界判断并不能判断到单独的方块
            //x+temp_border.lbound<0仅对应LEFT，x+temp_border.rbound>AREA_COL-1仅对应RIGHT
            //方块格子和场景格子在重合的地方则判断为碰撞了//超出边界也算碰撞了
                return true;
    return false;
}

void Widget::ConvertStable(int x,int y) //转换为稳定方块
{
    for(int i=cur_border.ubound;i<=cur_border.dbound;i++)
        for(int j=cur_border.lbound;j<=cur_border.rbound;j++)
            if(cur_block[i][j]==1)
                game_area[y+i][x+j]=2; //x和y别搞反
}

void Widget::BlockRotate(int block[4][4])//矩阵旋转
{
    int temp_block[4][4];
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            temp_block[3-j][i]=block[i][j];
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            block[i][j]=temp_block[i][j];
}
