/****************************************************************************
** Meta object code from reading C++ file 'userview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../userview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'userview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UserView_t {
    QByteArrayData data[21];
    char stringdata0[231];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UserView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UserView_t qt_meta_stringdata_UserView = {
    {
QT_MOC_LITERAL(0, 0, 8), // "UserView"
QT_MOC_LITERAL(1, 9, 10), // "imgClicked"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 11), // "nameClicked"
QT_MOC_LITERAL(4, 33, 16), // "addButtonClicked"
QT_MOC_LITERAL(5, 50, 11), // "QModelIndex"
QT_MOC_LITERAL(6, 62, 15), // "continueGetList"
QT_MOC_LITERAL(7, 78, 5), // "reset"
QT_MOC_LITERAL(8, 84, 8), // "inserted"
QT_MOC_LITERAL(9, 93, 3), // "row"
QT_MOC_LITERAL(10, 97, 6), // "column"
QT_MOC_LITERAL(11, 104, 5), // "count"
QT_MOC_LITERAL(12, 110, 11), // "dataChanged"
QT_MOC_LITERAL(13, 122, 7), // "topLeft"
QT_MOC_LITERAL(14, 130, 11), // "bottomRight"
QT_MOC_LITERAL(15, 142, 12), // "QVector<int>"
QT_MOC_LITERAL(16, 155, 5), // "roles"
QT_MOC_LITERAL(17, 161, 17), // "lastButtonClicked"
QT_MOC_LITERAL(18, 179, 17), // "nextButtonClicked"
QT_MOC_LITERAL(19, 197, 16), // "oneButtonClicked"
QT_MOC_LITERAL(20, 214, 16) // "twoButtonClicked"

    },
    "UserView\0imgClicked\0\0nameClicked\0"
    "addButtonClicked\0QModelIndex\0"
    "continueGetList\0reset\0inserted\0row\0"
    "column\0count\0dataChanged\0topLeft\0"
    "bottomRight\0QVector<int>\0roles\0"
    "lastButtonClicked\0nextButtonClicked\0"
    "oneButtonClicked\0twoButtonClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UserView[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       3,    1,   77,    2, 0x06 /* Public */,
       4,    1,   80,    2, 0x06 /* Public */,
       6,    0,   83,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   84,    2, 0x0a /* Public */,
       8,    3,   85,    2, 0x0a /* Public */,
      12,    3,   92,    2, 0x0a /* Public */,
      12,    2,   99,    2, 0x2a /* Public | MethodCloned */,
      17,    0,  104,    2, 0x0a /* Public */,
      18,    0,  105,    2, 0x0a /* Public */,
      19,    0,  106,    2, 0x0a /* Public */,
      20,    0,  107,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    9,   10,   11,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 5, 0x80000000 | 15,   13,   14,   16,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 5,   13,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void UserView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UserView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->imgClicked((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->nameClicked((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->addButtonClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 3: _t->continueGetList(); break;
        case 4: _t->reset(); break;
        case 5: _t->inserted((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 6: _t->dataChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2])),(*reinterpret_cast< const QVector<int>(*)>(_a[3]))); break;
        case 7: _t->dataChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 8: _t->lastButtonClicked(); break;
        case 9: _t->nextButtonClicked(); break;
        case 10: _t->oneButtonClicked(); break;
        case 11: _t->twoButtonClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<int> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (UserView::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserView::imgClicked)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (UserView::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserView::nameClicked)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (UserView::*)(const QModelIndex & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserView::addButtonClicked)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (UserView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserView::continueGetList)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject UserView::staticMetaObject = { {
    &QScrollArea::staticMetaObject,
    qt_meta_stringdata_UserView.data,
    qt_meta_data_UserView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *UserView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UserView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UserView.stringdata0))
        return static_cast<void*>(this);
    return QScrollArea::qt_metacast(_clname);
}

int UserView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QScrollArea::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void UserView::imgClicked(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void UserView::nameClicked(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void UserView::addButtonClicked(const QModelIndex & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void UserView::continueGetList()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
