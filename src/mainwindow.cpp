#include "mainwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QDebug>
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QFile>
#include <iostream>
#include <string>
#include <stdio.h>
#include <reportwidget.h>
#include <QFile>

#define SHADOW_WIDTH 10		// 阴影边框宽度;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
    //新增圆角
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    //
	connect(ui->ButtonPause,&QPushButton::clicked,this,&MainWindow::SlotButtonStart);
	connect(ui->ButtonNext,&QPushButton::clicked,this,&MainWindow::SlotButtonNext);
	connect(ui->ButtonLastMusic,&QPushButton::clicked,this,&MainWindow::SlotButtonPrevious);
	init();		//初始化相关函数
    //修改table外观
    initMusicTable();
    initWordTable();

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::init()
{
    setBackground();
    swapMode();
	PlayerList = new QMediaPlaylist;		//实例化播放器和播放列表
	Player = new QMediaPlayer;
    maxProgressValue=1000000;
    initDataBase();
    QFile file("lastMusicPath.txt");
    if (file.open(QIODevice::ReadWrite | QIODevice::Text)){
        QString line;
        QTextStream in(&file);
        while (!in.atEnd()) {
            line = in.readLine();
        }
        if(line.size()){
            defaultMusicPath=line;}
    }
    fileList = getFileNames(this->defaultMusicPath);    //获取文件夹下的所有音乐文件
	
	//	qDebug()<<"ALL Music File: \n "<<fileList;
    if(fileList.size()>0){
        for(int i=0;i<fileList.size();i++)   //输出所有音频文件到界面
            {
                QString fileName = fileList.at(i);
                QFileInfo fi(defaultMusicPath+"\\"+fileName);
                addItem(fi.completeBaseName());
                PlayerList->addMedia(QUrl::fromLocalFile(defaultMusicPath+"\\"+fileName));
            }
        PlayerList->setCurrentIndex(0);
        Player->setPlaylist(PlayerList);//将播放列表加载到播放器
    }else{
        changeToLikeSongs();
    }
    if(!PlayerList->isEmpty()){
        QString songName = ui->tableWidget->item(0,0)->text();
        ui->label->setText(songName);
    }
    qDebug()<<224;
    initVolumeSlider();
    initProgressSlider();
    connect(Player,&QMediaPlayer::stateChanged,this,&MainWindow::calculatePauseTime);
	connect(Player,&QMediaPlayer::currentMediaChanged,this,&MainWindow::SlotShowCurrentMedia);
//    connect(Player,&QMediaPlayer::stateChanged,this,&MainWindow::SlotShowCurrentMedia);
    connect(Player, SIGNAL(positionChanged(qint64)), this, SLOT(change_bar_position()));
    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(playTo(int,int)));
    connect(ui->tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(removeRow(int)));
    connect(ui->volumeSlider,SIGNAL(valueChanged(int)),this,SLOT(setVolume(int)));
    connect(ui->openFileButton,SIGNAL(clicked()),this,SLOT(openFiles()));

    connect(ui->progressSlider,SIGNAL(sliderMoved(int)),this,SLOT(progressMoved(int)));
    connect(ui->progressSlider,SIGNAL(sliderPressed()),this,SLOT(progressPressed()));
    connect(ui->progressSlider,SIGNAL(sliderReleased()),this,SLOT(progressReleased()));

    connect(ui->playMode,SIGNAL(clicked()),this,SLOT(setMode()));
//    connect(ui->newWindowWidget,SIGNAL(clicked()),this,SLOT(on_newWindowWidget_clicked()));
    connect(Player,SIGNAL(durationChanged(qint64)),this,SLOT(setLastSongDurationTime(qint64)));
    connect(Player,&QMediaPlayer::currentMediaChanged,this,&MainWindow::setLikeButton);
//    connect(ui->likeButton,SIGNAL(pressed()),this,SLOT(setLike()));
    connect(ui->likeButton,SIGNAL(pressed()),this,SLOT(pressLikeButton()));
    connect(ui->likeButton,SIGNAL(released()),this,SLOT(releaseLikeButton()));
//    connect(ui->searchButton,SIGNAL(clicked()),this,SLOT(clickSearchButton()));
    connect(ui->searchLine,SIGNAL(textChanged(const QString &)),this, SLOT(fuzzySearch(const QString &)));
//    connect(pCompleter,SIGNAL(activated(const QString &text)),this,SLOT(showWord(const QString &text)));
//    connect(ui->wordTable,SIGNAL(gainFocus()),this,SLOT(changeWT()));
    connect(ui->wordTable,SIGNAL(cellClicked(int,int)),this,SLOT(changeToWordDetail(int,int)));

    connect(ui->addToWorkBook,SIGNAL(pressed()),this,SLOT(pressWordButton()));
    connect(ui->addToWorkBook,SIGNAL(released()),this,SLOT(releaseWordButton()));
//    connect(listenTimer, &QTimer::timeout, this, SLOT(insertHistoryToDB));



    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    //The view supports both dragging and dropping
//    ui->tableWidget->setDragDropMode(QAbstractItemView::DragDrop);
//    ui->tableWidget->setDragEnabled(true);
//    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//选中一行
    setMode();//默认初始化列表循环
    this->setAcceptDrops(true);
    ui->tableWidget->setAcceptDrops(true);
//    ui->tableWidget->installEventFilter(this);
//    this->installEventFilter(this);
//    ui->tableWidget->setVisible(false);

//    clickSearchButton();
    setBackground();
}

QString MainWindow::cleanMusicName(QString musicName){
    QRegExp rx("(\\d+[\\.*\\_*\\-*\\s*]*)");//清除歌曲名前面的数字和.或-或空格等。
    QString str = musicName;
    QStringList list;
    int pos = 0;
    while ((pos = rx.indexIn(str, pos)) != -1) {
        list << rx.cap(1);
        pos += rx.matchedLength();
    }
    if(list.size()<=0){
        return str;
    }
    if(str.startsWith(list[0])){
        QString x=str.mid(list[0].size(),str.size());
        return x;
    }else{
        return str;
    }
}
QStringList MainWindow::getFileNames(const QString &path)
{	QDir dir(path);
	QStringList nameFilters;
    nameFilters << "*.wav"<<"*.flac"<<"*.mp3"<<"*.ape";
	QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
	return files;
}

