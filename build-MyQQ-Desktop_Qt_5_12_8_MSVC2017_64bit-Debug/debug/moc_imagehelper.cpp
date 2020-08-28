/****************************************************************************
** Meta object code from reading C++ file 'imagehelper.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../imagehelper.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'imagehelper.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Api_t {
    QByteArrayData data[5];
    char stringdata0[25];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Api_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Api_t qt_meta_stringdata_Api = {
    {
QT_MOC_LITERAL(0, 0, 3), // "Api"
QT_MOC_LITERAL(1, 4, 6), // "exists"
QT_MOC_LITERAL(2, 11, 0), // ""
QT_MOC_LITERAL(3, 12, 3), // "arg"
QT_MOC_LITERAL(4, 16, 8) // "baseName"

    },
    "Api\0exists\0\0arg\0baseName"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Api[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x02 /* Public */,
       4,    1,   27,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::Bool, QMetaType::QString,    3,
    QMetaType::QString, QMetaType::QString,    3,

       0        // eod
};

void Api::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Api *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { bool _r = _t->exists((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 1: { QString _r = _t->baseName((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Api::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_Api.data,
    qt_meta_data_Api,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Api::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Api::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Api.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Api::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
struct qt_meta_stringdata_ImageHelper_t {
    QByteArrayData data[24];
    char stringdata0[284];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ImageHelper_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ImageHelper_t qt_meta_stringdata_ImageHelper = {
    {
QT_MOC_LITERAL(0, 0, 11), // "ImageHelper"
QT_MOC_LITERAL(1, 12, 10), // "needUpdate"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 15), // "documentChanged"
QT_MOC_LITERAL(4, 40, 21), // "cursorPositionChanged"
QT_MOC_LITERAL(5, 62, 21), // "selectionStartChanged"
QT_MOC_LITERAL(6, 84, 19), // "selectionEndChanged"
QT_MOC_LITERAL(7, 104, 15), // "maxWidthChanged"
QT_MOC_LITERAL(8, 120, 16), // "maxHeightChanged"
QT_MOC_LITERAL(9, 137, 11), // "insertImage"
QT_MOC_LITERAL(10, 149, 3), // "url"
QT_MOC_LITERAL(11, 153, 3), // "pix"
QT_MOC_LITERAL(12, 157, 6), // "number"
QT_MOC_LITERAL(13, 164, 4), // "myqq"
QT_MOC_LITERAL(14, 169, 10), // "htmlAdjsut"
QT_MOC_LITERAL(15, 180, 4), // "html"
QT_MOC_LITERAL(16, 185, 7), // "cleanup"
QT_MOC_LITERAL(17, 193, 8), // "document"
QT_MOC_LITERAL(18, 202, 19), // "QQuickTextDocument*"
QT_MOC_LITERAL(19, 222, 14), // "cursorPosition"
QT_MOC_LITERAL(20, 237, 14), // "selectionStart"
QT_MOC_LITERAL(21, 252, 12), // "selectionEnd"
QT_MOC_LITERAL(22, 265, 8), // "maxWidth"
QT_MOC_LITERAL(23, 274, 9) // "maxHeight"

    },
    "ImageHelper\0needUpdate\0\0documentChanged\0"
    "cursorPositionChanged\0selectionStartChanged\0"
    "selectionEndChanged\0maxWidthChanged\0"
    "maxHeightChanged\0insertImage\0url\0pix\0"
    "number\0myqq\0htmlAdjsut\0html\0cleanup\0"
    "document\0QQuickTextDocument*\0"
    "cursorPosition\0selectionStart\0"
    "selectionEnd\0maxWidth\0maxHeight"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ImageHelper[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       6,   90, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,
       3,    0,   70,    2, 0x06 /* Public */,
       4,    0,   71,    2, 0x06 /* Public */,
       5,    0,   72,    2, 0x06 /* Public */,
       6,    0,   73,    2, 0x06 /* Public */,
       7,    0,   74,    2, 0x06 /* Public */,
       8,    0,   75,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
       9,    1,   76,    2, 0x02 /* Public */,
       9,    3,   79,    2, 0x02 /* Public */,
      14,    1,   86,    2, 0x02 /* Public */,
      16,    0,   89,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void, QMetaType::QUrl,   10,
    QMetaType::Void, QMetaType::QPixmap, QMetaType::QString, QMetaType::QString,   11,   12,   13,
    QMetaType::QString, QMetaType::QString,   15,
    QMetaType::Void,

 // properties: name, type, flags
      17, 0x80000000 | 18, 0x0049510b,
      19, QMetaType::Int, 0x00495103,
      20, QMetaType::Int, 0x00495103,
      21, QMetaType::Int, 0x00495103,
      22, QMetaType::Int, 0x00495103,
      23, QMetaType::Int, 0x00495103,

 // properties: notify_signal_id
       1,
       2,
       3,
       4,
       5,
       6,

       0        // eod
};

void ImageHelper::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ImageHelper *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->needUpdate(); break;
        case 1: _t->documentChanged(); break;
        case 2: _t->cursorPositionChanged(); break;
        case 3: _t->selectionStartChanged(); break;
        case 4: _t->selectionEndChanged(); break;
        case 5: _t->maxWidthChanged(); break;
        case 6: _t->maxHeightChanged(); break;
        case 7: _t->insertImage((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 8: _t->insertImage((*reinterpret_cast< QPixmap(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 9: { QString _r = _t->htmlAdjsut((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 10: _t->cleanup(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ImageHelper::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageHelper::needUpdate)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ImageHelper::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageHelper::documentChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ImageHelper::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageHelper::cursorPositionChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ImageHelper::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageHelper::selectionStartChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ImageHelper::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageHelper::selectionEndChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (ImageHelper::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageHelper::maxWidthChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (ImageHelper::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageHelper::maxHeightChanged)) {
                *result = 6;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<ImageHelper *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QQuickTextDocument**>(_v) = _t->document(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->cursorPosition(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->selectionStart(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->selectionEnd(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->maxWidth(); break;
        case 5: *reinterpret_cast< int*>(_v) = _t->maxHeight(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<ImageHelper *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setDocument(*reinterpret_cast< QQuickTextDocument**>(_v)); break;
        case 1: _t->setCursorPosition(*reinterpret_cast< int*>(_v)); break;
        case 2: _t->setSelectionStart(*reinterpret_cast< int*>(_v)); break;
        case 3: _t->setSelectionEnd(*reinterpret_cast< int*>(_v)); break;
        case 4: _t->setMaxWidth(*reinterpret_cast< int*>(_v)); break;
        case 5: _t->setMaxHeight(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject ImageHelper::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_ImageHelper.data,
    qt_meta_data_ImageHelper,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ImageHelper::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ImageHelper::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ImageHelper.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ImageHelper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void ImageHelper::needUpdate()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ImageHelper::documentChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ImageHelper::cursorPositionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void ImageHelper::selectionStartChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void ImageHelper::selectionEndChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void ImageHelper::maxWidthChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void ImageHelper::maxHeightChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
