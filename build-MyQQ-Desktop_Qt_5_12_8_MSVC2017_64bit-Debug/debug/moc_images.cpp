/****************************************************************************
** Meta object code from reading C++ file 'images.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../images.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'images.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Images_t {
    QByteArrayData data[22];
    char stringdata0[204];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Images_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Images_t qt_meta_stringdata_Images = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Images"
QT_MOC_LITERAL(1, 7, 17), // "historyImageAdded"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 3), // "url"
QT_MOC_LITERAL(4, 30, 5), // "index"
QT_MOC_LITERAL(5, 36, 18), // "readHistoryHeadImg"
QT_MOC_LITERAL(6, 55, 8), // "fileList"
QT_MOC_LITERAL(7, 64, 7), // "setMyQQ"
QT_MOC_LITERAL(8, 72, 3), // "arg"
QT_MOC_LITERAL(9, 76, 10), // "findPixmap"
QT_MOC_LITERAL(10, 87, 2), // "id"
QT_MOC_LITERAL(11, 90, 9), // "setPixmap"
QT_MOC_LITERAL(12, 100, 3), // "pix"
QT_MOC_LITERAL(13, 104, 11), // "readHistory"
QT_MOC_LITERAL(14, 116, 13), // "removeHistory"
QT_MOC_LITERAL(15, 130, 10), // "setPixmap2"
QT_MOC_LITERAL(16, 141, 13), // "insertPixmap3"
QT_MOC_LITERAL(17, 155, 5), // "count"
QT_MOC_LITERAL(18, 161, 16), // "QVector<QString>"
QT_MOC_LITERAL(19, 178, 5), // "files"
QT_MOC_LITERAL(20, 184, 10), // "setPixmap3"
QT_MOC_LITERAL(21, 195, 8) // "filename"

    },
    "Images\0historyImageAdded\0\0url\0index\0"
    "readHistoryHeadImg\0fileList\0setMyQQ\0"
    "arg\0findPixmap\0id\0setPixmap\0pix\0"
    "readHistory\0removeHistory\0setPixmap2\0"
    "insertPixmap3\0count\0QVector<QString>\0"
    "files\0setPixmap3\0filename"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Images[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   64,    2, 0x06 /* Public */,
       5,    1,   69,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
       7,    1,   72,    2, 0x02 /* Public */,
       9,    1,   75,    2, 0x02 /* Public */,
      11,    2,   78,    2, 0x02 /* Public */,
      13,    0,   83,    2, 0x02 /* Public */,
      14,    0,   84,    2, 0x02 /* Public */,
      15,    2,   85,    2, 0x02 /* Public */,
      16,    2,   90,    2, 0x02 /* Public */,
      20,    2,   95,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::QString,    6,

 // methods: parameters
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::QPixmap, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   10,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   10,   12,
    QMetaType::Int, QMetaType::Int, 0x80000000 | 18,   17,   19,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString,   10,   21,

       0        // eod
};

void Images::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Images *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->historyImageAdded((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< qint32(*)>(_a[2]))); break;
        case 1: _t->readHistoryHeadImg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->setMyQQ((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: { QPixmap _r = _t->findPixmap((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QPixmap*>(_a[0]) = std::move(_r); }  break;
        case 4: _t->setPixmap((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->readHistory(); break;
        case 6: _t->removeHistory(); break;
        case 7: _t->setPixmap2((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 8: { qint32 _r = _t->insertPixmap3((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< QVector<QString>(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< qint32*>(_a[0]) = std::move(_r); }  break;
        case 9: { bool _r = _t->setPixmap3((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QString> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Images::*)(QString , qint32 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Images::historyImageAdded)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Images::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Images::readHistoryHeadImg)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Images::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_Images.data,
    qt_meta_data_Images,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Images::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Images::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Images.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Images::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void Images::historyImageAdded(QString _t1, qint32 _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Images::readHistoryHeadImg(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