void MainWindow::SlotShowCurrentMedia()
{   qDebug()<<"Do SlotShowCurrentMedia";
    int index = PlayerList->currentIndex();
//    qDebug()<<"PlayerList->currentIndex(): "<<PlayerList->currentIndex();
    if(index<0){
        return;//PlayerList->clear()清空歌单的时候会触发，index<-1,后续都会产生超边界，直接崩溃
    }
//    std::cout<<"player index"<<index<<std::endl;
//    qDebug()<<"ui->tableWidget->rowCount(): "<<ui->tableWidget->rowCount();
    ui->tableWidget->setCurrentCell(index,0);
    QString songName = ui->tableWidget->item(index,0)->text();
    ui->label->setText(songName);
//    qDebug() << PlayerList->currentMedia().request().url().toLocalFile();
//完全被connect的前后顺序整崩了。。。
    QString musicPath = PlayerList->currentMedia().request().url().toLocalFile();
    if (musicPath!=lastSongPath){
        if(lastIsPlay==false){
            //因为按下上下键的时候，先触发的currentMediaChange，才触发statusChange，所以计算pauseTime在这里补充。
            float pauseDeltaTime = (double)clock()-pauseTimeStart;
            pauseTime+=pauseDeltaTime;
            lastIsPlay=true;
        }
//        listenTimer->start(30000);
        //todo 暂停计时不对处理
        if(lastSongPath==""){
            lastSongPath=musicPath;
            lastSongName = songName;
            lastTimeStartForHistory = (double)clock();
//            lastSongTotalTime=static_cast<float>(Player->duration());
//            qDebug()<<"lastSongTotalTime0: "<<lastSongTotalTime;
            lastDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            return;
        }

        listenTime=(double)clock()-lastTimeStartForHistory-pauseTime;
//        if(listenTime<0){
//            listenTime+=pauseTime;
//        }
        qDebug()<<"lastSongTotalTime: "<<lastSongTotalTime;
        qDebug()<<"listenTime: "<<listenTime;
        if(listenTime>23000){//歌曲时间太短会不正常,听23秒以上才算
            if(lastSongTotalTime>23000){

                listenCount=(int)((listenTime-23000)/lastSongTotalTime)+1;
                insertHistoryToDB();
            }else if(lastSongTotalTime>0){
                listenCount=listenTime/lastSongTotalTime;
                insertHistoryToDB();
            }

        }else{
            if(lastSongTotalTime<23000&&lastSongTotalTime>0){
                listenCount=listenTime/lastSongTotalTime;
                insertHistoryToDB();
            }
        }
       lastSongPath=musicPath;
       lastSongName = songName;
       lastTimeStartForHistory = (double)clock();
//       lastSongTotalTime=static_cast<float>(Player->duration());
//       qDebug()<<songName<<"Player->duration(): "<<lastSongTotalTime;
       lastDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
       listenTime=0;
       listenCount=0;
       pauseTime=0;
    }
}

void MainWindow::setLastSongDurationTime(qint64){
    int index = PlayerList->currentIndex();
//    qDebug()<<"PlayerList->currentIndex(): "<<PlayerList->currentIndex();
    if(index<0){
        return;//PlayerList->clear()清空歌单的时候会触发，index<-1,后续都会产生超边界，直接崩溃
    }
//    qDebug()<<"Do setLastSongDuarationTime:  "<<(float)(Player->duration());
    lastSongTotalTime = (float)(Player->duration());
}
void MainWindow::insertHistoryToDB(){

    //插入听歌历史记录todo
    db_MusicHistory.setDatabaseName("musicHistory.db");//连接本地数据库
    if(!db_MusicHistory.open())
    {
        return;
    }
//    QString dataTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
//    QString musicPath = PlayerList->currentMedia().request().url().toLocalFile();
//    QString songName = ui->tableWidget->item(PlayerList->currentIndex(),0)->text();
    QSqlQuery query(db_MusicHistory);
    int listenTimeToSecond = listenTime/1000;
    bool a = query.exec(QString("insert into MusicHistory (DateTime,MusicName,FileName,ListenTime,ListenCount) " "VALUES ('%1','%2','%3','%4','%5')").arg(lastDateTime).arg(lastSongName).arg(lastSongPath).arg(listenTimeToSecond).arg(listenCount));
    if(a){
        qDebug() << "Do addSongHistory";
    }else{
    qDebug()<<"query error :"<<query.lastError();}
}

void MainWindow::calculatePauseTime(){
    qDebug()<<"Current status change"<<Player->PlayingState;
    if(isPlay==true&&lastIsPlay==false){
        float pauseDeltaTime = (double)clock()-pauseTimeStart;
        pauseTime+=pauseDeltaTime;
        lastIsPlay=true;
    }else if(isPlay==false){
        pauseTimeStart=(double)clock();
        lastIsPlay=false;
    }
}
void MainWindow::SlotButtonStart()
{
	if(!isPlay)
		{
			isPlay = true;
            ui->ButtonPause->setIcon(QIcon(":off.ico"));
			Player->play();
		}
	else
		{
			isPlay = false;
            ui->ButtonPause->setIcon(QIcon(":on.ico"));
			Player->pause();
		}
}

void MainWindow::SlotButtonPrevious()
{
    if(PlayerList->currentIndex()>0){
	PlayerList->previous();
    PlayerList->previousIndex();
    Player->play();
    ui->ButtonPause->setIcon(QIcon(":off.ico"));
    isPlay=true;}
    else{
        PlayerList->setCurrentIndex(PlayerList->mediaCount()-1);
        Player->play();
        ui->ButtonPause->setIcon(QIcon(":off.ico"));
        isPlay=true;
    }
}

void MainWindow::SlotButtonNext()
{
    if(PlayerList->currentIndex()<(PlayerList->mediaCount()-1)){
	PlayerList->next();
    PlayerList->nextIndex();
    Player->play();
    ui->ButtonPause->setIcon(QIcon(":off.ico"));
    isPlay=true;
    }
    else{
        PlayerList->setCurrentIndex(0);
        Player->play();
        ui->ButtonPause->setIcon(QIcon(":off.ico"));
        isPlay=true;
    }
}

void MainWindow::addItem(QString name)
{
	int count = ui->tableWidget->rowCount();
	ui->tableWidget->setRowCount(count+1);
    QString nameCleaned = cleanMusicName(name);
    QTableWidgetItem *itemName = new QTableWidgetItem(nameCleaned);
	ui->tableWidget->setItem(count, 0 , itemName);
	
}

