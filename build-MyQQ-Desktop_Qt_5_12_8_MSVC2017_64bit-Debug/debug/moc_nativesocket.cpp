/****************************************************************************
** Meta object code from reading C++ file 'nativesocket.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../nativesocket.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'nativesocket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NativeSocket_t {
    QByteArrayData data[12];
    char stringdata0[93];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NativeSocket_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NativeSocket_t qt_meta_stringdata_NativeSocket = {
    {
QT_MOC_LITERAL(0, 0, 12), // "NativeSocket"
QT_MOC_LITERAL(1, 13, 8), // "finished"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 11), // "emitFVerify"
QT_MOC_LITERAL(4, 35, 4), // "data"
QT_MOC_LITERAL(5, 40, 13), // "emitGetFriend"
QT_MOC_LITERAL(6, 54, 3), // "pix"
QT_MOC_LITERAL(7, 58, 11), // "emitOffline"
QT_MOC_LITERAL(8, 70, 2), // "ip"
QT_MOC_LITERAL(9, 73, 4), // "host"
QT_MOC_LITERAL(10, 78, 8), // "datetime"
QT_MOC_LITERAL(11, 87, 5) // "readD"

    },
    "NativeSocket\0finished\0\0emitFVerify\0"
    "data\0emitGetFriend\0pix\0emitOffline\0"
    "ip\0host\0datetime\0readD"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NativeSocket[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    1,   40,    2, 0x06 /* Public */,
       5,    2,   43,    2, 0x06 /* Public */,
       7,    3,   48,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,   55,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    4,
    QMetaType::Void, QMetaType::QByteArray, QMetaType::QPixmap,    4,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    8,    9,   10,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void NativeSocket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NativeSocket *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->emitFVerify((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 2: _t->emitGetFriend((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< QPixmap(*)>(_a[2]))); break;
        case 3: _t->emitOffline((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 4: _t->readD(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NativeSocket::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NativeSocket::finished)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NativeSocket::*)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NativeSocket::emitFVerify)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NativeSocket::*)(QByteArray , QPixmap );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NativeSocket::emitGetFriend)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (NativeSocket::*)(QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NativeSocket::emitOffline)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NativeSocket::staticMetaObject = { {
    &QTcpSocket::staticMetaObject,
    qt_meta_stringdata_NativeSocket.data,
    qt_meta_data_NativeSocket,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NativeSocket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NativeSocket::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NativeSocket.stringdata0))
        return static_cast<void*>(this);
    return QTcpSocket::qt_metacast(_clname);
}

int NativeSocket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpSocket::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void NativeSocket::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void NativeSocket::emitFVerify(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void NativeSocket::emitGetFriend(QByteArray _t1, QPixmap _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void NativeSocket::emitOffline(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
