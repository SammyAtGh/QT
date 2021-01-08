#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QMediaPlaylist>
#include <QMediaPlayer>
#include <QStringList>
#include <QDir>
#include <QPaintEvent>
#include <QtSql>
#include <reportwidget.h>
//#include "connection.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	
private:
	Ui::MainWindow *ui;
	bool isPlay = false;
    QString defaultMusicPath = "C:\\";
	QStringList fileList;  //文件名称
	QMediaPlaylist *PlayerList;
	QMediaPlayer *Player;
    float px,py,pxx,pyy,hx,hy,kx,ky;
    QTime max_time,pos_time;
    int maxProgressValue,currentProgressValue;
    int modeFlag=1;//0表示列表循环，1表示单曲循环，2表示随机播放。
    int likeFlag=0;//0表示不喜欢，1表示喜欢；
    double time_Start;
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
//    bool eventFilter(QObject *obj, QEvent *e);
    int swapFlag=0;
    int searchButtonFlag=1;
    void setBackground();
    void addWordItem(QString);
    QStringList fuzzyWordList;
    QVector<QMap<QString,QString> > queryData;
    QVector<QMap<QString,QString> > wordBookQuery;
    int wordButtonFlag=0;//0表示未加入单词本，1表示加入；
    QSqlDatabase db_music,db_stardic,db_wordBook,db_change_music,db_MusicHistory;
    void initDataBase();
    QString currentWordDetail,currentWord,currentPhonetic,currentDefinition,currentTranslation;
    int currentWordNumber=0;
    int wordTableLoadFlag=0;//0表示加载的是fuzzySearch的数据，1表示加载的是wordBook的数据
    reportWidget reportW;
//    QTimer *listenTimer=new QTimer(this);
    QString lastSongPath="";
    QString lastSongName="";
    QString lastDateTime;
    double lastTimeStartForHistory,lastSongTotalTime,listenTime,pauseTime=0,pauseTimeStart;
    bool lastIsPlay;
    int listenCount=0;
    QString cleanMusicName(QString);

private slots:
	void init();
    void playTo(int,int);
	void addItem(QString name);		//添加tableWidget
	QStringList getFileNames(const QString &path);	//获取路径下所有需要的文件类型文件 
	
	void SlotShowCurrentMedia();
	void SlotButtonStart();
	void SlotButtonPrevious();
	void SlotButtonNext();
    void initMusicTable();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void removeRow(int);
    void change_bar_position();
    void initVolumeSlider();
    void setVolume(int);
    void initProgressSlider();
    void progressPressed();
    void progressMoved(int);
    void progressReleased();
    void openFiles();
    void setMode();

    void setLike();
    void addLikeSong();
    void delLikeSong();
    void setLikeButton();
    void changeToLikeSongs();
    void pressLikeButton();
    void releaseLikeButton();

    void swapMode();
    void clickSearchButton();
    void fuzzySearch(const QString &text);
    void showWord(const QString &text);
    void initWordTable();
    void resetWordTable(QStringList);
    void chooseWord(int);
    void changeWT();

    void changeToWordDetail(int, int);
    void addToWordBook();
    void delFromWordBook();
    void setWordButtonStatus();
    void clickLWordButton();
    void on_buttonNextWord_clicked();
    void pressWordButton();
    void releaseWordButton();
    void changeToWordBook();
    void on_newWindowWidget_clicked();
    void on_buttonLastWord_clicked();

    void insertHistoryToDB();
    void calculatePauseTime();
    void setLastSongDurationTime(qint64 );
};

#endif // MAINWINDOW_H