void MainWindow::playTo(int i,int)
{
    PlayerList->setCurrentIndex(i);
    Player->play();
    ui->ButtonPause->setIcon(QIcon(":off.ico"));
    isPlay=true;
}
//新增阴影
void MainWindow::setBackground()
{
//    QPainter painter(this);
//    painter.setRenderHint(QPainter::Antialiasing,true);  // 反锯齿;
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setOffset(0,0);
    shadow->setColor(QColor("#444444"));
    shadow->setBlurRadius(10);
    ui->bgFrame->setGraphicsEffect(shadow);
}
//设置table
void MainWindow::initMusicTable()
{
//    ui->tableWidget->setWindowFlags(Qt::FramelessWindowHint);
    ui->tableWidget->setFrameShape(QFrame::NoFrame);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setVisible(false);
    ui->tableWidget->setShowGrid(false);

    //添加背景色
    ui->tableWidget->setStyleSheet("QTableWidget::item:selected {background-color: rgb(40,209,168)}");
    //滚动条
    ui->tableWidget->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
//    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
//    QFile file(":/tableWidget.qss");
//    file.open(QFile::ReadOnly);
//    QTextStream filetext(&file);
//    QString stylesheet = filetext.readAll();
//    ui->tableWidget->verticalScrollBar()->setStyleSheet(stylesheet);
//    file.close();
    //圆角 tableWidget(无效todo）
//    QPainter painter2(ui->tableWidget);
//    painter2.setBrush(QBrush(QColor(48,51,55)));
//    painter2.setPen(Qt::transparent);
//    QRect rect2 = ui->tableWidget->rect();
//    painter2.drawRoundedRect(rect2, 25, 25);
    //奇偶行分颜色
//    QPalette pal;
//    pal.setColor(QPalette::Base, QColor(240, 255, 240));
//    pal.setColor(QPalette::AlternateBase, QColor(244, 238, 224));
//    ui->tableWidget->setPalette(pal);
//    ui->tableWidget->setAlternatingRowColors(true);
    //设置背景色
//    QPalette pal;
//    pal.setColor(QPalette::Base, QColor(145, 208, 214));
//    pal.setColor(QPalette::Base, QColor(255, 255, 255));
//    ui->tableWidget->setPalette(pal);

}
//拖拽todo
void MainWindow::mousePressEvent(QMouseEvent *event)
{
//    qDebug()<<event->source();
    if(event->button() == Qt::LeftButton){
    px=event->x();
    py=event->y();//记录鼠标相对窗口坐标
    pxx=geometry().width();//窗口宽度
    pyy=geometry().height();//窗口高度
    hx=geometry().x();
    hy=geometry().y();//记录鼠标按下时窗口属性，窗口左上点坐标（hx，hy）
    kx=cursor().pos().x();
    ky=cursor().pos().y();//记录鼠标全局坐标（大概有别的记录鼠标的方法但是我不会QwQ
    update();}
    else if(event->button() == Qt::RightButton){
        float cursorRightClickX=event->x();
        float cursorRightClickY=event->y();
        if((cursorRightClickX<70||cursorRightClickX>449)&&(cursorRightClickY<99||cursorRightClickY>300)){
            qDebug()<<"Hahahahah";
            swapMode();}
    }

}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
//    qDebug()<<event->source();
    if(px>0&&px<pxx-10&&py>0&&py<pyy-10)//中间区域（拖拽
         {this->setGeometry(hx+cursor().pos().x()-kx,hy+cursor().pos().y()-ky,pxx,pyy);}
    update();
}
//删除歌曲todo
void MainWindow::removeRow(int r)
{
    ui->tableWidget->removeRow(r);
    fileList.removeAt(r);
    PlayerList->removeMedia(r);
    Player->setPlaylist(PlayerList);
}
void MainWindow::change_bar_position()
{
    quint64  current_position = Player->position();
    float pSliderValue=(static_cast<float>(current_position)/static_cast<float>(Player->duration()))*maxProgressValue;
//    std::cout<<pSliderValue<<std::endl;
    ui->progressSlider->setValue(pSliderValue);
//            qDebug() << "setmaximum";
            max_time.setHMS(0, Player->duration()/60000,qRound((Player->duration()%60000)/1000.0));
            ui->duration_label->setText(max_time.toString("mm:ss"));
//根据当前播放位置，设置当前时间
//    ui->progressSlider->setValue((current_position/Player->duration())*maxProgressValue);
    pos_time.setHMS(0, current_position/60000,qRound((current_position%60000)/1000.0));
    ui->pos_time_label->setText(pos_time.toString("mm:ss"));
}
void MainWindow::initVolumeSlider(){
    int nMin = 0;
    int nMax = 100;
    int nSingleStep = 10;/*
    QWidget* backgroundWidget = new QWidget(ui->volumeSlider);
    backgroundWidget->resize(262, 36);
    backgroundWidget->setStyleSheet("QWidget{background-color:rgba(0, 0, 0,80);border-radius:18px;}");*/
    ui->volumeSlider->setOrientation(Qt::Horizontal);  // 水平方向
    ui->volumeSlider->setMinimum(nMin);  // 最小值
    ui->volumeSlider->setMaximum(nMax);  // 最大值
    ui->volumeSlider->setSingleStep(nSingleStep);  // 步长
    QFile file(":/volumeSlider.qss");
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    ui->volumeSlider->setStyleSheet(stylesheet);
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setOffset(4,4);
    effect->setColor(QColor(0,0,0,50));
    effect->setBlurRadius(10);
    ui->volumeSlider->setGraphicsEffect(effect);
    setVolume(20);//默认20%音量
}
void MainWindow::setVolume(int vol)
{
    Player->setVolume(vol);
}

void MainWindow::initProgressSlider(){
    ui->progressSlider->setOrientation(Qt::Horizontal);  // 水平方向
    QFile file(":/progressSlider.qss");
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    ui->progressSlider->setStyleSheet(stylesheet);
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setOffset(4,4);
    effect->setColor(QColor(0,0,0,50));
    effect->setBlurRadius(10);
    ui->progressSlider->setGraphicsEffect(effect);
    ui->progressSlider->setRange(0,maxProgressValue);
}

