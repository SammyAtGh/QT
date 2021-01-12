#include "reportwidget.h"
#include "ui_reportwidget.h"
#include "CustomTabStyle.h"
#include <QGraphicsDropShadowEffect>
#include <QSqlTableModel>
#include <QDebug>
#include <QSqlQuery>
#include <QDateTime>
#include <QMouseEvent>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSet>
#include <QtWidgets/QGridLayout>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBoxSet>
#include <QtCharts/QBoxPlotSeries>

reportWidget::reportWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::reportWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    init();
}

reportWidget::~reportWidget()
{
    delete ui;
}

void reportWidget::init(){
    initBackGround();
    initDatabase();
    loadMusicHistory();
    initLabel();
    ui->tabWidget->tabBar()->setStyle(new CustomTabStyle);
    getHistoryData();
    getWeekHistoryData();
    getMonthHistoryData();
}

void reportWidget::initBackGround(){
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setOffset(0,0);
    shadow->setColor(QColor("#444444"));
    shadow->setBlurRadius(15);
    ui->tabWidget->setGraphicsEffect(shadow);
    ui->background->setGraphicsEffect(shadow);
}

void reportWidget::initDatabase(){
    db_History = QSqlDatabase::addDatabase("QSQLITE","db_History");
    db_History.setDatabaseName("musicHistory.db");
    if(!db_History.open())
    {
        qDebug()<<"history db fail.";
    }
}

void reportWidget::loadMusicHistory(){
    if(!db_History.open())
    {
        qDebug()<<"history db fail.";
        return;
    }
    QSqlTableModel *model = new QSqlTableModel(ui->tab,db_History);
    model ->setTable("MusicHistory");//
//    QSqlQuery query(db_History);
//    QString QSSelect("SELECT * FROM MusicHistory");
    //查询的起时间startDateTime,和止时间endDateTime回到查询语句中。
    QDateTime currentTime = QDateTime::currentDateTime();
    QString dataTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
//    qDebug()<<dataTime;
    QString timeStart = currentTime.addDays(-6).toString("yyyy-MM-dd hh:mm:ss");//选一周时间(-6)，包括当天。
    QString SqlStr = QString("DateTime < '%1' and DateTime > '%2'").arg(dataTime).arg(timeStart);
    model ->setFilter(SqlStr);//按这个方式查询
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
//    model->setHeaderData(0, Qt::Horizontal, tr("DateTime"));
//    model->setHeaderData(1, Qt::Horizontal, tr("MusicName"));
    if(model ->select()){
        qDebug()<<"数据查询成功";
    }
    ui->tableView->setModel(model);//用tableview控件显示查询后的结果
    ui->tableView->hideColumn(0);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->show();

}

void reportWidget::on_pushButton_clicked()
{
    qDebug()<<"按下查询历史按钮";
    loadMusicHistory();
}

void reportWidget::on_pushButton_2_clicked()
{
    this->close();
}
void reportWidget::mousePressEvent(QMouseEvent *event)
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

}

void reportWidget::mouseMoveEvent(QMouseEvent *event)
{
//    qDebug()<<event->source();
    if(px>10&&px<pxx-10&&py>20&&py<70)//中间区域（拖拽
         {this->setGeometry(hx+cursor().pos().x()-kx,hy+cursor().pos().y()-ky,pxx,pyy);}
    update();
}

