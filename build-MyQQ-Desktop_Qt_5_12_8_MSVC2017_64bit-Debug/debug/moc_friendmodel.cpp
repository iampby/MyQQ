/****************************************************************************
** Meta object code from reading C++ file 'friendmodel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../friendmodel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'friendmodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FriendModel_t {
    QByteArrayData data[24];
    char stringdata0[162];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FriendModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FriendModel_t qt_meta_stringdata_FriendModel = {
    {
QT_MOC_LITERAL(0, 0, 11), // "FriendModel"
QT_MOC_LITERAL(1, 12, 8), // "rowCount"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 11), // "QModelIndex"
QT_MOC_LITERAL(4, 34, 6), // "parent"
QT_MOC_LITERAL(5, 41, 4), // "data"
QT_MOC_LITERAL(6, 46, 5), // "rowOf"
QT_MOC_LITERAL(7, 52, 3), // "var"
QT_MOC_LITERAL(8, 56, 4), // "role"
QT_MOC_LITERAL(9, 61, 6), // "remove"
QT_MOC_LITERAL(10, 68, 3), // "row"
QT_MOC_LITERAL(11, 72, 5), // "count"
QT_MOC_LITERAL(12, 78, 7), // "setData"
QT_MOC_LITERAL(13, 86, 5), // "value"
QT_MOC_LITERAL(14, 92, 6), // "append"
QT_MOC_LITERAL(15, 99, 4), // "myqq"
QT_MOC_LITERAL(16, 104, 4), // "name"
QT_MOC_LITERAL(17, 109, 9), // "signature"
QT_MOC_LITERAL(18, 119, 7), // "imgPath"
QT_MOC_LITERAL(19, 127, 3), // "tag"
QT_MOC_LITERAL(20, 131, 5), // "grade"
QT_MOC_LITERAL(21, 137, 6), // "status"
QT_MOC_LITERAL(22, 144, 7), // "infoSet"
QT_MOC_LITERAL(23, 152, 9) // "statusSet"

    },
    "FriendModel\0rowCount\0\0QModelIndex\0"
    "parent\0data\0rowOf\0var\0role\0remove\0row\0"
    "count\0setData\0value\0append\0myqq\0name\0"
    "signature\0imgPath\0tag\0grade\0status\0"
    "infoSet\0statusSet"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FriendModel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x02 /* Public */,
       1,    0,   67,    2, 0x22 /* Public | MethodCloned */,
       5,    2,   68,    2, 0x02 /* Public */,
       6,    2,   73,    2, 0x02 /* Public */,
       6,    1,   78,    2, 0x22 /* Public | MethodCloned */,
       9,    2,   81,    2, 0x02 /* Public */,
       9,    1,   86,    2, 0x22 /* Public | MethodCloned */,
      12,    3,   89,    2, 0x02 /* Public */,
      12,    2,   96,    2, 0x22 /* Public | MethodCloned */,
      14,    9,  101,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::Int, 0x80000000 | 3,    4,
    QMetaType::Int,
    QMetaType::QString, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Int, QMetaType::QVariant, QMetaType::Int,    7,    8,
    QMetaType::Int, QMetaType::QVariant,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   10,   11,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::Int,   10,   13,    8,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   10,   13,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   15,   16,   17,   18,   19,   20,   21,   22,   23,

       0        // eod
};

void FriendModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FriendModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { int _r = _t->rowCount((*reinterpret_cast< const QModelIndex(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 1: { int _r = _t->rowCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 2: { QString _r = _t->data((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 3: { int _r = _t->rowOf((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 4: { int _r = _t->rowOf((*reinterpret_cast< const QVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 5: _t->remove((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 6: _t->remove((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 7: _t->setData((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 8: _t->setData((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 9: _t->append((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< const QString(*)>(_a[6])),(*reinterpret_cast< const QString(*)>(_a[7])),(*reinterpret_cast< const QString(*)>(_a[8])),(*reinterpret_cast< const QString(*)>(_a[9]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FriendModel::staticMetaObject = { {
    &QAbstractListModel::staticMetaObject,
    qt_meta_stringdata_FriendModel.data,
    qt_meta_data_FriendModel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FriendModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FriendModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FriendModel.stringdata0))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int FriendModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