void MainWindow::progressMoved(int pValue)
{
    currentProgressValue=pValue;
    Player->setPosition((static_cast<float>(currentProgressValue)/maxProgressValue*static_cast<float>(Player->duration())));
//    std::cout<<"cpValue: "<<currentProgressValue<<std::endl;

}

void MainWindow::progressPressed()
{
    Player->pause();
}

void MainWindow::progressReleased()
{

    Player->play();
}
//todo
void MainWindow::openFiles()
{
    QStringList nameFilters;
    QFileDialog *qfd=new QFileDialog;
    QStringList fileRead = qfd->getOpenFileNames(this, tr("Open Files"),defaultMusicPath,"*.wav *.flac *.mp3 *.ape");
    if(fileRead.size()>0){
        defaultMusicPath = (QFileInfo(fileRead.at(0))).absolutePath();
        QFile file("lastMusicPath.txt");
        if (file.open(QIODevice:: WriteOnly| QIODevice::Truncate)){
            QTextStream out(&file);
            out<<defaultMusicPath;
        }
    }

    if(fileRead.size()>0){
        fileList=fileRead;
        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(0);
        PlayerList->clear();
        for(int i=0;i<fileList.size();i++)   //输出所有音频文件到界面
            {
                QString fileName = fileList.at(i);
                QFileInfo fileInfo(fileName);
                addItem(fileInfo.completeBaseName());

                PlayerList->addMedia(QUrl::fromLocalFile(fileName));
            }
        Player->setPlaylist(PlayerList);        
        Player->pause();
        isPlay=false;
        PlayerList->setCurrentIndex(0);
        ui->ButtonPause->setIcon(QIcon(":on.ico"));
    }
}

void MainWindow::setMode(){
    if(modeFlag==0){
        modeFlag=1;
        ui->playMode->setIcon(QIcon(":single.ico"));
        PlayerList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    }else if(modeFlag==1){
        modeFlag=2;
        ui->playMode->setIcon(QIcon(":random.ico"));
        PlayerList->setPlaybackMode(QMediaPlaylist::Random);
    }else if(modeFlag==2){
        modeFlag=0;
        ui->playMode->setIcon(QIcon(":loop.ico"));
        PlayerList->setPlaybackMode(QMediaPlaylist::Loop);
    }else{
        modeFlag=0;
        ui->playMode->setIcon(QIcon(":loop.ico"));
        PlayerList->setPlaybackMode(QMediaPlaylist::Loop);
    }
}

void MainWindow::setLike(){
    if(ui->tableWidget->rowCount()<1){
        return;
    }
    if(likeFlag==0){
        likeFlag=1;
        ui->likeButton->setIcon(QIcon(":like.ico"));
        addLikeSong();
    }else{
        likeFlag=0;
        ui->likeButton->setIcon(QIcon(":notlike.ico"));
        delLikeSong();
    }
}


void MainWindow::addLikeSong(){
//    db_music=QSqlDatabase::addDatabase("QSQLITE");//建库
    db_music.setDatabaseName("like.db");//连接本地数据库
    if(!db_music.open())
    {
        return;
    }
    QSqlQuery query(db_music);
    bool a = query.exec(QString("insert into LocalMusic (MusicName,FileName) " "VALUES ('%1','%2')").arg(ui->tableWidget->item(PlayerList->currentIndex(),0)->text())\
       .arg(PlayerList->currentMedia().request().url().path()));
    if(a){
        qDebug() << "Do addLikeSong";
    }else{
    qDebug()<<"query error :"<<query.lastError();}
}

void MainWindow::delLikeSong(){
//    db_music=QSqlDatabase::addDatabase("QSQLITE");//建库
    db_music.setDatabaseName("like.db");//连接本地数据库
    if(!db_music.open())
    {
        return;
    }
    QSqlQuery query(db_music);
    QString QSDelete("DELETE FROM LocalMusic WHERE FileName == :FileName");
    query.prepare(QSDelete);
    query.bindValue(":FileName", PlayerList->currentMedia().request().url().path());
    bool b = query.exec();
//    if(b){
//        qDebug() << "Do delLikeSong";
//    }else{
//        qDebug()<<"query error :"<<query.lastError();}

}

void MainWindow::setLikeButton(){
//    db_music=QSqlDatabase::addDatabase("QSQLITE");//建库
//    db_music.setDatabaseName("like.db");//连接本地数据库
    int index = PlayerList->currentIndex();
//    qDebug()<<"PlayerList->currentIndex(): "<<PlayerList->currentIndex();
    if(index<0){
        return;//PlayerList->clear()清空歌单的时候会触发,无需查询
    }
    db_music=QSqlDatabase::database("likeMusic");
    if(!db_music.open())
    {
        qDebug()<<"connect to like.db fail";
        return;
    }
    QSqlQuery query(db_music);
    QString QSSelect("SELECT * FROM LocalMusic WHERE FileName == :FileName");
    query.prepare(QSSelect);
    query.bindValue(":FileName", PlayerList->currentMedia().request().url().path());
    bool b = query.exec();
//    if(b){
//        qDebug() << "Do search";
//    }
    while (query.next())
    {
         qDebug() << "setLikeButton:"<<query.value(0).toString() << query.value(1).toString();
         likeFlag=1;
         ui->likeButton->setIcon(QIcon(":like.ico"));
         return;
    }
    likeFlag=0;
    ui->likeButton->setIcon(QIcon(":notlike.ico"));
}

void MainWindow::changeToLikeSongs(){
    qDebug()<<"changeToLikeSongs.";
    db_change_music=QSqlDatabase::database("changeLikeMusic");
    QSqlQuery query(db_change_music);
    QString QSSelect("SELECT * FROM LocalMusic");
    query.prepare(QSSelect);
    if(!query.exec()){
        qDebug()<<"like.db: Changetolikesong Fail!";
        return;}
    if(!PlayerList->isEmpty()){
        PlayerList->clear();
        qDebug()<<"PlayerList->clear()";
    }

    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    int likeMusicCount = 0;
    while (query.next()){   
        qDebug()<<343;
//        qDebug()<<query.value(1).toString()<<"231"<<query.value(2).toString();
        likeMusicCount++;
        addItem(query.value(1).toString());
        PlayerList->addMedia(QUrl::fromLocalFile(query.value(2).toString()));
    }
    if(likeMusicCount<1){
        return;
    }
    qDebug()<<3433;
    PlayerList->setCurrentIndex(0);
    qDebug()<<"PlayerList->setCurrentIndex(0)";
    Player->setPlaylist(PlayerList); 
    qDebug()<<"setPlaylist(PlayerList)";
    Player->pause();
    isPlay=false;
    ui->ButtonPause->setIcon(QIcon(":on.ico"));
//    setLikeButton();
    likeFlag=1;
    ui->likeButton->setIcon(QIcon(":like.ico"));
}

