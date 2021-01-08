#ifndef REPORTWIDGET_H
#define REPORTWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QtCharts/QChartGlobal>
#include <QChart>
#include <QChartView>
QT_CHARTS_USE_NAMESPACE//没这行还用不了QChart了
typedef QMap<QString,int> qDataMap;
typedef QMap<QString,QList<double> > qDataFrame;


namespace Ui {
class reportWidget;
}

class reportWidget : public QWidget
{
    Q_OBJECT

public:
    explicit reportWidget(QWidget *parent = nullptr);
    ~reportWidget();

private:
    Ui::reportWidget *ui;
    QSqlDatabase db_History;
    void initDatabase();
    float px,py,pxx,pyy,hx,hy,kx,ky;
//    QMap<QString,QVector<QString> > *queryData;
    QString cleanMusicName(QString);
    QChartView *chartView;
    QList<QChartView *> m_charts;
    QChart *createAreaChart() const;
    QChart *createBarChart(int valueCount) const;
    QChart *createPieChart(qDataMap) const;
    QChart *createLineChart() const;
    QChart *createSplineChart() const;
    QChart *createScatterChart() const;
    QChart *createBoxplot(qDataFrame ,QString);
    QChart *createBarChart(QMap<int, QList<double> >,QStringList) const;

private slots:
    void init();
    void initBackGround();
    void loadMusicHistory();

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void getHistoryData();
    void getWeekHistoryData();
    void getMonthHistoryData();
    void on_pushButton_3_clicked();
    void initLabel();
    void loadHtml();
    double findMedian(int begin, int end,QList<double>);
//    void initChartView();
//    void updateUI();

};

#endif // REPORTWIDGET_H
