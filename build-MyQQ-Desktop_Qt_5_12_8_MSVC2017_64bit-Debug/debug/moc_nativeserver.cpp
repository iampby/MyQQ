/****************************************************************************
** Meta object code from reading C++ file 'nativeserver.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../nativeserver.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'nativeserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NativeServer_t {
    QByteArrayData data[17];
    char stringdata0[157];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NativeServer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NativeServer_t qt_meta_stringdata_NativeServer = {
    {
QT_MOC_LITERAL(0, 0, 12), // "NativeServer"
QT_MOC_LITERAL(1, 13, 8), // "emitExit"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 8), // "findPort"
QT_MOC_LITERAL(4, 32, 4), // "port"
QT_MOC_LITERAL(5, 37, 11), // "emitFverify"
QT_MOC_LITERAL(6, 49, 4), // "data"
QT_MOC_LITERAL(7, 54, 13), // "emitGetFriend"
QT_MOC_LITERAL(8, 68, 3), // "pix"
QT_MOC_LITERAL(9, 72, 11), // "emitOffline"
QT_MOC_LITERAL(10, 84, 2), // "ip"
QT_MOC_LITERAL(11, 87, 4), // "host"
QT_MOC_LITERAL(12, 92, 8), // "datetime"
QT_MOC_LITERAL(13, 101, 11), // "slotStarted"
QT_MOC_LITERAL(14, 113, 18), // "incomingConnection"
QT_MOC_LITERAL(15, 132, 7), // "qintptr"
QT_MOC_LITERAL(16, 140, 16) // "socketDescriptor"

    },
    "NativeServer\0emitExit\0\0findPort\0port\0"
    "emitFverify\0data\0emitGetFriend\0pix\0"
    "emitOffline\0ip\0host\0datetime\0slotStarted\0"
    "incomingConnection\0qintptr\0socketDescriptor"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NativeServer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    1,   50,    2, 0x06 /* Public */,
       5,    1,   53,    2, 0x06 /* Public */,
       7,    2,   56,    2, 0x06 /* Public */,
       9,    3,   61,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    0,   68,    2, 0x0a /* Public */,
      14,    1,   69,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::UShort,    4,
    QMetaType::Void, QMetaType::QByteArray,    6,
    QMetaType::Void, QMetaType::QByteArray, QMetaType::QPixmap,    6,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   10,   11,   12,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 15,   16,

       0        // eod
};

void NativeServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NativeServer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->emitExit(); break;
        case 1: _t->findPort((*reinterpret_cast< quint16(*)>(_a[1]))); break;
        case 2: _t->emitFverify((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 3: _t->emitGetFriend((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< QPixmap(*)>(_a[2]))); break;
        case 4: _t->emitOffline((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 5: _t->slotStarted(); break;
        case 6: _t->incomingConnection((*reinterpret_cast< qintptr(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NativeServer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NativeServer::emitExit)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NativeServer::*)(quint16 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NativeServer::findPort)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NativeServer::*)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NativeServer::emitFverify)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (NativeServer::*)(QByteArray , QPixmap );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NativeServer::emitGetFriend)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (NativeServer::*)(QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NativeServer::emitOffline)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NativeServer::staticMetaObject = { {
    &QTcpServer::staticMetaObject,
    qt_meta_stringdata_NativeServer.data,
    qt_meta_data_NativeServer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NativeServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NativeServer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NativeServer.stringdata0))
        return static_cast<void*>(this);
    return QTcpServer::qt_metacast(_clname);
}

int NativeServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpServer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void NativeServer::emitExit()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void NativeServer::findPort(quint16 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void NativeServer::emitFverify(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void NativeServer::emitGetFriend(QByteArray _t1, QPixmap _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void NativeServer::emitOffline(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
