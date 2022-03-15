#include "mainscene.h"
#include "ui_mainscene.h"
#include<QPaintEvent>
#include<QPainter>
#include"mypushbutton.h"
#include<QDebug>
#include<QTimer>
#include<QTime>
#include<QSound>  //多媒体
MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);

    //配置主场景

    //设置固定大小
    this->setFixedSize(400,600);
    //设置图标
    this->setWindowIcon(QIcon(":/res/Coin0001.png"));
    //设置标题

    this->setWindowTitle("杰哥带你翻金币");
    //退出按钮的实现
    connect(ui->actionquit,&QAction::triggered,[=](){
       this->close();
    });

    //准备开始按钮的音效
    QSound * startSound = new QSound(":/res/TapButtonSound.wav");

    //开始按钮
    MyPushButton * startBtn = new MyPushButton(":/res/MenuSceneStartButton.png");
    startBtn->setParent(this);
    startBtn->move(this->width()*0.5 - startBtn->width()*0.5,this->height()*0.7);
    //实例化选择关卡场景
    this->chooseScene = new ChooseLevelScene;

    //监听选择关卡的返回按钮的信号
    connect(chooseScene,&ChooseLevelScene::chooseSceneBack,[=](){
        this->setGeometry(chooseScene->geometry());
        chooseScene->hide();
        this->show();//重新显示主场景
    });

    connect(startBtn,&MyPushButton::clicked,[=](){
        //qDebug() << "点击了开始 ";
        //播放开始音效资源
        startSound->play();

        //做弹起特效
        startBtn->zoom1();
        startBtn->zoom2();
        //延时进入
        QTimer::singleShot(400,this,[=](){
            //进入到选择关卡场景中
            //显示选择关卡场景
            chooseScene->show();

            //自身隐藏
            this->hide();
            chooseScene->setGeometry(this->geometry());


        });

    });
}
void MainScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap  pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //画背景上的图标
    pix.load(":/res/Title.png");
    pix = pix.scaled(pix.width()*0.5,pix.height()*0.5);
    painter.drawPixmap(10,30,pix);

}
MainScene::~MainScene()
{
    delete ui;
}

