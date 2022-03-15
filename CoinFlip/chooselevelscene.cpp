// 源文件编码保存为 “utf-8 带有 BOM ”
#include "chooselevelscene.h"
#include<QMenuBar>
#include<QString>
#include<QPainter>
#include<QDebug>
#include"mypushbutton.h"
#include<QTimer>
#include<QLabel>
#include<QSound>
ChooseLevelScene::ChooseLevelScene(QWidget *parent) : QMainWindow(parent)
{
    //设置固定大小
    this->setFixedSize(400,600);
    //设置图标
    this->setWindowIcon(QIcon(":/res/Coin0001.png"));
    //设置标题

    this->setWindowTitle("选择关卡场景");
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
    //选择关卡按钮音效
    QSound *chooseSound = new QSound(":/res/TapButtonSound.wav",this);
    QSound *backSound = new QSound(":/res/BackButtonSound.wav",this);
    //返回按钮
    MyPushButton *backBtn = new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(),this->height()-backBtn->height());

    //点击返回
    connect(backBtn,&MyPushButton::clicked,[=](){
        backSound->play();
        //qDebug() << "点击了返回按钮";
        //告诉主场景 我返回了，主场景监听ChooseLevelScene的返回按钮
        //延时返回
        QTimer::singleShot(300,this,[=](){
            emit this->chooseSceneBack();
        });


    });

    //创建选择关卡的按钮
    for(int i=0;i<20;i++)
    {
        MyPushButton *menuBtn = new MyPushButton(":/res/LevelIcon.png");
        menuBtn->setParent(this);
        // 一个for循环 写一个矩阵
        menuBtn->move(55 + (i%4) *75,140 + (i/4) * 75);

        //监听每个按钮的点击事件
        connect(menuBtn,&MyPushButton::clicked,this,[=](){

            //播放选择关卡音效
            chooseSound->play();
            QString str = QString("您选择的是第 %1 关").arg(i+1);
            //qDebug() << str;
            // 进入到游戏场景
            this->hide(); //将选关场景隐藏掉
            play = new PlayScene(i+1);
            //设置游戏场景的初始位置
            play->setGeometry(this->geometry());
            play->show();

            connect(play,&PlayScene::SceneBack,this,[=](){
                this->setGeometry(play->geometry());
                this->show();
                //qDebug()<<"返回成功";
                delete play;
                play=NULL;
            });

        });

        //menuBtn->setText(QString::number(1+i));
        QLabel *label = new QLabel;
        label->setParent(this);
        label->setFixedSize(menuBtn->width(),menuBtn->height());
        label->setText(QString::number(i+1));

        label->move(55 + (i%4) *75,140 + (i/4) * 75);
        //设置 label上文字的对齐方式
        label->setAlignment(Qt::AlignHCenter | Qt ::AlignVCenter);
        //设置 让鼠标进行穿透
        label->setAttribute(Qt::WA_TransparentForMouseEvents);
    }
}
void ChooseLevelScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap  pix;
    pix.load(":/res/OtherSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //画背景上的图标
    pix.load(":/res/Title.png");
    //pix = pix.scaled(pix.width()*0.5,pix.height()*0.5);
    painter.drawPixmap((this->width()-pix.width())*0.5,30,pix);

}