void reportWidget::getHistoryData(){
    qDebug()<<"Do getHistoryData";
    if(!db_History.open())
    {
        qDebug()<<"history db fail.";
    }
    QSqlQuery query(db_History);
    QString sql("SELECT * FROM MusicHistory");
    bool a = query.exec(sql);//本地听音乐历史数据
    qDebug()<<sql;
    if(!a){
        qDebug()<<"query error :"<<query.size();
        return;
    }
    else{
        qDebug()<<"Query music history success!";
    }
    //获取所有查询的数据，并添加想要的数据列<start>
    QVector<QString> queryDateTime,queryMusicName,queryFileName,weekList,yearList,monthList;
    QVector<int> queryListenTime,queryListenCount;
    qDataMap weekListenTime;
    QStringList weekNameList;
    qDataFrame qHourData;
    weekNameList<<"周一"<<"周二"<<"周三"<<"周四"<<"周五"<<"周六"<<"周日";
//            qDataMap()<<("周一",0)<<("周一",0)<<("周二",0)<<("周三",0)<<("周四",0)<<("周五",0)<<("周六",0)<<("周日",0);
    foreach(auto weekName,weekNameList){
        weekListenTime.insert(weekName,0);
    }
    int listenMusicTotalCount = 0;
    int listenTimeTotal = 0;
    //计算所有听歌总数不再另开循环。
    qDebug()<<55;
    while(query.next()){
        queryDateTime.append(query.value(1).toString());
        queryMusicName.append(cleanMusicName(query.value(2).toString()));
        queryFileName.append(query.value(3).toString());
        queryListenTime.append(query.value(4).toInt());
        queryListenCount.append(query.value(5).toInt());
        //计算总时长及总听歌次数
        listenMusicTotalCount+=query.value(5).toInt();
        listenTimeTotal+=query.value(4).toInt();
        //from DateTime to week
        QDateTime dateTime = QDateTime::fromString(query.value(1).toString(), "yyyy-MM-dd hh:mm:ss");
        QString week = dateTime.toLocalTime().toString("ddd");
        weekList.append(week);
        weekListenTime[week]+=query.value(4).toInt();
        //hour变量
        int hour = dateTime.toString("hh").toInt();
        if(hour>=0&&hour<6){
            qHourData["Deep Night"].append(query.value(4).toInt());
        }else if(hour>=6&&hour<12){
            qHourData["Morning"].append(query.value(4).toInt());
        }else if(hour>=12&&hour<19){
            qHourData["Afternoon"].append(query.value(4).toInt());
        }else if(hour>=19&&hour<=23){
            qHourData["Night"].append(query.value(4).toInt());
        }
    }
    if(queryDateTime.size()<1){
        return;
    }
    qDebug()<<66;
//    qDebug()<<qHourData;
    //<end>

    //听歌次数计数及排序。歌曲总数也可以在此获得获得。<start>
    for (int i=0;i<5;i++) {
//        qDebug()<<queryDateTime[i]<<queryMusicName[i]<<queryFileName[i]<<queryListenTime[i]<<queryListenCount[i];
    }
    QVector<QString> tempQueryMusicName(queryMusicName);
//    QList musicNameList=tempQueryMusicName.toList();
    //tQMNS存储所有查询到的歌曲集合
    QSet<QString> tQMNS(tempQueryMusicName.begin(),tempQueryMusicName.end());
//    qDebug()<<tQMNS;
    QMap<QString,int> calculateListenCount;
    int musicCount = tQMNS.size();
    if(musicCount<3){
        return;//歌曲数量只有大于等于三，后面读取前三首听最多的歌曲才不会，出bug.
    }
//    qDebug()<<musicCount;
    foreach(auto a ,tQMNS){
        calculateListenCount.insert(a,0);
    }
    for (int  j= 0;j<queryMusicName.size();j++) {
        if(calculateListenCount.contains(queryMusicName[j])){
            calculateListenCount[queryMusicName[j]]+=queryListenCount[j];

        }
    }
    QList<int> countList = calculateListenCount.values();
    std::sort(countList.begin(),countList.end());
    QList<int> maxCountX3(countList.end()-3,countList.end());
    QList<QString> maxCountMusicX3;
    foreach(int maxCount,maxCountX3){
        foreach(auto music,tQMNS){
            if(calculateListenCount[music]==maxCount){
                if(maxCountMusicX3.contains(music)){
                    continue;
                }
                maxCountMusicX3.append(music);
                break;
            }
        }
    }
//    qDebug()<<calculateListenCount;
    qDebug()<<maxCountMusicX3;
    qDebug()<<maxCountX3;
    QString listenCountDisplayText=QString("<p style='line-height:80%'><font size = 4>嗨，你这段时间听的最多的3首歌是：</font></p>"
                                           "<p style='line-height:80%'>%1,</p>"
                                           "<p style='line-height:80%'>%2,</p>"
                                           "<p style='line-height:80%'>%3;</p>"
                                           "<p style='line-height:90%'><font size = 4>%4听了 <b>%5</b> 次呢！</font></p>")
            .arg(maxCountMusicX3[2]).arg(maxCountMusicX3[1]).arg(maxCountMusicX3[0]).arg(maxCountMusicX3[2]).arg(maxCountX3[2]);
    ui->label->setText(listenCountDisplayText);
//    ui->label->setText(QString("<p>嗨，你这段时间听的最多的3首歌是：<p>%1, %2, %3;\n%4听了 %5 次呢！").arg(maxCountMusicX3[2]).arg(maxCountMusicX3[1]).arg(maxCountMusicX3[0]).arg(maxCountMusicX3[2]).arg(maxCountX3[2]));
    ui->label->adjustSize();
    ui->label->setWordWrap(true);
    ui->label->setAlignment(Qt::AlignTop);
    //<end>
    //听歌总时长换算时、分并显示label上<start>
    int minuteTotal = listenTimeTotal/60;
    int hour = minuteTotal/60;
//    int minute = minuteTotal%60;
    QString line1("听歌时长  ");
    QString line2=QString("  %1 小时").arg(hour);
    ui->label_2->setText(QString("<p><font color = #777985 ; size = 5  >%1</font></p>").arg(line1)+
                                 "<p style='line-height:80%' ></p>"+
                                   QString("<p><font color = #000000 ; size = 7 >%1</font></p>").arg(line2));
    ui->label_2->setWordWrap(true);
    ui->label_2->adjustSize();
    ui->label_2->setAlignment(Qt::AlignTop);
    //<end>
    //听歌总数 <start>
    QString lineT1("歌曲总数");
    QString lineT2=QString("%1").arg(musicCount);
    ui->label_3->setText(QString("<p><font color = #777985 ; size = 5  >%1</font></p>").arg(lineT1)+
                                 "<p style='line-height:80%' ></p>"+
                                   QString("<p><font color = #000000 ; size = 7 >%1</font></p>").arg(lineT2));
    ui->label_3->setWordWrap(true);
    ui->label_3->setAlignment(Qt::AlignTop);
    ui->label_3->adjustSize();
    //</end>
    //按周统计时长，做饼图；start

//    QChart *pieWeekListenTimeChar = createPieChart(weekListenTime);
//    pieWeekListenTimeChar->setTheme(QChart::ChartThemeBrownSand);
////    pieWeekListenTimeChar->setBackgroundBrush(QBrush(QRgb(0XF69000)));
//    pieWeekListenTimeChar->legend()->hide();

//    QChartView *chartView = new QChartView(pieWeekListenTimeChar);
//    chartView->setRenderHint(QPainter::Antialiasing);
//    QGridLayout *baseLayout = new QGridLayout();
//    baseLayout->addWidget(chartView, 0, 0);
//    if(ui->charViewPie->layout()==NULL){
//        ui->charViewPie->setLayout(baseLayout);
//    }else{
//        ui->charViewPie->layout()->activate();
//    }
//    //end
    //按当天时间算是night，deep night , morning or afternoon,做箱型图。
    QChart *hourBoxplot = createBoxplot(qHourData,"Date Cat");

    hourBoxplot->setTheme(QChart::ChartThemeBrownSand);
    hourBoxplot->legend()->hide();

    QChartView *boxPlotView = new QChartView(hourBoxplot);
    boxPlotView->setRenderHint(QPainter::Antialiasing);
    QGridLayout *baseLayout2 = new QGridLayout();
    baseLayout2->addWidget(boxPlotView, 0, 0);
    if(ui->charViewBoxPlot->layout()==NULL){
        ui->charViewBoxPlot->setLayout(baseLayout2);
    }else{
        delete ui->charViewBoxPlot->layout();
        ui->charViewBoxPlot->setLayout(baseLayout2);
    }

}