void MainWindow::pressLikeButton(){
    qDebug()<<"pressLikeButton";
    time_Start=(double)clock();

}
void MainWindow::releaseLikeButton(){
    qDebug()<<"releaseLikeButton";
   double totalPressTime = (double)clock()-time_Start;
   if(totalPressTime<=800){
       setLike();
   }else if(totalPressTime<=3000){
       changeToLikeSongs();
   }else{
//       this->close();
   }
}
void MainWindow::dragEnterEvent(QDragEnterEvent *event){
//    qDebug()<<"drag 1"<<event->source();
    if(event->mimeData()->hasUrls())
    {
            event->setDropAction(Qt::MoveAction);
            event->accept();
            qDebug()<<"drag 2"<<event->type()<<"  "<<event->source();
            event->acceptProposedAction();
    }
    else
    {
        qDebug()<<"drag 3";
        event->ignore();
    }

}
void MainWindow::dropEvent(QDropEvent *event){
    qDebug()<<"Drop";
    event->acceptProposedAction();
    const QMimeData *mimeData = event->mimeData();
    if(!mimeData->hasUrls())
    {
        qDebug()<<"Drop:Don't have data";
        return;
    }

    QList<QUrl> urlList = mimeData->urls();

    for(int i=0;i<urlList.size();i++){
        QString filePath = urlList.at(i).toLocalFile();
        qDebug()<<urlList.at(i).path();
        //打开拖入的文件
        qDebug()<<"Drop:have data:"<<mimeData->hasHtml();
        QFileInfo fileInfo(filePath);
        qDebug()<<fileInfo.completeBaseName();
        QString fileName=fileInfo.fileName();
        if(fileName.right(4).contains(".mp3")|fileName.right(4).contains(".wav")|fileName.right(4).contains(".ape")|fileName.right(5).contains(".flac"))
            {   qDebug()<<"Drop:add item";
                addItem(fileInfo.completeBaseName());
                PlayerList->addMedia(urlList.at(i));}
    }

}
//bool MainWindow::eventFilter(QObject *obj, QEvent *e){

//    if (e->type()==QEvent::Drop){
//        qDebug()<<"Event 2";
//        if(obj == ui->tableWidget){
//            qDebug()<<"Event 3:"<<clock()<<obj;
//            return true;
//        }else {
//            qDebug()<<"Event 4"<<clock()<<obj;
//            return false;
//        }
//    }else return QWidget::eventFilter(obj,e);
//}
void MainWindow::swapMode(){
    if(swapFlag==0){
        swapFlag=1;
    ui->frame_2->setVisible(true);
    ui->frame_3->setVisible(false);
//    db_music=QSqlDatabase::addDatabase("QSQLITE");//建库
    db_music.setDatabaseName("like.db");//连接本地数据库
    if(!db_music.open())
    {
        return;
    }
    }
    else {
        swapFlag=0;
        ui->frame_2->setVisible(false);
        ui->frame_3->setVisible(true);
//        db_stardic=QSqlDatabase::addDatabase("QSQLITE");//建库
//        db_stardic.setDatabaseName("stardict.db");//；连接数据库文件
//        if(!db_stardic.open()){
//            return ;}
    }

}

void MainWindow::clickSearchButton(){
//    if(searchButtonFlag==0){
//        ui->searchLine->setVisible(false);
//        searchButtonFlag=1;
//    }else{
//        ui->searchLine->setVisible(true);
//        searchButtonFlag=0;
//    }
}

void MainWindow::fuzzySearch(const QString &){
    wordTableLoadFlag=0;
//    db_stardic=QSqlDatabase::addDatabase("QSQLITE");//建库
    db_stardic=QSqlDatabase::database("db_stardic");
    db_stardic.setDatabaseName("stardict.db");//；连接数据库文件
    if (!db_stardic.open()){
        return;
    }
    QStringList tables = db_stardic.tables();  //获取数据库中的表
    qDebug() << QString("表的个数： %1").arg(tables.count()); //打印表的个数
    QStringListIterator itr(tables);
    while (itr.hasNext())
    {
        QString tableNmae = itr.next().toLocal8Bit();
        qDebug() << QString("表名：")+ tableNmae;}
    qDebug()<<ui->wordTable->isHidden();
    if(ui->wordTable->isHidden()){
        ui->wordTable->setVisible(true);
        ui->textEdit->setVisible(false);
        ui->buttonNextWord->setVisible(false);
        ui->buttonLastWord->setVisible(false);
    }
    queryData.clear();
    queryData.resize(15);
    QStringList keyWords;
    fuzzyWordList.clear();
    keyWords<<"id"<<"word"<<"sw"<<"phonetic"<<"definition"<<"translation"<<"pos"<<"collins"<<"oxford"<<"tag"<<"bnc"<<"frq"<<"exchange";

    QSqlQuery query(db_stardic);
    QString queryWord=ui->searchLine->text();
    qDebug()<<"current text:"<<queryWord;
    QString queryFuzzyWord="'"+queryWord+"%'";
    QString QSSelect=QString("SELECT * FROM stardict WHERE word LIKE %1").arg(queryFuzzyWord);
    qDebug()<<QSSelect;
    qDebug()<<"fuzzyWord: "<<queryFuzzyWord;
//    QString QSSelect2("SELECT * FROM stardict WHERE word GLOB '*average*'");
//    QString QSSelect3=QString("SELECT * FROM stardict WHERE word LIKE '%average%'");
    query.prepare(QSSelect);
//    query.bindValue(":word",queryFuzzyWord);
    bool b=query.exec();
    if(!b){
        qDebug()<<"query fail";
        return;
    }
    int countFuzzyWords=0;
    while(query.next()){
        qDebug()<<"query data get!";
        if(countFuzzyWords>=15){
            break;
        }
        for(int j=0;j<keyWords.size();j++){
//                qDebug()<<"j: "<<j;
            qDebug()<<queryData.size();
            queryData[countFuzzyWords][keyWords[j]]=query.value(j).toString();
//                qDebug()<<j;
//                qDebug()<<keyWords[j];
//                qDebug()<<query.value(j).toString();
            if (j<3){
//                qDebug()<<queryData[countFuzzyWords][keyWords[j]];
            }
        }
        countFuzzyWords+=1;

    }
    for(int i=0;i<13;i++){
        if(i>=countFuzzyWords){
            break;
        }

        fuzzyWordList<<queryData[i]["word"]+" "+queryData[i]["translation"];
    }
//    qDebug()<<queryData[0];
    qDebug()<<fuzzyWordList;
//    pCompleter->setModel(new QStringListModel(fuzzyWordList));
//    pCompleter->setCaseSensitivity(Qt::CaseInsensitive);
//    ui->searchLine->setCompleter(pCompleter);
    if(fuzzyWordList.size()>0){
        resetWordTable(fuzzyWordList);}
}

