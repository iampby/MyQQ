/****************************************************************************
** Meta object code from reading C++ file 'netmonitor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../netmonitor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'netmonitor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NetMonitor_t {
    QByteArrayData data[6];
    char stringdata0[58];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetMonitor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetMonitor_t qt_meta_stringdata_NetMonitor = {
    {
QT_MOC_LITERAL(0, 0, 10), // "NetMonitor"
QT_MOC_LITERAL(1, 11, 12), // "getNetStatus"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 7), // "qint32&"
QT_MOC_LITERAL(4, 33, 11), // "stopMonitor"
QT_MOC_LITERAL(5, 45, 12) // "startMonitor"

    },
    "NetMonitor\0getNetStatus\0\0qint32&\0"
    "stopMonitor\0startMonitor"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetMonitor[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       4,    0,   32,    2, 0x06 /* Public */,
       5,    0,   33,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void NetMonitor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetMonitor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->getNetStatus((*reinterpret_cast< qint32(*)>(_a[1]))); break;
        case 1: _t->stopMonitor(); break;
        case 2: _t->startMonitor(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NetMonitor::*)(qint32 & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetMonitor::getNetStatus)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NetMonitor::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetMonitor::stopMonitor)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NetMonitor::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetMonitor::startMonitor)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NetMonitor::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_NetMonitor.data,
    qt_meta_data_NetMonitor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NetMonitor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetMonitor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NetMonitor.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NetMonitor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void NetMonitor::getNetStatus(qint32 & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NetMonitor::stopMonitor()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void NetMonitor::startMonitor()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
