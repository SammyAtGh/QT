/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[57];
    char stringdata0[809];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 4), // "init"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 6), // "playTo"
QT_MOC_LITERAL(4, 24, 7), // "addItem"
QT_MOC_LITERAL(5, 32, 4), // "name"
QT_MOC_LITERAL(6, 37, 12), // "getFileNames"
QT_MOC_LITERAL(7, 50, 4), // "path"
QT_MOC_LITERAL(8, 55, 20), // "SlotShowCurrentMedia"
QT_MOC_LITERAL(9, 76, 15), // "SlotButtonStart"
QT_MOC_LITERAL(10, 92, 18), // "SlotButtonPrevious"
QT_MOC_LITERAL(11, 111, 14), // "SlotButtonNext"
QT_MOC_LITERAL(12, 126, 14), // "initMusicTable"
QT_MOC_LITERAL(13, 141, 15), // "mousePressEvent"
QT_MOC_LITERAL(14, 157, 12), // "QMouseEvent*"
QT_MOC_LITERAL(15, 170, 5), // "event"
QT_MOC_LITERAL(16, 176, 14), // "mouseMoveEvent"
QT_MOC_LITERAL(17, 191, 9), // "removeRow"
QT_MOC_LITERAL(18, 201, 19), // "change_bar_position"
QT_MOC_LITERAL(19, 221, 16), // "initVolumeSlider"
QT_MOC_LITERAL(20, 238, 9), // "setVolume"
QT_MOC_LITERAL(21, 248, 18), // "initProgressSlider"
QT_MOC_LITERAL(22, 267, 15), // "progressPressed"
QT_MOC_LITERAL(23, 283, 13), // "progressMoved"
QT_MOC_LITERAL(24, 297, 16), // "progressReleased"
QT_MOC_LITERAL(25, 314, 9), // "openFiles"
QT_MOC_LITERAL(26, 324, 7), // "setMode"
QT_MOC_LITERAL(27, 332, 7), // "setLike"
QT_MOC_LITERAL(28, 340, 11), // "addLikeSong"
QT_MOC_LITERAL(29, 352, 11), // "delLikeSong"
QT_MOC_LITERAL(30, 364, 13), // "setLikeButton"
QT_MOC_LITERAL(31, 378, 17), // "changeToLikeSongs"
QT_MOC_LITERAL(32, 396, 15), // "pressLikeButton"
QT_MOC_LITERAL(33, 412, 17), // "releaseLikeButton"
QT_MOC_LITERAL(34, 430, 8), // "swapMode"
QT_MOC_LITERAL(35, 439, 17), // "clickSearchButton"
QT_MOC_LITERAL(36, 457, 11), // "fuzzySearch"
QT_MOC_LITERAL(37, 469, 4), // "text"
QT_MOC_LITERAL(38, 474, 8), // "showWord"
QT_MOC_LITERAL(39, 483, 13), // "initWordTable"
QT_MOC_LITERAL(40, 497, 14), // "resetWordTable"
QT_MOC_LITERAL(41, 512, 10), // "chooseWord"
QT_MOC_LITERAL(42, 523, 8), // "changeWT"
QT_MOC_LITERAL(43, 532, 18), // "changeToWordDetail"
QT_MOC_LITERAL(44, 551, 13), // "addToWordBook"
QT_MOC_LITERAL(45, 565, 15), // "delFromWordBook"
QT_MOC_LITERAL(46, 581, 19), // "setWordButtonStatus"
QT_MOC_LITERAL(47, 601, 16), // "clickLWordButton"
QT_MOC_LITERAL(48, 618, 25), // "on_buttonNextWord_clicked"
QT_MOC_LITERAL(49, 644, 15), // "pressWordButton"
QT_MOC_LITERAL(50, 660, 17), // "releaseWordButton"
QT_MOC_LITERAL(51, 678, 16), // "changeToWordBook"
QT_MOC_LITERAL(52, 695, 26), // "on_newWindowWidget_clicked"
QT_MOC_LITERAL(53, 722, 25), // "on_buttonLastWord_clicked"
QT_MOC_LITERAL(54, 748, 17), // "insertHistoryToDB"
QT_MOC_LITERAL(55, 766, 18), // "calculatePauseTime"
QT_MOC_LITERAL(56, 785, 23) // "setLastSongDurationTime"

    },
    "MainWindow\0init\0\0playTo\0addItem\0name\0"
    "getFileNames\0path\0SlotShowCurrentMedia\0"
    "SlotButtonStart\0SlotButtonPrevious\0"
    "SlotButtonNext\0initMusicTable\0"
    "mousePressEvent\0QMouseEvent*\0event\0"
    "mouseMoveEvent\0removeRow\0change_bar_position\0"
    "initVolumeSlider\0setVolume\0"
    "initProgressSlider\0progressPressed\0"
    "progressMoved\0progressReleased\0openFiles\0"
    "setMode\0setLike\0addLikeSong\0delLikeSong\0"
    "setLikeButton\0changeToLikeSongs\0"
    "pressLikeButton\0releaseLikeButton\0"
    "swapMode\0clickSearchButton\0fuzzySearch\0"
    "text\0showWord\0initWordTable\0resetWordTable\0"
    "chooseWord\0changeWT\0changeToWordDetail\0"
    "addToWordBook\0delFromWordBook\0"
    "setWordButtonStatus\0clickLWordButton\0"
    "on_buttonNextWord_clicked\0pressWordButton\0"
    "releaseWordButton\0changeToWordBook\0"
    "on_newWindowWidget_clicked\0"
    "on_buttonLastWord_clicked\0insertHistoryToDB\0"
    "calculatePauseTime\0setLastSongDurationTime"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      50,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  264,    2, 0x08 /* Private */,
       3,    2,  265,    2, 0x08 /* Private */,
       4,    1,  270,    2, 0x08 /* Private */,
       6,    1,  273,    2, 0x08 /* Private */,
       8,    0,  276,    2, 0x08 /* Private */,
       9,    0,  277,    2, 0x08 /* Private */,
      10,    0,  278,    2, 0x08 /* Private */,
      11,    0,  279,    2, 0x08 /* Private */,
      12,    0,  280,    2, 0x08 /* Private */,
      13,    1,  281,    2, 0x08 /* Private */,
      16,    1,  284,    2, 0x08 /* Private */,
      17,    1,  287,    2, 0x08 /* Private */,
      18,    0,  290,    2, 0x08 /* Private */,
      19,    0,  291,    2, 0x08 /* Private */,
      20,    1,  292,    2, 0x08 /* Private */,
      21,    0,  295,    2, 0x08 /* Private */,
      22,    0,  296,    2, 0x08 /* Private */,
      23,    1,  297,    2, 0x08 /* Private */,
      24,    0,  300,    2, 0x08 /* Private */,
      25,    0,  301,    2, 0x08 /* Private */,
      26,    0,  302,    2, 0x08 /* Private */,
      27,    0,  303,    2, 0x08 /* Private */,
      28,    0,  304,    2, 0x08 /* Private */,
      29,    0,  305,    2, 0x08 /* Private */,
      30,    0,  306,    2, 0x08 /* Private */,
      31,    0,  307,    2, 0x08 /* Private */,
      32,    0,  308,    2, 0x08 /* Private */,
      33,    0,  309,    2, 0x08 /* Private */,
      34,    0,  310,    2, 0x08 /* Private */,
      35,    0,  311,    2, 0x08 /* Private */,
      36,    1,  312,    2, 0x08 /* Private */,
      38,    1,  315,    2, 0x08 /* Private */,
      39,    0,  318,    2, 0x08 /* Private */,
      40,    1,  319,    2, 0x08 /* Private */,
      41,    1,  322,    2, 0x08 /* Private */,
      42,    0,  325,    2, 0x08 /* Private */,
      43,    2,  326,    2, 0x08 /* Private */,
      44,    0,  331,    2, 0x08 /* Private */,
      45,    0,  332,    2, 0x08 /* Private */,
      46,    0,  333,    2, 0x08 /* Private */,
      47,    0,  334,    2, 0x08 /* Private */,
      48,    0,  335,    2, 0x08 /* Private */,
      49,    0,  336,    2, 0x08 /* Private */,
      50,    0,  337,    2, 0x08 /* Private */,
      51,    0,  338,    2, 0x08 /* Private */,
      52,    0,  339,    2, 0x08 /* Private */,
      53,    0,  340,    2, 0x08 /* Private */,
      54,    0,  341,    2, 0x08 /* Private */,
      55,    0,  342,    2, 0x08 /* Private */,
      56,    1,  343,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::QStringList, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   37,
    QMetaType::Void, QMetaType::QString,   37,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QStringList,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,    2,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->init(); break;
        case 1: _t->playTo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->addItem((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: { QStringList _r = _t->getFileNames((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 4: _t->SlotShowCurrentMedia(); break;
        case 5: _t->SlotButtonStart(); break;
        case 6: _t->SlotButtonPrevious(); break;
        case 7: _t->SlotButtonNext(); break;
        case 8: _t->initMusicTable(); break;
        case 9: _t->mousePressEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 10: _t->mouseMoveEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 11: _t->removeRow((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->change_bar_position(); break;
        case 13: _t->initVolumeSlider(); break;
        case 14: _t->setVolume((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->initProgressSlider(); break;
        case 16: _t->progressPressed(); break;
        case 17: _t->progressMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->progressReleased(); break;
        case 19: _t->openFiles(); break;
        case 20: _t->setMode(); break;
        case 21: _t->setLike(); break;
        case 22: _t->addLikeSong(); break;
        case 23: _t->delLikeSong(); break;
        case 24: _t->setLikeButton(); break;
        case 25: _t->changeToLikeSongs(); break;
        case 26: _t->pressLikeButton(); break;
        case 27: _t->releaseLikeButton(); break;
        case 28: _t->swapMode(); break;
        case 29: _t->clickSearchButton(); break;
        case 30: _t->fuzzySearch((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 31: _t->showWord((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 32: _t->initWordTable(); break;
        case 33: _t->resetWordTable((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 34: _t->chooseWord((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 35: _t->changeWT(); break;
        case 36: _t->changeToWordDetail((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 37: _t->addToWordBook(); break;
        case 38: _t->delFromWordBook(); break;
        case 39: _t->setWordButtonStatus(); break;
        case 40: _t->clickLWordButton(); break;
        case 41: _t->on_buttonNextWord_clicked(); break;
        case 42: _t->pressWordButton(); break;
        case 43: _t->releaseWordButton(); break;
        case 44: _t->changeToWordBook(); break;
        case 45: _t->on_newWindowWidget_clicked(); break;
        case 46: _t->on_buttonLastWord_clicked(); break;
        case 47: _t->insertHistoryToDB(); break;
        case 48: _t->calculatePauseTime(); break;
        case 49: _t->setLastSongDurationTime((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 50)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 50;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 50)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 50;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
