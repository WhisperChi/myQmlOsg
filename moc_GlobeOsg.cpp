/****************************************************************************
** Meta object code from reading C++ file 'GlobeOsg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "GlobeOsg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GlobeOsg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GlobeOsg_t {
    QByteArrayData data[9];
    char stringdata0[65];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GlobeOsg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GlobeOsg_t qt_meta_stringdata_GlobeOsg = {
    {
QT_MOC_LITERAL(0, 0, 8), // "GlobeOsg"
QT_MOC_LITERAL(1, 9, 4), // "home"
QT_MOC_LITERAL(2, 14, 0), // ""
QT_MOC_LITERAL(3, 15, 16), // "_mousePressEvent"
QT_MOC_LITERAL(4, 32, 1), // "x"
QT_MOC_LITERAL(5, 34, 1), // "y"
QT_MOC_LITERAL(6, 36, 15), // "_mouseDragEvent"
QT_MOC_LITERAL(7, 52, 4), // "zoom"
QT_MOC_LITERAL(8, 57, 7) // "animate"

    },
    "GlobeOsg\0home\0\0_mousePressEvent\0x\0y\0"
    "_mouseDragEvent\0zoom\0animate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GlobeOsg[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       2,   40, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x02 /* Public */,
       3,    2,   30,    2, 0x01 /* Protected */,
       6,    2,   35,    2, 0x01 /* Protected */,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    4,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    4,    5,

 // properties: name, type, flags
       7, QMetaType::QReal, 0x00095103,
       8, QMetaType::Bool, 0x00095103,

       0        // eod
};

void GlobeOsg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GlobeOsg *_t = static_cast<GlobeOsg *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->home(); break;
        case 1: _t->_mousePressEvent((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->_mouseDragEvent((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        GlobeOsg *_t = static_cast<GlobeOsg *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< qreal*>(_v) = _t->getZoom(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->getAnimate(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        GlobeOsg *_t = static_cast<GlobeOsg *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setZoom(*reinterpret_cast< qreal*>(_v)); break;
        case 1: _t->setAnimate(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject GlobeOsg::staticMetaObject = {
    { &QQuickItem::staticMetaObject, qt_meta_stringdata_GlobeOsg.data,
      qt_meta_data_GlobeOsg,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *GlobeOsg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GlobeOsg::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_GlobeOsg.stringdata0))
        return static_cast<void*>(const_cast< GlobeOsg*>(this));
    return QQuickItem::qt_metacast(_clname);
}

int GlobeOsg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQuickItem::qt_metacall(_c, _id, _a);
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
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
