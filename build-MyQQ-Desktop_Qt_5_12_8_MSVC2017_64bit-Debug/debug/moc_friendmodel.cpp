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
struct qt_meta_stringdata_FriendData_t {
    QByteArrayData data[3];
    char stringdata0[17];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FriendData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FriendData_t qt_meta_stringdata_FriendData = {
    {
QT_MOC_LITERAL(0, 0, 10), // "FriendData"
QT_MOC_LITERAL(1, 11, 4), // "myqq"
QT_MOC_LITERAL(2, 16, 0) // ""

    },
    "FriendData\0myqq\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FriendData[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::QString,

       0        // eod
};

void FriendData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FriendData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { QString _r = _t->myqq();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FriendData::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_FriendData.data,
    qt_meta_data_FriendData,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FriendData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FriendData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FriendData.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int FriendData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_FriendModel_t {
    QByteArrayData data[33];
    char stringdata0[229];
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
QT_MOC_LITERAL(5, 41, 6), // "update"
QT_MOC_LITERAL(6, 48, 5), // "index"
QT_MOC_LITERAL(7, 54, 6), // "index1"
QT_MOC_LITERAL(8, 61, 6), // "index2"
QT_MOC_LITERAL(9, 68, 4), // "data"
QT_MOC_LITERAL(10, 73, 4), // "role"
QT_MOC_LITERAL(11, 78, 5), // "rowOf"
QT_MOC_LITERAL(12, 84, 3), // "var"
QT_MOC_LITERAL(13, 88, 4), // "sort"
QT_MOC_LITERAL(14, 93, 8), // "deletion"
QT_MOC_LITERAL(15, 102, 8), // "takeItem"
QT_MOC_LITERAL(16, 111, 11), // "FriendData*"
QT_MOC_LITERAL(17, 123, 6), // "remove"
QT_MOC_LITERAL(18, 130, 3), // "row"
QT_MOC_LITERAL(19, 134, 5), // "count"
QT_MOC_LITERAL(20, 140, 7), // "setData"
QT_MOC_LITERAL(21, 148, 5), // "value"
QT_MOC_LITERAL(22, 154, 6), // "append"
QT_MOC_LITERAL(23, 161, 4), // "item"
QT_MOC_LITERAL(24, 166, 4), // "myqq"
QT_MOC_LITERAL(25, 171, 4), // "name"
QT_MOC_LITERAL(26, 176, 9), // "signature"
QT_MOC_LITERAL(27, 186, 7), // "imgPath"
QT_MOC_LITERAL(28, 194, 3), // "tag"
QT_MOC_LITERAL(29, 198, 5), // "grade"
QT_MOC_LITERAL(30, 204, 6), // "status"
QT_MOC_LITERAL(31, 211, 7), // "infoSet"
QT_MOC_LITERAL(32, 219, 9) // "statusSet"

    },
    "FriendModel\0rowCount\0\0QModelIndex\0"
    "parent\0update\0index\0index1\0index2\0"
    "data\0role\0rowOf\0var\0sort\0deletion\0"
    "takeItem\0FriendData*\0remove\0row\0count\0"
    "setData\0value\0append\0item\0myqq\0name\0"
    "signature\0imgPath\0tag\0grade\0status\0"
    "infoSet\0statusSet"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FriendModel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    1,   99,    2, 0x02 /* Public */,
       1,    0,  102,    2, 0x22 /* Public | MethodCloned */,
       5,    1,  103,    2, 0x02 /* Public */,
       5,    2,  106,    2, 0x02 /* Public */,
       9,    2,  111,    2, 0x02 /* Public */,
       9,    1,  116,    2, 0x22 /* Public | MethodCloned */,
      11,    2,  119,    2, 0x02 /* Public */,
      11,    1,  124,    2, 0x22 /* Public | MethodCloned */,
      13,    0,  127,    2, 0x02 /* Public */,
      14,    0,  128,    2, 0x02 /* Public */,
      15,    1,  129,    2, 0x02 /* Public */,
      17,    2,  132,    2, 0x02 /* Public */,
      17,    1,  137,    2, 0x22 /* Public | MethodCloned */,
      20,    3,  140,    2, 0x02 /* Public */,
      20,    2,  147,    2, 0x22 /* Public | MethodCloned */,
      22,    1,  152,    2, 0x02 /* Public */,
      22,    9,  155,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::Int, 0x80000000 | 3,    4,
    QMetaType::Int,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    7,    8,
    QMetaType::QString, QMetaType::Int, QMetaType::Int,    2,   10,
    QMetaType::QString, QMetaType::Int,    2,
    QMetaType::Int, QMetaType::QVariant, QMetaType::Int,   12,   10,
    QMetaType::Int, QMetaType::QVariant,   12,
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 16, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   18,   19,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::Int,   18,   21,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   18,   21,
    QMetaType::Void, 0x80000000 | 16,   23,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   24,   25,   26,   27,   28,   29,   30,   31,   32,

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
        case 2: _t->update((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->update((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: { QString _r = _t->data((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 5: { QString _r = _t->data((*reinterpret_cast< const int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 6: { int _r = _t->rowOf((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 7: { int _r = _t->rowOf((*reinterpret_cast< const QVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->sort(); break;
        case 9: _t->deletion(); break;
        case 10: { FriendData* _r = _t->takeItem((*reinterpret_cast< const int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< FriendData**>(_a[0]) = std::move(_r); }  break;
        case 11: _t->remove((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 12: _t->remove((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 13: _t->setData((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 14: _t->setData((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 15: _t->append((*reinterpret_cast< FriendData*(*)>(_a[1]))); break;
        case 16: _t->append((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< const QString(*)>(_a[6])),(*reinterpret_cast< const QString(*)>(_a[7])),(*reinterpret_cast< const QString(*)>(_a[8])),(*reinterpret_cast< const QString(*)>(_a[9]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 15:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< FriendData* >(); break;
            }
            break;
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
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
