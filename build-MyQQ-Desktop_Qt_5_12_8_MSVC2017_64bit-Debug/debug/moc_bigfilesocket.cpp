/****************************************************************************
** Meta object code from reading C++ file 'bigfilesocket.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../bigfilesocket.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bigfilesocket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BigFileSocket_t {
    QByteArrayData data[12];
    char stringdata0[104];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BigFileSocket_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BigFileSocket_t qt_meta_stringdata_BigFileSocket = {
    {
QT_MOC_LITERAL(0, 0, 13), // "BigFileSocket"
QT_MOC_LITERAL(1, 14, 6), // "result"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 4), // "code"
QT_MOC_LITERAL(4, 27, 5), // "start"
QT_MOC_LITERAL(5, 33, 8), // "finished"
QT_MOC_LITERAL(6, 42, 5), // "readD"
QT_MOC_LITERAL(7, 48, 6), // "writeD"
QT_MOC_LITERAL(8, 55, 3), // "err"
QT_MOC_LITERAL(9, 59, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(10, 88, 4), // "post"
QT_MOC_LITERAL(11, 93, 10) // "resultSlot"

    },
    "BigFileSocket\0result\0\0code\0start\0"
    "finished\0readD\0writeD\0err\0"
    "QAbstractSocket::SocketError\0post\0"
    "resultSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BigFileSocket[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       4,    0,   57,    2, 0x06 /* Public */,
       5,    1,   58,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   61,    2, 0x0a /* Public */,
       7,    0,   62,    2, 0x0a /* Public */,
       8,    1,   63,    2, 0x0a /* Public */,
      10,    0,   66,    2, 0x0a /* Public */,
      11,    1,   67,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

void BigFileSocket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BigFileSocket *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->result((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->start(); break;
        case 2: _t->finished((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->readD(); break;
        case 4: _t->writeD(); break;
        case 5: _t->err((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 6: _t->post(); break;
        case 7: _t->resultSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
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
            using _t = void (BigFileSocket::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BigFileSocket::result)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BigFileSocket::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BigFileSocket::start)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (BigFileSocket::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BigFileSocket::finished)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject BigFileSocket::staticMetaObject = { {
    &QTcpSocket::staticMetaObject,
    qt_meta_stringdata_BigFileSocket.data,
    qt_meta_data_BigFileSocket,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *BigFileSocket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BigFileSocket::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BigFileSocket.stringdata0))
        return static_cast<void*>(this);
    return QTcpSocket::qt_metacast(_clname);
}

int BigFileSocket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpSocket::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void BigFileSocket::result(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BigFileSocket::start()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void BigFileSocket::finished(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
