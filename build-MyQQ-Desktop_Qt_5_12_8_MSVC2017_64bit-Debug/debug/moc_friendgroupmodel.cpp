/****************************************************************************
** Meta object code from reading C++ file 'friendgroupmodel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../friendgroupmodel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'friendgroupmodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FriendGroupModel_t {
    QByteArrayData data[16];
    char stringdata0[99];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FriendGroupModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FriendGroupModel_t qt_meta_stringdata_FriendGroupModel = {
    {
QT_MOC_LITERAL(0, 0, 16), // "FriendGroupModel"
QT_MOC_LITERAL(1, 17, 4), // "data"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 1), // "i"
QT_MOC_LITERAL(4, 25, 4), // "role"
QT_MOC_LITERAL(5, 30, 6), // "insert"
QT_MOC_LITERAL(6, 37, 5), // "index"
QT_MOC_LITERAL(7, 43, 4), // "Data"
QT_MOC_LITERAL(8, 48, 6), // "remove"
QT_MOC_LITERAL(9, 55, 7), // "setData"
QT_MOC_LITERAL(10, 63, 5), // "value"
QT_MOC_LITERAL(11, 69, 6), // "append"
QT_MOC_LITERAL(12, 76, 5), // "group"
QT_MOC_LITERAL(13, 82, 6), // "online"
QT_MOC_LITERAL(14, 89, 5), // "count"
QT_MOC_LITERAL(15, 95, 3) // "set"

    },
    "FriendGroupModel\0data\0\0i\0role\0insert\0"
    "index\0Data\0remove\0setData\0value\0append\0"
    "group\0online\0count\0set"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FriendGroupModel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    2,   54,    2, 0x02 /* Public */,
       1,    1,   59,    2, 0x22 /* Public | MethodCloned */,
       5,    2,   62,    2, 0x02 /* Public */,
       8,    1,   67,    2, 0x02 /* Public */,
       9,    3,   70,    2, 0x02 /* Public */,
       9,    2,   77,    2, 0x22 /* Public | MethodCloned */,
      11,    4,   82,    2, 0x02 /* Public */,
      14,    0,   91,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::QVariant, QMetaType::Int, QMetaType::Int,    3,    4,
    QMetaType::QVariant, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 7,    6,    1,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::Int,    3,   10,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    3,   10,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   12,   13,   14,   15,
    QMetaType::Int,

       0        // eod
};

void FriendGroupModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FriendGroupModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { QVariant _r = _t->data((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = std::move(_r); }  break;
        case 1: { QVariant _r = _t->data((*reinterpret_cast< const int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = std::move(_r); }  break;
        case 2: _t->insert((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const Data(*)>(_a[2]))); break;
        case 3: _t->remove((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 4: _t->setData((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 5: _t->setData((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 6: { bool _r = _t->append((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 7: { int _r = _t->count();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FriendGroupModel::staticMetaObject = { {
    &QAbstractListModel::staticMetaObject,
    qt_meta_stringdata_FriendGroupModel.data,
    qt_meta_data_FriendGroupModel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FriendGroupModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FriendGroupModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FriendGroupModel.stringdata0))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int FriendGroupModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