void MainWindow::showWord(const QString &text){
//    ui->wordTable->setVisible(false);
//    ui->textEdit->setVisible(true);
    ui->textEdit->clear();
    ui->textEdit->append(text);
}


void MainWindow::initWordTable(){
    qDebug()<<"initWordTable";
    ui->wordTable->setVisible(false);
    ui->textEdit->setVisible(true);
    ui->buttonNextWord->setVisible(true);
    ui->buttonLastWord->setVisible(true);
    ui->wordTable->setWindowFlags(Qt::FramelessWindowHint);
    ui->wordTable->setFrameShape(QFrame::NoFrame);
    ui->wordTable->horizontalHeader()->setVisible(false);
    ui->wordTable->setShowGrid(false);

    //添加背景色
//    ui->wordTable->setStyleSheet("QTableWidget::item:selected {background-color: rgb(0,159,168)}");
    ui->wordTable->setStyleSheet("QTableWidget::item:selected {background-color: rgb(100,170,12)}");
    //滚动条
    ui->wordTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

//    ui->wordTable->verticalScrollBar()->setEnabled(false);

    ui->wordTable->setFocusPolicy(Qt::NoFocus);
    //initLineEdit
}
void MainWindow::addWordItem(QString wordShortcut){
    qDebug()<<"addWordItem: "<<wordShortcut;
    int count = ui->wordTable->rowCount();
    ui->wordTable->setRowCount(count+1);
    QTableWidgetItem *word = new QTableWidgetItem(wordShortcut);
    ui->wordTable->setItem(count, 0 , word);
}

void MainWindow::resetWordTable(QStringList wordList){
    qDebug()<<"resetWordTable";
    ui->wordTable->clear();
    ui->wordTable->setRowCount(0);
    for(int i=0;i<wordList.size();i++){
        addWordItem(fuzzyWordList[i]);
    }
}

void MainWindow::chooseWord(int i){
    qDebug()<<"chooseWord";
    if(i<fuzzyWordList.size())
    showWord(fuzzyWordList[i]);
}
void MainWindow::changeWT(){
//    ui->wordTable->setVisible(true);
//    ui->textEdit->setVisible(false);
}

void MainWindow::changeToWordDetail(int i,int){
    currentWordNumber=i;
    ui->buttonNextWord->setVisible(true);
    ui->buttonLastWord->setVisible(true);
    ui->wordTable->setVisible(false);
    ui->textEdit->setVisible(true);
//    ui->textEdit->isHidden()
//    wordTableVisibleStatus=false;
//    textEditVisibleStatus=true;
    ui->textEdit->clear();
    QString phonetic,definition;
    if(wordTableLoadFlag==0){
        if(queryData[i]["phonetic"].size()>0){
            phonetic =" /"+queryData[i]["phonetic"]+"/";}
        if(queryData[i]["definition"].size()>0){
            definition ="\n\n英英:\n"+queryData[i]["definition"];}
        QString text="\n   "+queryData[i]["word"]+phonetic+"\n\n"+"简明:\n"+queryData[i]["translation"]+definition;
        currentWordDetail="\n   "+queryData[i]["word"]+"\n\n"+"简明:\n"+queryData[i]["translation"]+definition;
        currentWord=queryData[i]["word"];
        currentPhonetic=queryData[i]["phonetic"];
        currentDefinition=queryData[i]["definition"];
        currentTranslation=queryData[i]["translation"];
        ui->textEdit->append(text);
        ui->textEdit->moveCursor(QTextCursor::Start);}
    else{
        if(wordBookQuery[i]["phonetic"].size()>0){
            phonetic =" /"+wordBookQuery[i]["phonetic"]+"/";}
        if(wordBookQuery[i]["definition"].size()>0){
            definition ="\n\n英英:\n"+wordBookQuery[i]["definition"];}
        QString text="\n   "+wordBookQuery[i]["word"]+phonetic+"\n\n"+"简明:\n"+wordBookQuery[i]["translation"]+definition;
        currentWordDetail="\n   "+wordBookQuery[i]["word"]+"\n\n"+"简明:\n"+wordBookQuery[i]["translation"]+definition;
        currentWord=wordBookQuery[i]["word"];
        currentPhonetic=wordBookQuery[i]["phonetic"];
        currentDefinition=wordBookQuery[i]["definition"];
        currentTranslation=wordBookQuery[i]["translation"];
        ui->textEdit->append(text);
        ui->textEdit->moveCursor(QTextCursor::Start);
    }
    setWordButtonStatus();
}

void MainWindow::clickLWordButton(){
    if((ui->textEdit->toPlainText()).size()<1){
        return;
    }
    if(wordButtonFlag==0){
        wordButtonFlag=1;
        ui->addToWorkBook->setIconSize(QSize(30,30));
        ui->addToWorkBook->setIcon(QIcon(":018cidian.ico"));//这是收藏状态的图标，由于图标大小不一致，得改一下尺寸

        addToWordBook();
    }else{
        wordButtonFlag=0;
        ui->addToWorkBook->setIconSize(QSize(35,35));
        ui->addToWorkBook->setIcon(QIcon(":018dict.ico"));//这是没有收藏状态的图标
        delFromWordBook();
    }
}

