/****************************************************************************
** Meta object code from reading C++ file 'registersocket.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../registersocket.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'registersocket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RegisterSocket_t {
    QByteArrayData data[8];
    char stringdata0[82];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RegisterSocket_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RegisterSocket_t qt_meta_stringdata_RegisterSocket = {
    {
QT_MOC_LITERAL(0, 0, 14), // "RegisterSocket"
QT_MOC_LITERAL(1, 15, 8), // "finished"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 10), // "errOccured"
QT_MOC_LITERAL(4, 36, 5), // "readD"
QT_MOC_LITERAL(5, 42, 6), // "writeD"
QT_MOC_LITERAL(6, 49, 3), // "err"
QT_MOC_LITERAL(7, 53, 28) // "QAbstractSocket::SocketError"

    },
    "RegisterSocket\0finished\0\0errOccured\0"
    "readD\0writeD\0err\0QAbstractSocket::SocketError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RegisterSocket[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    0,   40,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   41,    2, 0x08 /* Private */,
       5,    0,   42,    2, 0x08 /* Private */,
       6,    1,   43,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    2,

       0        // eod
};

void RegisterSocket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RegisterSocket *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->errOccured(); break;
        case 2: _t->readD(); break;
        case 3: _t->writeD(); break;
        case 4: _t->err((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RegisterSocket::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RegisterSocket::finished)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RegisterSocket::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RegisterSocket::errOccured)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject RegisterSocket::staticMetaObject = { {
    &QTcpSocket::staticMetaObject,
    qt_meta_stringdata_RegisterSocket.data,
    qt_meta_data_RegisterSocket,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RegisterSocket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RegisterSocket::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RegisterSocket.stringdata0))
        return static_cast<void*>(this);
    return QTcpSocket::qt_metacast(_clname);
}

int RegisterSocket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void RegisterSocket::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void RegisterSocket::errOccured()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