void reportWidget::getWeekHistoryData(){
    qDebug()<<"Do getWeekHistoryData";
    if(!db_History.open())
    {
        qDebug()<<"history db fail.";
        return;
    }
    QSqlQuery query(db_History);
    QDateTime currentTime = QDateTime::currentDateTime();
    QString dataTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
//    qDebug()<<dataTime;
    QString timeStart = currentTime.addDays(-6).toString("yyyy-MM-dd hh:mm:ss");//选一个月时间(-6)，包括当天。
    QString SqlStr = QString(" Where DateTime < '%1' and DateTime > '%2'").arg(dataTime).arg(timeStart);
    QString sql = QString("SELECT * FROM MusicHistory%1").arg(SqlStr);
    qDebug()<<sql;
    bool a = query.exec(sql);//本地听音乐历史数据
    if(!a){
        qDebug()<<"query error :"<<query.size();
        return;
    }
    else{
        qDebug()<<"Query music history success!";
    }
    //获取所有查询的数据，并添加想要的数据列<start>
    QVector<QString> queryDateTime,queryMusicName,queryFileName,weekList,yearList,monthList;
    QVector<int> queryListenTime,queryListenCount;
    qDataMap weekListenTime;
    QStringList weekNameList;
    qDataFrame qHourData;
    weekNameList<<"周一"<<"周二"<<"周三"<<"周四"<<"周五"<<"周六"<<"周日";
//            qDataMap()<<("周一",0)<<("周一",0)<<("周二",0)<<("周三",0)<<("周四",0)<<("周五",0)<<("周六",0)<<("周日",0);
//    foreach(auto weekName,weekNameList){
//        weekListenTime.insert(weekName,0);
//    }
    int listenMusicTotalCount = 0;
    int listenTimeTotal = 0;
    //计算所有听歌总数不再另开循环。
    while(query.next()){
        queryDateTime.append(query.value(1).toString());
        queryMusicName.append(cleanMusicName(query.value(2).toString()));
        queryFileName.append(query.value(3).toString());
        queryListenTime.append(query.value(4).toInt());
        queryListenCount.append(query.value(5).toInt());
        //计算总时长及总听歌次数
        listenMusicTotalCount+=query.value(5).toInt();
        listenTimeTotal+=query.value(4).toInt();
        //from DateTime to week
        QDateTime dateTime = QDateTime::fromString(query.value(1).toString(), "yyyy-MM-dd hh:mm:ss");
        QString week = dateTime.toLocalTime().toString("ddd");
        weekList.append(week);
        weekListenTime[week]+=query.value(4).toInt();
        //hour变量
        int hour = dateTime.toString("hh").toInt();
        if(hour>=0&&hour<6){
            qHourData["Deep Night"].append(query.value(4).toInt());
        }else if(hour>=6&&hour<12){
            qHourData["Morning"].append(query.value(4).toInt());
        }else if(hour>=12&&hour<19){
            qHourData["Afternoon"].append(query.value(4).toInt());
        }else if(hour>=19&&hour<=23){
            qHourData["Night"].append(query.value(4).toInt());
        }


    }
    if(queryDateTime.size()<1){
        return;
    }
//    qDebug()<<qHourData;
    //<end>

    //听歌次数计数及排序。歌曲总数也可以在此获得获得。<start>
    for (int i=0;i<5;i++) {
//        qDebug()<<queryDateTime[i]<<queryMusicName[i]<<queryFileName[i]<<queryListenTime[i]<<queryListenCount[i];
    }
    QVector<QString> tempQueryMusicName(queryMusicName);
//    QList musicNameList=tempQueryMusicName.toList();
    //tQMNS存储所有查询到的歌曲集合
    QSet<QString> tQMNS(tempQueryMusicName.begin(),tempQueryMusicName.end());
//    qDebug()<<tQMNS;
    QMap<QString,int> calculateListenCount;
    int musicCount = tQMNS.size();
    if(musicCount<3){
        return;
    }
//    qDebug()<<musicCount;
    foreach(auto a ,tQMNS){
        calculateListenCount.insert(a,0);
    }
    for (int  j= 0;j<queryMusicName.size();j++) {
        if(calculateListenCount.contains(queryMusicName[j])){
            calculateListenCount[queryMusicName[j]]+=queryListenCount[j];

        }
    }
    QList<int> countList = calculateListenCount.values();
    std::sort(countList.begin(),countList.end());
    QList<int> maxCountX3(countList.end()-3,countList.end());
    QList<QString> maxCountMusicX3;
    foreach(int maxCount,maxCountX3){
        foreach(auto music,tQMNS){
            if(calculateListenCount[music]==maxCount){
                if(maxCountMusicX3.contains(music)){
                    continue;
                }
                maxCountMusicX3.append(music);
                break;
            }
        }
    }
//    qDebug()<<calculateListenCount;
    qDebug()<<maxCountMusicX3;
    qDebug()<<maxCountX3;
    QString listenCountDisplayText=QString("<p style='line-height:80%'><font size = 4>嗨，你这周听的最多的3首歌是：</font></p>"
                                           "<p style='line-height:80%'>%1,</p>"
                                           "<p style='line-height:80%'>%2,</p>"
                                           "<p style='line-height:80%'>%3;</p>"
                                           "<p style='line-height:90%'><font size = 4>%4听了 <b>%5</b> 次呢！</font></p>")
            .arg(maxCountMusicX3[2]).arg(maxCountMusicX3[1]).arg(maxCountMusicX3[0]).arg(maxCountMusicX3[2]).arg(maxCountX3[2]);
    ui->label_weekDetail->setText(listenCountDisplayText);
    ui->label_weekDetail->adjustSize();
    ui->label_weekDetail->setWordWrap(true);
    ui->label_weekDetail->setAlignment(Qt::AlignTop);
    //<end>
    //听歌总时长换算时、分并显示label上<start>
    int minuteTotal = listenTimeTotal/60;
    int hour = minuteTotal/60;
//    int minute = minuteTotal%60;
    QString line1("听歌时长  ");
    QString line2=QString("  %1 小时").arg(hour);
    ui->label_weekTotalHour->setText(QString("<p><font color = #777985 ; size = 5  >%1</font></p>").arg(line1)+
                                 "<p style='line-height:80%' ></p>"+
                                   QString("<p><font color = #000000 ; size = 7 >%1</font></p>").arg(line2));
    ui->label_weekTotalHour->setWordWrap(true);
    ui->label_weekTotalHour->adjustSize();
    ui->label_weekTotalHour->setAlignment(Qt::AlignTop);
    //<end>
    //听歌总数 <start>
    QString lineT1("歌曲总数");
    QString lineT2=QString("%1").arg(musicCount);
    ui->label_weekTotalSong->setText(QString("<p><font color = #777985 ; size = 5  >%1</font></p>").arg(lineT1)+
                                 "<p style='line-height:80%' ></p>"+
                                   QString("<p><font color = #000000 ; size = 7 >%1</font></p>").arg(lineT2));
    ui->label_weekTotalSong->setWordWrap(true);
    ui->label_weekTotalSong->setAlignment(Qt::AlignTop);
    ui->label_weekTotalSong->adjustSize();
    //</end>
    //按周统计时长，做饼图；start

    QChart *pieWeekListenTimeChar = createPieChart(weekListenTime);
    pieWeekListenTimeChar->setTheme(QChart::ChartThemeBrownSand);
//    pieWeekListenTimeChar->setBackgroundBrush(QBrush(QRgb(0XF69000)));
//    pieWeekListenTimeChar->legend()->hide();

    QChartView *chartView = new QChartView(pieWeekListenTimeChar);
    chartView->setRenderHint(QPainter::Antialiasing);
    QGridLayout *baseLayout = new QGridLayout();
    baseLayout->addWidget(chartView, 0, 0);
    if(ui->charViewPie->layout()==NULL){
        ui->charViewPie->setLayout(baseLayout);
    }else{
        delete (ui->charViewPie->layout());
        ui->charViewPie->setLayout(baseLayout);
    }
    //end
    //按当天时间算是night，deep night , morning or afternoon,做箱型图。
//    QChart *hourBoxplot = createBoxplot(qHourData,"Date Cat");

//    hourBoxplot->setTheme(QChart::ChartThemeBrownSand);
//   pieWeekListenTimeChar->setBackgroundBrush(QBrush(QRgb(0XF69000)));
//    hourBoxplot->legend()->hide();

//    QChartView *boxPlotView = new QChartView(hourBoxplot);
//    boxPlotView->setRenderHint(QPainter::Antialiasing);
//    QGridLayout *baseLayout2 = new QGridLayout();
//    baseLayout2->addWidget(boxPlotView, 0, 0);
//    if(ui->charViewBoxPlot->layout()==NULL){
//        ui->charViewBoxPlot->setLayout(baseLayout2);
//    }else{
//        ui->charViewBoxPlot->layout()->activate();
//    }

}