void MainWindow::setWordButtonStatus(){
    db_wordBook=QSqlDatabase::database("wordBook");
    if(!db_wordBook.open())
    {
        qDebug()<<"connect to wordBook.db fail";
        return;
    }
    QSqlQuery query(db_wordBook);
    QString QSSelect("SELECT * FROM wordBook WHERE word == :wordSearched");
    query.prepare(QSSelect);
    query.bindValue(":wordSearched", currentWord);
    bool b = query.exec();
    if(b){
        qDebug() << "Do search wordBook";
    }else{
        return;
    }
    while (query.next())
    {
         qDebug() << "setWordButtonStatus:"<<query.value(0).toString() << query.value(1).toString();
         wordButtonFlag=1;
         ui->addToWorkBook->setIconSize(QSize(30,30));
         ui->addToWorkBook->setIcon(QIcon(":018cidian.ico"));
         return;
    }
    wordButtonFlag=0;
    ui->addToWorkBook->setIconSize(QSize(35,35));
    ui->addToWorkBook->setIcon(QIcon(":018dict.ico"));
}

void MainWindow::addToWordBook(){
//        db_wordBook=QSqlDatabase::database("qt_sql_default_connection");//建库
        db_wordBook.setDatabaseName("wordBook.db");//连接本地数据库
        if(!db_wordBook.open())
        {
            return;
        }
//        qDebug()<<ui->textEdit->toPlainText();
//        qDebug()<<currentWordDetail;
        QSqlQuery query(db_wordBook);
        bool a = query.exec(QString("insert into wordBook (word,phonetic,definition,translation) " "VALUES ('%1','%2','%3','%4')").arg(currentWord).arg(currentPhonetic).arg(currentDefinition).arg(currentTranslation));
        if(a){
            qDebug() << "Do addToWordBook";
        }else{
        qDebug()<<"query error :"<<query.lastError();}
}
void MainWindow::delFromWordBook(){
        if(!db_wordBook.open())
        {
            return;
        }
        QSqlQuery query(db_wordBook);
        QString QSDelete("DELETE FROM wordBook WHERE word == :wordSearched");
        query.prepare(QSDelete);
        query.bindValue(":wordSearched", currentWord);
        bool b = query.exec();
        if(b){
            qDebug() << "Do del word";
        }else{
            qDebug()<<"query error :"<<query.lastError();}

}

void MainWindow::pressWordButton(){
    time_Start=(double)clock();
}
void MainWindow::releaseWordButton(){
   double totalPressTime = (double)clock()-time_Start;
   if(totalPressTime<=800){
       clickLWordButton();
   }else if(totalPressTime<=3000){
       changeToWordBook();
   }else{
//       this->close();
   }
}

void MainWindow::initDataBase(){
    //workBook DB
    db_wordBook=QSqlDatabase::addDatabase("QSQLITE","wordBook");
    db_wordBook.setDatabaseName("wordBook.db");//连接本地数据库
    if(!db_wordBook.open())
    {
        qDebug()<<"init wordBook DB fail.";
    }
    QSqlQuery query(db_wordBook);
    bool a = query.exec(QString("create table wordBook (id integer primary key,word varchar, phonetic varchar,definition varchar,translation varchar);"));//本地单词本数据，把单词内容存起来
    if(!a){
        qDebug()<<"query error :"<<query.size();
    }
    else{
        qDebug()<<"Create likeDB success!";
    }

    //like DB
//    db_music=QSqlDatabase::addDatabase("QSQLITE");
    db_music = QSqlDatabase::addDatabase("QSQLITE","likeMusic");
    db_music.setDatabaseName("like.db");//给数据库起名字
    if(!db_music.open())
    {
        qDebug()<<"initLikeDB fail.";
    }
    QSqlQuery query2(db_music);
    a = query2.exec(QString("create table LocalMusic (id integer primary key,MusicName varchar,FileName varchar);"));//本地音乐列表数据，把歌曲名和歌曲路径给存起来
    if(!a){
        qDebug()<<"query error :"<<query2.size();
    }
    else{
        qDebug()<<"Create likeDB success!";
    }

    //stardic DB
//    db_stardic=QSqlDatabase::addDatabase("QSQLITE");
    db_stardic = QSqlDatabase::addDatabase("QSQLITE","db_stardic");
    db_stardic.setDatabaseName("stardict.db");
    //like DB for changeLikeList
    db_change_music = QSqlDatabase::addDatabase("QSQLITE","changeLikeMusic");
    db_change_music.setDatabaseName("like.db");//连接本地数据库
    //music history
    db_MusicHistory = QSqlDatabase::addDatabase("QSQLITE","db_musicHistory");
    db_MusicHistory.setDatabaseName("musicHistory.db");
    if(!db_MusicHistory.open())
    {
        qDebug()<<"history db fail.";
    }
    QSqlQuery query3(db_MusicHistory);
    a = query3.exec(QString("create table MusicHistory (id integer primary key,DateTime varchar,MusicName varchar,FileName varchar,ListenTime integer,ListenCount integer);"));//本地听音乐历史数据
    if(!a){
        qDebug()<<"query error :"<<query3.size();
    }
    else{
        qDebug()<<"Create musicHistory success!";
    }
}


void MainWindow::changeToWordBook(){
    qDebug()<<"changeToWordBook.";
//    db_wordBook=QSqlDatabase::database("wordBook");
    QSqlQuery query(db_wordBook);
    QString QSSelect("SELECT * FROM wordBook");
    query.prepare(QSSelect);
    if(!query.exec()){
        qDebug()<<"wordBook.db: ChangetowordBook Fail!";
        return;}

    QSqlQueryModel *queryModel = new QSqlQueryModel();
    queryModel->setQuery(query);
    wordBookQuery.clear();
    int nRecordCount = queryModel->rowCount();
    wordBookQuery.resize(nRecordCount);
    qDebug()<<"query cont: "<<nRecordCount;

    ui->wordTable->clear();
    ui->wordTable->setRowCount(0);
//    QSSelect=("SELECT * FROM wordBook");
//    query.prepare(QSSelect);
//    if(!query.exec()){
//        qDebug()<<"wordBook.db: ChangetowordBook Fail!";
//        return;}
    int i = 0;
    query.seek(-1);
    while (query.next()){
            qDebug()<<i;
        qDebug()<<query.value(1).toString()<<" from wordBook "<<query.value(2).toString();
        wordBookQuery[i]["word"]=query.value(1).toString();
        wordBookQuery[i]["phonetic"]=query.value(2).toString();
        wordBookQuery[i]["definition"]=query.value(3).toString();
        wordBookQuery[i]["translation"]=query.value(4).toString();
        addWordItem(query.value(1).toString()+" "+query.value(4).toString());
        wordButtonFlag=1;
        i++;
    }
    if (wordButtonFlag==1){
        ui->addToWorkBook->setIconSize(QSize(30,30));
        ui->addToWorkBook->setIcon(QIcon(":018cidian.ico"));//这是收藏状态的图标
        ui->wordTable->setVisible(true);
        ui->textEdit->setVisible(false);
        wordTableLoadFlag=1;
    }

}

