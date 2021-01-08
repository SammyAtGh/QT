#ifndef CONNECTION_H
#define CONNECTION_H

#endif // CONNECTION_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
static bool CreatConnection()
{

QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");//建库
db.setDatabaseName("like.db");//给数据库起名字
if(!db.open())
{
    return false;
}
QSqlQuery query;
bool a = query.exec(QString("create table LocalMusic (id integer primary key,MusicName varchar,FileName varchar);"));//本地音乐列表数据，把歌曲名和歌曲路径给存起来
if(!a){
    qDebug()<<"query error :"<<query.size();
}
else{
    qDebug()<<"insert data success!";
}
return true;

}