void reportWidget::getMonthHistoryData(){
    //Todo
    qDebug()<<"Do getMonthHistoryData";
    if(!db_History.open())
    {
        qDebug()<<"history db fail.";
        return;
    }
    QSqlQuery query(db_History);
    QDateTime currentTime = QDateTime::currentDateTime();
    QString dataTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
//    qDebug()<<dataTime;
    QString timeStart = currentTime.addDays(-29).toString("yyyy-MM-dd hh:mm:ss");//选一个月时间(-29)，包括当天。
    QString SqlStr = QString(" Where DateTime < '%1' and DateTime > '%2'").arg(dataTime).arg(timeStart);
    QString sql = QString("SELECT * FROM MusicHistory%1").arg(SqlStr);
    qDebug()<<sql;
    bool a = query.exec(sql);//本地听音乐历史数据
    if(!a){
        qDebug()<<"query error :"<<query.size();
        return;
    }
    else{
        qDebug()<<"Query music history success!";
    }
    //获取所有查询的数据，并添加想要的数据列<start>
    QVector<QString> queryDateTime,queryMusicName,queryFileName,weekList,yearList,monthList;
    QVector<int> queryListenTime,queryListenCount;
    qDataMap weekListenTime;
    QStringList weekNameList;
    qDataFrame qHourData;
    QMap<int,QList<double> > qDateCat;
    weekNameList<<"周一"<<"周二"<<"周三"<<"周四"<<"周五"<<"周六"<<"周日";
//            qDataMap()<<("周一",0)<<("周一",0)<<("周二",0)<<("周三",0)<<("周四",0)<<("周五",0)<<("周六",0)<<("周日",0);
    foreach(auto weekName,weekNameList){
        weekListenTime.insert(weekName,0);
    }
    int listenMusicTotalCount = 0;
    int listenTimeTotal = 0;
    //计算所有听歌总数不再另开循环。
    while(query.next()){
        queryDateTime.append(query.value(1).toString());
        queryMusicName.append(cleanMusicName(query.value(2).toString()));
        queryFileName.append(query.value(3).toString());
        queryListenTime.append(query.value(4).toInt());
        queryListenCount.append(query.value(5).toInt());
        //计算总时长及总听歌次数
        listenMusicTotalCount+=query.value(5).toInt();
        listenTimeTotal+=query.value(4).toInt();
        //from DateTime to week
        QDateTime dateTime = QDateTime::fromString(query.value(1).toString(), "yyyy-MM-dd hh:mm:ss");
        QString week = dateTime.toLocalTime().toString("ddd");
        weekList.append(week);
        weekListenTime[week]+=query.value(4).toInt();
        //hour变量
        int day = dateTime.toString("dd").toInt();
        int hour = dateTime.toString("hh").toInt();
        if(qDateCat[day].size()==0){
            qDateCat[day]<<0.0<<0.0<<0.0<<0.0;
        }
        if(hour>=0&&hour<6){
            qHourData["Deep Night"].append(query.value(4).toInt());
            qDateCat[day][0]+=(query.value(4).toDouble())/3600.0;
        }else if(hour>=6&&hour<12){
            qHourData["Morning"].append(query.value(4).toInt());
            qDateCat[day][1]+=(query.value(4).toDouble())/3600.0;
        }else if(hour>=12&&hour<19){
            qHourData["Afternoon"].append(query.value(4).toInt());
            qDateCat[day][2]+=(query.value(4).toDouble())/3600.0;
        }else if(hour>=19&&hour<=23){
            qHourData["Night"].append(query.value(4).toInt());
            qDateCat[day][3]+=(query.value(4).toDouble())/3600.0;
        }


    }
    if(queryDateTime.size()<1){
        return;
    }
    qDebug()<<qDateCat;
    //<end>

    //听歌次数计数及排序。歌曲总数也可以在此获得获得。<start>
    for (int i=0;i<5;i++) {
//        qDebug()<<queryDateTime[i]<<queryMusicName[i]<<queryFileName[i]<<queryListenTime[i]<<queryListenCount[i];
    }
    QVector<QString> tempQueryMusicName(queryMusicName);
//    QList musicNameList=tempQueryMusicName.toList();
    //tQMNS存储所有查询到的歌曲集合
    QSet<QString> tQMNS(tempQueryMusicName.begin(),tempQueryMusicName.end());
//    qDebug()<<tQMNS;
    QMap<QString,int> calculateListenCount;
    int musicCount = tQMNS.size();
    if(musicCount<3){
        return;
    }
//    qDebug()<<musicCount;
    foreach(auto a ,tQMNS){
        calculateListenCount.insert(a,0);
    }
    for (int  j= 0;j<queryMusicName.size();j++) {
        if(calculateListenCount.contains(queryMusicName[j])){
            calculateListenCount[queryMusicName[j]]+=queryListenCount[j];

        }
    }
    QList<int> countList = calculateListenCount.values();
    std::sort(countList.begin(),countList.end());
    QList<int> maxCountX3(countList.end()-3,countList.end());
    QList<QString> maxCountMusicX3;
    foreach(int maxCount,maxCountX3){
        foreach(auto music,tQMNS){
            if(calculateListenCount[music]==maxCount){
                if(maxCountMusicX3.contains(music)){
                    continue;
                }
                maxCountMusicX3.append(music);
                break;
            }
        }
    }
//    qDebug()<<calculateListenCount;
    qDebug()<<maxCountMusicX3;
    qDebug()<<maxCountX3;
//    QString listenCountDisplayText=QString("<p style='line-height:80%'><font size = 4>嗨，你这个月听的最多的3首歌是：</font></p>"
//                                           "<p style='line-height:80%'>%1,</p>"
//                                           "<p style='line-height:80%'>%2,</p>"
//                                           "<p style='line-height:80%'>%3;</p>"
//                                           "<p style='line-height:90%'><font size = 4>%4听了 <b>%5</b> 次呢！</font></p>")
//            .arg(maxCountMusicX3[2]).arg(maxCountMusicX3[1]).arg(maxCountMusicX3[0]).arg(maxCountMusicX3[2]).arg(maxCountX3[2]);
//    ui->label_weekDetail->setText(listenCountDisplayText);
//    ui->label_weekDetail->adjustSize();
//    ui->label_weekDetail->setWordWrap(true);
//    ui->label_weekDetail->setAlignment(Qt::AlignTop);
    //<end>
    //听歌总时长换算时、分并显示label上<start>
    int minuteTotal = listenTimeTotal/60;
    int hour = minuteTotal/60;
//    int minute = minuteTotal%60;
    QString line1("听歌时长  ");
    QString line2=QString("  %1 小时").arg(hour);
    ui->label_monthTotalHour->setText(QString("<p><font color = #777985 ; size = 5  >%1</font></p>").arg(line1)+
                                 "<p style='line-height:80%' ></p>"+
                                   QString("<p><font color = #000000 ; size = 7 >%1</font></p>").arg(line2));
    ui->label_monthTotalHour->setWordWrap(true);
    ui->label_monthTotalHour->adjustSize();
    ui->label_monthTotalHour->setAlignment(Qt::AlignTop);
    //<end>
    //听歌总数 <start>
    QString lineT1("歌曲总数");
    QString lineT2=QString("%1").arg(musicCount);
    ui->label_monthTotalSong->setText(QString("<p><font color = #777985 ; size = 5  >%1</font></p>").arg(lineT1)+
                                 "<p style='line-height:80%' ></p>"+
                                   QString("<p><font color = #000000 ; size = 7 >%1</font></p>").arg(lineT2));
    ui->label_monthTotalSong->setWordWrap(true);
    ui->label_monthTotalSong->setAlignment(Qt::AlignTop);
    ui->label_monthTotalSong->adjustSize();
    //</end>
    //做barChart：start
    QChart *monthBarchat=createBarChart(qDateCat,QStringList{"Deep Night","Morning","Afternoon","Night"});
    monthBarchat->setTheme(QChart::ChartThemeBrownSand);
    QChartView *barChartView = new QChartView(monthBarchat);
    barChartView->setRenderHint(QPainter::Antialiasing);
    QGridLayout *barLayout = new QGridLayout();
    barLayout->addWidget(barChartView, 0, 0);
    if(ui->charViewBar->layout()==NULL){
        ui->charViewBar->setLayout(barLayout);
    }else{
        delete (ui->charViewBar->layout());
        ui->charViewBar->setLayout(barLayout);
    }
    //end
    //按周统计时长，做饼图；start
//    QChart *pieWeekListenTimeChar = createPieChart(weekListenTime);
//    pieWeekListenTimeChar->setTheme(QChart::ChartThemeBrownSand);
//    pieWeekListenTimeChar->setBackgroundBrush(QBrush(QRgb(0XF69000)));
//    pieWeekListenTimeChar->legend()->hide();

//    QChartView *chartView = new QChartView(pieWeekListenTimeChar);
//    chartView->setRenderHint(QPainter::Antialiasing);
//    QGridLayout *baseLayout = new QGridLayout();
//    baseLayout->addWidget(chartView, 0, 0);
//    if(ui->charViewPie->layout()==NULL){
//        ui->charViewPie->setLayout(baseLayout);
//    }else{
//        ui->charViewPie->layout()->activate();
//    }
    //end
    //按当天时间算是night，deep night , morning or afternoon,做箱型图。
//    QChart *hourBoxplot = createBoxplot(qHourData,"Date Cat");

//    hourBoxplot->setTheme(QChart::ChartThemeBrownSand);
//   pieWeekListenTimeChar->setBackgroundBrush(QBrush(QRgb(0XF69000)));
//    hourBoxplot->legend()->hide();

//    QChartView *boxPlotView = new QChartView(hourBoxplot);
//    boxPlotView->setRenderHint(QPainter::Antialiasing);
//    QGridLayout *baseLayout2 = new QGridLayout();
//    baseLayout2->addWidget(boxPlotView, 0, 0);
//    if(ui->charViewBoxPlot->layout()==NULL){
//        ui->charViewBoxPlot->setLayout(baseLayout2);
//    }else{
//        ui->charViewBoxPlot->layout()->activate();
//    }
}