void MainWindow::on_newWindowWidget_clicked()
{
   reportW.show();
}

void MainWindow::on_buttonNextWord_clicked()
{   QString phonetic,definition;
    if(wordTableLoadFlag==0){//表示加载的是搜索的数据。
        if(currentWordNumber<queryData.size()-1){
            qDebug()<<"queryDataSize: "<<queryData.size();
            qDebug()<<"currentWordNumber "<<currentWordNumber;
            currentWordNumber+=1;
            if(queryData[currentWordNumber]["phonetic"].size()>0){
                phonetic =" /"+queryData[currentWordNumber]["phonetic"]+"/";}
            if(queryData[currentWordNumber]["definition"].size()>0){
                definition ="\n\n英英:\n"+queryData[currentWordNumber]["definition"];}
            QString text="\n   "+queryData[currentWordNumber]["word"]+phonetic+"\n\n"+"简明:\n"+queryData[currentWordNumber]["translation"]+definition;
            currentWordDetail="\n   "+queryData[currentWordNumber]["word"]+"\n\n"+"简明:\n"+queryData[currentWordNumber]["translation"]+definition;
            currentWord=queryData[currentWordNumber]["word"];
            currentPhonetic=queryData[currentWordNumber]["phonetic"];
            currentDefinition=queryData[currentWordNumber]["definition"];
            currentTranslation=queryData[currentWordNumber]["translation"];
            ui->textEdit->clear();
            ui->textEdit->append(text);
            ui->textEdit->moveCursor(QTextCursor::Start);
            setWordButtonStatus();
        }
    }else{
        if(currentWordNumber<wordBookQuery.size()-1){
            currentWordNumber+=1;
            if(wordBookQuery[currentWordNumber]["phonetic"].size()>0){
                phonetic =" /"+wordBookQuery[currentWordNumber]["phonetic"]+"/";}
            if(wordBookQuery[currentWordNumber]["definition"].size()>0){
                definition ="\n\n英英:\n"+wordBookQuery[currentWordNumber]["definition"];}
            QString text="\n   "+wordBookQuery[currentWordNumber]["word"]+phonetic+"\n\n"+"简明:\n"+wordBookQuery[currentWordNumber]["translation"]+definition;
            currentWordDetail="\n   "+wordBookQuery[currentWordNumber]["word"]+"\n\n"+"简明:\n"+wordBookQuery[currentWordNumber]["translation"]+definition;
            currentWord=wordBookQuery[currentWordNumber]["word"];
            currentPhonetic=wordBookQuery[currentWordNumber]["phonetic"];
            currentDefinition=wordBookQuery[currentWordNumber]["definition"];
            currentTranslation=wordBookQuery[currentWordNumber]["translation"];
            ui->textEdit->clear();
            ui->textEdit->append(text);
            ui->textEdit->moveCursor(QTextCursor::Start);
            setWordButtonStatus();
        }
    }
}


void MainWindow::on_buttonLastWord_clicked()
{   QString phonetic,definition;
    if(wordTableLoadFlag==0){//表示加载的是搜索的数据。
        if(currentWordNumber>0){
            qDebug()<<"queryDataSize: "<<queryData.size();
            qDebug()<<"currentWordNumber "<<currentWordNumber;
            currentWordNumber-=1;
            if(queryData[currentWordNumber]["phonetic"].size()>0){
                phonetic =" /"+queryData[currentWordNumber]["phonetic"]+"/";}
            if(queryData[currentWordNumber]["definition"].size()>0){
                definition ="\n\n英英:\n"+queryData[currentWordNumber]["definition"];}
            QString text="\n   "+queryData[currentWordNumber]["word"]+phonetic+"\n\n"+"简明:\n"+queryData[currentWordNumber]["translation"]+definition;
            currentWordDetail="\n   "+queryData[currentWordNumber]["word"]+"\n\n"+"简明:\n"+queryData[currentWordNumber]["translation"]+definition;
            currentWord=queryData[currentWordNumber]["word"];
            currentPhonetic=queryData[currentWordNumber]["phonetic"];
            currentDefinition=queryData[currentWordNumber]["definition"];
            currentTranslation=queryData[currentWordNumber]["translation"];
            ui->textEdit->clear();
            ui->textEdit->append(text);
            ui->textEdit->moveCursor(QTextCursor::Start);
            setWordButtonStatus();
        }
    }else{
        if(currentWordNumber>0){
            currentWordNumber-=1;
            if(wordBookQuery[currentWordNumber]["phonetic"].size()>0){
                phonetic =" /"+wordBookQuery[currentWordNumber]["phonetic"]+"/";}
            if(wordBookQuery[currentWordNumber]["definition"].size()>0){
                definition ="\n\n英英:\n"+wordBookQuery[currentWordNumber]["definition"];}
            QString text="\n   "+wordBookQuery[currentWordNumber]["word"]+phonetic+"\n\n"+"简明:\n"+wordBookQuery[currentWordNumber]["translation"]+definition;
            currentWordDetail="\n   "+wordBookQuery[currentWordNumber]["word"]+"\n\n"+"简明:\n"+wordBookQuery[currentWordNumber]["translation"]+definition;
            currentWord=wordBookQuery[currentWordNumber]["word"];
            currentPhonetic=wordBookQuery[currentWordNumber]["phonetic"];
            currentDefinition=wordBookQuery[currentWordNumber]["definition"];
            currentTranslation=wordBookQuery[currentWordNumber]["translation"];
            ui->textEdit->clear();
            ui->textEdit->append(text);
            ui->textEdit->moveCursor(QTextCursor::Start);
            setWordButtonStatus();
        }
    }
}
