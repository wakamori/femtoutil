/****************************************************************************
** Meta object code from reading C++ file 'visual.hpp'
**
** Created: Thu Jul 7 01:56:23 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/visual.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'visual.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Connector[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   11,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Connector[] = {
    "Connector\0\0val\0slot_func(qreal)\0"
};

const QMetaObject Connector::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Connector,
      qt_meta_data_Connector, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Connector::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Connector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Connector::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Connector))
        return static_cast<void*>(const_cast< Connector*>(this));
    return QObject::qt_metacast(_clname);
}

int Connector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: slot_func((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_KGraphicsRectItem[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   19,   18,   18, 0x05,
      72,   19,   18,   18, 0x05,
     121,   19,   18,   18, 0x05,
     167,   19,   18,   18, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_KGraphicsRectItem[] = {
    "KGraphicsRectItem\0\0event\0"
    "emitMousePressEvent(QGraphicsSceneMouseEvent*)\0"
    "emitMouseReleaseEvent(QGraphicsSceneMouseEvent*)\0"
    "emitMouseMoveEvent(QGraphicsSceneMouseEvent*)\0"
    "emitDragEnterEvent(QGraphicsSceneDragDropEvent*)\0"
};

const QMetaObject KGraphicsRectItem::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_KGraphicsRectItem,
      qt_meta_data_KGraphicsRectItem, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KGraphicsRectItem::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KGraphicsRectItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KGraphicsRectItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KGraphicsRectItem))
        return static_cast<void*>(const_cast< KGraphicsRectItem*>(this));
    if (!strcmp(_clname, "QGraphicsRectItem"))
        return static_cast< QGraphicsRectItem*>(const_cast< KGraphicsRectItem*>(this));
    return QObject::qt_metacast(_clname);
}

int KGraphicsRectItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: emitMousePressEvent((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        case 1: emitMouseReleaseEvent((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        case 2: emitMouseMoveEvent((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        case 3: emitDragEnterEvent((*reinterpret_cast< QGraphicsSceneDragDropEvent*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void KGraphicsRectItem::emitMousePressEvent(QGraphicsSceneMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void KGraphicsRectItem::emitMouseReleaseEvent(QGraphicsSceneMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void KGraphicsRectItem::emitMouseMoveEvent(QGraphicsSceneMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void KGraphicsRectItem::emitDragEnterEvent(QGraphicsSceneDragDropEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
static const uint qt_meta_data_KRect[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,    7,    6,    6, 0x0a,
      56,    7,    6,    6, 0x0a,
      98,    7,    6,    6, 0x0a,
     143,    7,    6,    6, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_KRect[] = {
    "KRect\0\0event\0mousePressEvent(QGraphicsSceneMouseEvent*)\0"
    "mouseMoveEvent(QGraphicsSceneMouseEvent*)\0"
    "mouseReleaseEvent(QGraphicsSceneMouseEvent*)\0"
    "dragEnterEvent(QGraphicsSceneDragDropEvent*)\0"
};

const QMetaObject KRect::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_KRect,
      qt_meta_data_KRect, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KRect::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KRect::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KRect::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KRect))
        return static_cast<void*>(const_cast< KRect*>(this));
    return QObject::qt_metacast(_clname);
}

int KRect::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: mousePressEvent((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        case 1: mouseMoveEvent((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        case 2: mouseReleaseEvent((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        case 3: dragEnterEvent((*reinterpret_cast< QGraphicsSceneDragDropEvent*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
static const uint qt_meta_data_KEllipse[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_KEllipse[] = {
    "KEllipse\0"
};

const QMetaObject KEllipse::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_KEllipse,
      qt_meta_data_KEllipse, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KEllipse::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KEllipse::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KEllipse::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KEllipse))
        return static_cast<void*>(const_cast< KEllipse*>(this));
    return QObject::qt_metacast(_clname);
}

int KEllipse::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_KTexture[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_KTexture[] = {
    "KTexture\0"
};

const QMetaObject KTexture::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_KTexture,
      qt_meta_data_KTexture, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KTexture::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KTexture::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KTexture::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KTexture))
        return static_cast<void*>(const_cast< KTexture*>(this));
    return QObject::qt_metacast(_clname);
}

int KTexture::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_KText[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_KText[] = {
    "KText\0"
};

const QMetaObject KText::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_KText,
      qt_meta_data_KText, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KText::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KText::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KText::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KText))
        return static_cast<void*>(const_cast< KText*>(this));
    return QObject::qt_metacast(_clname);
}

int KText::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_KGroup[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_KGroup[] = {
    "KGroup\0"
};

const QMetaObject KGroup::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_KGroup,
      qt_meta_data_KGroup, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KGroup::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KGroup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KGroup::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KGroup))
        return static_cast<void*>(const_cast< KGroup*>(this));
    return QObject::qt_metacast(_clname);
}

int KGroup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
