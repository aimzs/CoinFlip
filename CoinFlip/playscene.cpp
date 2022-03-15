#include "playscene.h"
#include <QDebug>
#include<QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include <QTimer>
#include <QLabel>
#include "mycoin.h"
#include "dataconfig.h"
#include<QPropertyAnimation>
#include<QSound>
//PlayScene::PlayScene(QWidget *parent) : QMainWindow(parent)
//{

//}

PlayScene::PlayScene(int levelNum)
{
    //QString str = QString("进入了第 %1 关").arg(levelNum);
    //qDebug() << str;
    this->levelIndex = levelNum;

    //初始化游戏场景
    //设置固定大小
    this->setFixedSize(400,600);
    //设置图标
    this->setWindowIcon(QIcon(":/res/Coin0001.png"));
    //设置标题

    this->setWindowTitle("翻金币场景");
    //创建菜单栏
    QMenuBar *bar = menuBar();
    setMenuBar(bar);
    //创建开始菜单
    QMenu *startMenu = bar->addMenu("开始");
    //创建退出 菜单项
    QAction *quitAction = startMenu->addAction("退出");
    //退出按钮的实现

    connect(quitAction,&QAction::triggered,this,[=](){
       this->close();
    });
    //添加音效
    QSound *backSound = new QSound(":/res/BackButtonSound.wav",this);
    QSound *flipSound = new QSound(":/res/ConFlipSound.wav",this);
    QSound *winSound =  new QSound(":/res/LevelWinSound.wav",this);
    //返回按钮
    MyPushButton *backBtn = new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(),this->height()-backBtn->height());

    //点击返回
    connect(backBtn,&MyPushButton::clicked,this,[=](){
        //qDebug() << "金币场景中点击了返回按钮";
        backSound->play();
        //告诉主场景 我返回了，主场景监听ChooseLevelScene的返回按钮
        //延时返回
        QTimer::singleShot(300,this,[=](){
            emit this->SceneBack();
        });
    });
    //显示 当前的关卡数
    QLabel * label = new QLabel;
    label->setParent(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    label->setFont(font);
    QString str = QString("Level: %1").arg(this->levelIndex);
    label->setText(str);
    label->setGeometry(QRect(30,this->height()-70,150,50));


    dataConfig config;
    //初始化每个关卡的二维数组
    for(int i =0;i<4;i++)
    {
        for(int j =0 ; j<4 ; j++)
        {
            this->gameArray[i][j] = config.mData[this->levelIndex][i][j];
        }
    }

    //胜利图片显示
    QLabel *WinLabel = new QLabel;
    QPixmap tmpPix;
    tmpPix.load(":/res/LevelCompletedDialogBg.png");
    WinLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
    WinLabel->setPixmap(tmpPix);
    WinLabel->setParent(this);
    WinLabel->move((this->width() - tmpPix.width())*0.5,-tmpPix.height());


    //创建金币的背景图片
    for (int i = 0 ;i<4 ;i++ )
    {
        for(int j=0; j <4 ;j++)
        {
            //绘制背景图片
            QLabel *label = new QLabel;
            QPixmap pix = QPixmap(":/res/BoardNode.png");
            label->setGeometry(0,0,pix.width(),pix.height());
            label->setPixmap(pix);
            label->setParent(this);
            label->move(85+i*60,200+j*60);


            //创建金币
            QString str;
            if(this->gameArray[i][j] == 1)
            {
                //显示金币
                str = ":/res/Coin0001.png";
            }else
            {
                str = ":/res/Coin0008.png";
            }
            MyCoin *coin = new MyCoin(str);
            coin->setParent(this);
            coin->move(87+i*60,204+j*60);

            //给金币属性赋值
            coin->posX = i;
            coin->posY = j;
            coin->flag = this->gameArray[i][j]; // 1正面 0反面

            //将金币放入到二维数组里 以便后期维护
            coinBtn[i][j] = coin;
            //点击金币 进行翻转
            connect(coin,&MyCoin::clicked,this,[=](){
                //播放翻金币的音效
                flipSound->play();
                for (int i =0;i<4 ;i++ ) {
                    for(int j=0;j<4;j++)
                    {
                        this->coinBtn[i][j]->isWin = true;
                    }
                }
                coin->changeFlag();
                //更新 二维数组中的值
                this->gameArray[i][j] = this->gameArray[i][j]== 0? 1:0;

                //翻转周围硬币，延时翻转
                QTimer::singleShot(250,this,[=](){
                    if(coin->posX +1 <= 3 )//周围右侧金币翻转的条件
                    {
                        coinBtn[coin->posX+1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX+1][coin->posY] = this->gameArray[coin->posX+1][coin->posY]== 0? 1:0;
                    }
                    //周围的左侧硬币翻转条件
                    if(coin->posX -1 >=0 )
                    {
                        coinBtn[coin->posX-1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX-1][coin->posY] = this->gameArray[coin->posX-1][coin->posY]== 0? 1:0;
                    }
                    //周围的上侧
                    if(coin->posY +1 <=3 )
                    {
                        coinBtn[coin->posX][coin->posY+1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY+1] = this->gameArray[coin->posX][coin->posY+1]== 0? 1:0;
                    }
                    //下侧
                    if(coin->posY -1 >=0 )
                    {
                        coinBtn[coin->posX][coin->posY-1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY-1] = this->gameArray[coin->posX][coin->posY-1]== 0? 1:0;
                    }
                    for (int i =0;i<4 ;i++ ) {
                        for(int j=0;j<4;j++)
                        {
                            this->coinBtn[i][j]->isWin = false;
                        }
                    }

                    //判断 是否胜利
                    this->isWin = true;
                    for(int i = 0 ;i<4;i++)
                    {
                        for(int j=0;j<4;j++)
                        {
                            if(coinBtn[i][j]->flag == false) // 只要有一个是反面那就是失败
                            {
                                this->isWin = false;
                                break;
                            }
                        }
                    }
                    if(this->isWin == true)
                    {
                        winSound->play();
                        //胜利了
                        //qDebug() << "游戏胜利了！";
                        //将所有按钮的胜利的标志改为true
                        for(int i = 0 ;i<4;i++)
                        {
                            for(int j=0;j<4;j++)
                            {
                                coinBtn[i][j]->isWin = true;
                            }
                        }
                        //将胜利的图片移动下来
                        QPropertyAnimation *animation = new QPropertyAnimation(WinLabel,"geometry");
                        //设置时间间隔
                        animation->setDuration(1000);
                        //设置开始位置
                        animation->setStartValue(QRect(WinLabel->x(),WinLabel->y(),WinLabel->width(),WinLabel->height()));
                        //设置结束位置
                        animation->setEndValue(QRect(WinLabel->x(),WinLabel->y()+120,WinLabel->width(),WinLabel->height()));
                        //设置缓和曲线
                        animation->setEasingCurve(QEasingCurve::OutBounce);
                        //执行动画
                        animation->start();
                    }
                });

            });
        }
    }
}

void PlayScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //加载标题
    pix.load(":res/Title.png");
    pix = pix.scaled(pix.width()*0.5,pix.height()*0.5);
    painter.drawPixmap(10,30,pix.width(),pix.height(),pix);
}
