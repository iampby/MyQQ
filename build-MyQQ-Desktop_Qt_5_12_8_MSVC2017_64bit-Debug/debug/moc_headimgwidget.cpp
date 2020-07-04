/****************************************************************************
** Meta object code from reading C++ file 'headimgwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../headimgwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'headimgwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HeadImgWidget_t {
    QByteArrayData data[9];
    char stringdata0[87];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HeadImgWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HeadImgWidget_t qt_meta_stringdata_HeadImgWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "HeadImgWidget"
QT_MOC_LITERAL(1, 14, 8), // "getFocus"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 19), // "updateRemoteHeadImg"
QT_MOC_LITERAL(4, 44, 8), // "openFile"
QT_MOC_LITERAL(5, 53, 8), // "filename"
QT_MOC_LITERAL(6, 62, 9), // "okClicked"
QT_MOC_LITERAL(7, 72, 7), // "Images*"
QT_MOC_LITERAL(8, 80, 6) // "images"

    },
    "HeadImgWidget\0getFocus\0\0updateRemoteHeadImg\0"
    "openFile\0filename\0okClicked\0Images*\0"
    "images"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HeadImgWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    1,   35,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   38,    2, 0x0a /* Public */,
       6,    1,   41,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPixmap,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, 0x80000000 | 7,    8,

       0        // eod
};

void HeadImgWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HeadImgWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->getFocus(); break;
        case 1: _t->updateRemoteHeadImg((*reinterpret_cast< const QPixmap(*)>(_a[1]))); break;
        case 2: _t->openFile((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->okClicked((*reinterpret_cast< Images*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (HeadImgWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HeadImgWidget::getFocus)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (HeadImgWidget::*)(const QPixmap & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HeadImgWidget::updateRemoteHeadImg)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject HeadImgWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_HeadImgWidget.data,
    qt_meta_data_HeadImgWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *HeadImgWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HeadImgWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HeadImgWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int HeadImgWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void HeadImgWidget::getFocus()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void HeadImgWidget::updateRemoteHeadImg(const QPixmap & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
