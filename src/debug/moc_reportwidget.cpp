/****************************************************************************
** Meta object code from reading C++ file 'reportwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../reportwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'reportwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_reportWidget_t {
    QByteArrayData data[21];
    char stringdata0[279];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_reportWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_reportWidget_t qt_meta_stringdata_reportWidget = {
    {
QT_MOC_LITERAL(0, 0, 12), // "reportWidget"
QT_MOC_LITERAL(1, 13, 4), // "init"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 14), // "initBackGround"
QT_MOC_LITERAL(4, 34, 16), // "loadMusicHistory"
QT_MOC_LITERAL(5, 51, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(6, 73, 23), // "on_pushButton_2_clicked"
QT_MOC_LITERAL(7, 97, 15), // "mousePressEvent"
QT_MOC_LITERAL(8, 113, 12), // "QMouseEvent*"
QT_MOC_LITERAL(9, 126, 5), // "event"
QT_MOC_LITERAL(10, 132, 14), // "mouseMoveEvent"
QT_MOC_LITERAL(11, 147, 14), // "getHistoryData"
QT_MOC_LITERAL(12, 162, 18), // "getWeekHistoryData"
QT_MOC_LITERAL(13, 181, 19), // "getMonthHistoryData"
QT_MOC_LITERAL(14, 201, 23), // "on_pushButton_3_clicked"
QT_MOC_LITERAL(15, 225, 9), // "initLabel"
QT_MOC_LITERAL(16, 235, 8), // "loadHtml"
QT_MOC_LITERAL(17, 244, 10), // "findMedian"
QT_MOC_LITERAL(18, 255, 5), // "begin"
QT_MOC_LITERAL(19, 261, 3), // "end"
QT_MOC_LITERAL(20, 265, 13) // "QList<double>"

    },
    "reportWidget\0init\0\0initBackGround\0"
    "loadMusicHistory\0on_pushButton_clicked\0"
    "on_pushButton_2_clicked\0mousePressEvent\0"
    "QMouseEvent*\0event\0mouseMoveEvent\0"
    "getHistoryData\0getWeekHistoryData\0"
    "getMonthHistoryData\0on_pushButton_3_clicked\0"
    "initLabel\0loadHtml\0findMedian\0begin\0"
    "end\0QList<double>"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_reportWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x08 /* Private */,
       3,    0,   85,    2, 0x08 /* Private */,
       4,    0,   86,    2, 0x08 /* Private */,
       5,    0,   87,    2, 0x08 /* Private */,
       6,    0,   88,    2, 0x08 /* Private */,
       7,    1,   89,    2, 0x08 /* Private */,
      10,    1,   92,    2, 0x08 /* Private */,
      11,    0,   95,    2, 0x08 /* Private */,
      12,    0,   96,    2, 0x08 /* Private */,
      13,    0,   97,    2, 0x08 /* Private */,
      14,    0,   98,    2, 0x08 /* Private */,
      15,    0,   99,    2, 0x08 /* Private */,
      16,    0,  100,    2, 0x08 /* Private */,
      17,    3,  101,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Double, QMetaType::Int, QMetaType::Int, 0x80000000 | 20,   18,   19,    2,

       0        // eod
};

void reportWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<reportWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->init(); break;
        case 1: _t->initBackGround(); break;
        case 2: _t->loadMusicHistory(); break;
        case 3: _t->on_pushButton_clicked(); break;
        case 4: _t->on_pushButton_2_clicked(); break;
        case 5: _t->mousePressEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 6: _t->mouseMoveEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 7: _t->getHistoryData(); break;
        case 8: _t->getWeekHistoryData(); break;
        case 9: _t->getMonthHistoryData(); break;
        case 10: _t->on_pushButton_3_clicked(); break;
        case 11: _t->initLabel(); break;
        case 12: _t->loadHtml(); break;
        case 13: { double _r = _t->findMedian((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QList<double>(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<double> >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject reportWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_reportWidget.data,
    qt_meta_data_reportWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *reportWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *reportWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_reportWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int reportWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