void reportWidget::on_pushButton_3_clicked()
{
    getHistoryData();
    getWeekHistoryData();
    getMonthHistoryData();
}

void reportWidget::initLabel()
{
//    ui->label->adjustSize();
//    ui->label->setGeometry(QRect(328, 240, 329, 27*4)); //四倍行距
    ui->label->setWordWrap(true);
    ui->label->setAlignment(Qt::AlignTop);
}

QString reportWidget::cleanMusicName(QString musicName){
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

void reportWidget::loadHtml(){
//    QWebView view= new QWebView;
//    QUrl url = QUrl::fromUserInput("http://www.baidu.com");
//    view->load(url);
//    int i = 0;
//    ui->gridLayout->addWidget(view, i/2 , i%2); //添加到布局中
}



QChart *reportWidget::createPieChart(qDataMap m_weekListenTime) const
{
    QChart *chart = new QChart();
    chart->setTitle("Week report");

    QPieSeries *series = new QPieSeries(chart);
    QMap<QString,int>::Iterator it1=m_weekListenTime.begin();
    while(it1!=m_weekListenTime.end())
    {
        QPieSlice *slice = series->append(it1.key(), it1.value());
//        if ("周六" == it1.key()) {
            // Show the first slice exploded with label
            slice->setLabelVisible();
//            slice->setExploded();
//            slice->setExplodeDistanceFactor(0.5);
//        }
        it1++;
    }
    series->setVerticalPosition(0.54);
    series->setPieSize(0.8);
    chart->addSeries(series);
    return chart;
}


QChart * reportWidget::createBoxplot(qDataFrame qDF,QString boxPlotName){
    if(qDF.size()>0){
        QStringList columns = qDF.keys();
        QBoxPlotSeries *boxSeries = new QBoxPlotSeries();
        boxSeries->setName(boxPlotName);
        foreach(auto column , columns){
            QList<double> sortedList=qDF[column];
            std::sort(sortedList.begin(), sortedList.end());
            int count = sortedList.count();
            QBoxSet *box = new QBoxSet(column);
            box->setValue(QBoxSet::LowerExtreme, sortedList.first());
            box->setValue(QBoxSet::UpperExtreme, sortedList.last());
            box->setValue(QBoxSet::Median, findMedian(0, count,sortedList));
            box->setValue(QBoxSet::LowerQuartile, findMedian(0, count / 2,sortedList));
            box->setValue(QBoxSet::UpperQuartile, findMedian(count / 2 + (count % 2), count,sortedList));
            boxSeries->append(box);
        }
        QChart *chart = new QChart();
        chart->addSeries(boxSeries);
        chart->setTitle(boxPlotName);
        chart->setAnimationOptions(QChart::SeriesAnimations);
        //! [5]
        chart->createDefaultAxes();
//        chart->axes(Qt::Vertical).first()->setMin(15.0);
//        chart->axes(Qt::Horizontal).first()->setMax(34.0);
        //! [5]

        //! [6]
        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);
        //! [6]

        return chart;
    }else{
        return nullptr;
    }
}

double reportWidget::findMedian(int begin, int end,QList<double> sortedList){
    //! [5]
    int count = end - begin;
    if (count % 2) {
        return sortedList.at(count / 2 + begin);
    } else {
        double right = sortedList.at(count / 2 + begin);
        double left = sortedList.at(count / 2 - 1 + begin);
        return (right + left) / 2.0;
    }
    //! [5]
}

QChart* reportWidget::createBarChart(QMap<int, QList<double> > qDateCat_t,QStringList dayCats_t) const
{
    QList<int> dayList= qDateCat_t.keys();
    int dayCatsSize = dayCats_t.size();
    QBarSet *sets[dayCatsSize];
    for(int i=0;i<dayCatsSize;i++){
       sets[i]=new QBarSet(dayCats_t[i]);
    }
    foreach(int day, dayList){
        for(int i=0;i<dayCatsSize;i++){
            sets[i]->append(qDateCat_t[day][i]);
        }
    }
    QStackedBarSeries *series = new QStackedBarSeries();
    for(int i=0;i<dayCatsSize;i++){
        series->append(sets[i]);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Listen time of each day in Month");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    foreach(int day, dayList){
        categories<<QString::number(day);
    }
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    return chart;
}
