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
    QByteArrayData data[16];
    char stringdata0[144];
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
QT_MOC_LITERAL(4, 27, 6), // "folder"
QT_MOC_LITERAL(5, 34, 4), // "type"
QT_MOC_LITERAL(6, 39, 5), // "start"
QT_MOC_LITERAL(7, 45, 8), // "finished"
QT_MOC_LITERAL(8, 54, 8), // "loopStop"
QT_MOC_LITERAL(9, 63, 18), // "writtenInstruction"
QT_MOC_LITERAL(10, 82, 5), // "readD"
QT_MOC_LITERAL(11, 88, 6), // "writeD"
QT_MOC_LITERAL(12, 95, 3), // "err"
QT_MOC_LITERAL(13, 99, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(14, 128, 4), // "post"
QT_MOC_LITERAL(15, 133, 10) // "resultSlot"

    },
    "BigFileSocket\0result\0\0code\0folder\0"
    "type\0start\0finished\0loopStop\0"
    "writtenInstruction\0readD\0writeD\0err\0"
    "QAbstractSocket::SocketError\0post\0"
    "resultSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BigFileSocket[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   79,    2, 0x06 /* Public */,
       1,    2,   86,    2, 0x26 /* Public | MethodCloned */,
       1,    1,   91,    2, 0x26 /* Public | MethodCloned */,
       6,    0,   94,    2, 0x06 /* Public */,
       7,    1,   95,    2, 0x06 /* Public */,
       7,    0,   98,    2, 0x26 /* Public | MethodCloned */,
       8,    0,   99,    2, 0x06 /* Public */,
       9,    0,  100,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,  101,    2, 0x0a /* Public */,
      11,    0,  102,    2, 0x0a /* Public */,
      12,    1,  103,    2, 0x0a /* Public */,
      14,    0,  106,    2, 0x0a /* Public */,
      15,    3,  107,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QString,    3,    4,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    3,    4,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QString,    3,    4,    5,

       0        // eod
};

void BigFileSocket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BigFileSocket *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->result((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 1: _t->result((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->result((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->start(); break;
        case 4: _t->finished((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->finished(); break;
        case 6: _t->loopStop(); break;
        case 7: _t->writtenInstruction(); break;
        case 8: _t->readD(); break;
        case 9: _t->writeD(); break;
        case 10: _t->err((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 11: _t->post(); break;
        case 12: _t->resultSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 10:
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
            using _t = void (BigFileSocket::*)(int , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BigFileSocket::result)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BigFileSocket::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BigFileSocket::start)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (BigFileSocket::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BigFileSocket::finished)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (BigFileSocket::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BigFileSocket::loopStop)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (BigFileSocket::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BigFileSocket::writtenInstruction)) {
                *result = 7;
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
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void BigFileSocket::result(int _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 3
void BigFileSocket::start()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void BigFileSocket::finished(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 6
void BigFileSocket::loopStop()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void BigFileSocket::writtenInstruction()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
