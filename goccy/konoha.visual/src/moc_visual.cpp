/****************************************************************************
** Meta object code from reading C++ file 'visual.hpp'
**
** Created: Thu Jul 14 16:09:05 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/visual.hpp"
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
static const uint qt_meta_data_KGraphicsLineItem[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   19,   18,   18, 0x05,
      72,   19,   18,   18, 0x05,
     121,   19,   18,   18, 0x05,
     167,   19,   18,   18, 0x05,
     220,   19,   18,   18, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_KGraphicsLineItem[] = {
    "KGraphicsLineItem\0\0event\0"
    "emitMousePressEvent(QGraphicsSceneMouseEvent*)\0"
    "emitMouseReleaseEvent(QGraphicsSceneMouseEvent*)\0"
    "emitMouseMoveEvent(QGraphicsSceneMouseEvent*)\0"
    "emitMouseDoubleClickEvent(QGraphicsSceneMouseEvent*)\0"
    "emitDragEnterEvent(QGraphicsSceneDragDropEvent*)\0"
};

const QMetaObject KGraphicsLineItem::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_KGraphicsLineItem,
      qt_meta_data_KGraphicsLineItem, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KGraphicsLineItem::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KGraphicsLineItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KGraphicsLineItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KGraphicsLineItem))
        return static_cast<void*>(const_cast< KGraphicsLineItem*>(this));
    if (!strcmp(_clname, "QGraphicsLineItem"))
        return static_cast< QGraphicsLineItem*>(const_cast< KGraphicsLineItem*>(this));
    return QObject::qt_metacast(_clname);
}

int KGraphicsLineItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: emitMousePressEvent((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        case 1: emitMouseReleaseEvent((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        case 2: emitMouseMoveEvent((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        case 3: emitMouseDoubleClickEvent((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        case 4: emitDragEnterEvent((*reinterpret_cast< QGraphicsSceneDragDropEvent*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void KGraphicsLineItem::emitMousePressEvent(QGraphicsSceneMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void KGraphicsLineItem::emitMouseReleaseEvent(QGraphicsSceneMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void KGraphicsLineItem::emitMouseMoveEvent(QGraphicsSceneMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void KGraphicsLineItem::emitMouseDoubleClickEvent(QGraphicsSceneMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void KGraphicsLineItem::emitDragEnterEvent(QGraphicsSceneDragDropEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
static const uint qt_meta_data_KGraphicsScene[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      22,   16,   15,   15, 0x05,
      69,   16,   15,   15, 0x05,
     118,   16,   15,   15, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_KGraphicsScene[] = {
    "KGraphicsScene\0\0event\0"
    "emitMousePressEvent(QGraphicsSceneMouseEvent*)\0"
    "emitMouseReleaseEvent(QGraphicsSceneMouseEvent*)\0"
    "emitMouseMoveEvent(QGraphicsSceneMouseEvent*)\0"
};

const QMetaObject KGraphicsScene::staticMetaObject = {
    { &QGraphicsScene::staticMetaObject, qt_meta_stringdata_KGraphicsScene,
      qt_meta_data_KGraphicsScene, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KGraphicsScene::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KGraphicsScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KGraphicsScene::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KGraphicsScene))
        return static_cast<void*>(const_cast< KGraphicsScene*>(this));
    return QGraphicsScene::qt_metacast(_clname);
}

int KGraphicsScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: emitMousePressEvent((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        case 1: emitMouseReleaseEvent((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        case 2: emitMouseMoveEvent((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void KGraphicsScene::emitMousePressEvent(QGraphicsSceneMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void KGraphicsScene::emitMouseReleaseEvent(QGraphicsSceneMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void KGraphicsScene::emitMouseMoveEvent(QGraphicsSceneMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
static const uint qt_meta_data_KGraphicsRectItem[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   19,   18,   18, 0x05,
      72,   19,   18,   18, 0x05,
     121,   19,   18,   18, 0x05,
     167,   19,   18,   18, 0x05,
     220,   19,   18,   18, 0x05,
     269,   19,   18,   18, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_KGraphicsRectItem[] = {
    "KGraphicsRectItem\0\0event\0"
    "emitMousePressEvent(QGraphicsSceneMouseEvent*)\0"
    "emitMouseReleaseEvent(QGraphicsSceneMouseEvent*)\0"
    "emitMouseMoveEvent(QGraphicsSceneMouseEvent*)\0"
    "emitMouseDoubleClickEvent(QGraphicsSceneMouseEvent*)\0"
    "emitDragEnterEvent(QGraphicsSceneDragDropEvent*)\0"
    "emitWheelEvent(QGraphicsSceneWheelEvent*)\0"
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
        case 3: emitMouseDoubleClickEvent((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        case 4: emitDragEnterEvent((*reinterpret_cast< QGraphicsSceneDragDropEvent*(*)>(_a[1]))); break;
        case 5: emitWheelEvent((*reinterpret_cast< QGraphicsSceneWheelEvent*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
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
void KGraphicsRectItem::emitMouseDoubleClickEvent(QGraphicsSceneMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void KGraphicsRectItem::emitDragEnterEvent(QGraphicsSceneDragDropEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void KGraphicsRectItem::emitWheelEvent(QGraphicsSceneWheelEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
static const uint qt_meta_data_KGraphicsPixmapItem[] = {

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
      27,   21,   20,   20, 0x05,
      74,   21,   20,   20, 0x05,
     123,   21,   20,   20, 0x05,
     169,   21,   20,   20, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_KGraphicsPixmapItem[] = {
    "KGraphicsPixmapItem\0\0event\0"
    "emitMousePressEvent(QGraphicsSceneMouseEvent*)\0"
    "emitMouseReleaseEvent(QGraphicsSceneMouseEvent*)\0"
    "emitMouseMoveEvent(QGraphicsSceneMouseEvent*)\0"
    "emitDragEnterEvent(QGraphicsSceneDragDropEvent*)\0"
};

const QMetaObject KGraphicsPixmapItem::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_KGraphicsPixmapItem,
      qt_meta_data_KGraphicsPixmapItem, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KGraphicsPixmapItem::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KGraphicsPixmapItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KGraphicsPixmapItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KGraphicsPixmapItem))
        return static_cast<void*>(const_cast< KGraphicsPixmapItem*>(this));
    if (!strcmp(_clname, "QGraphicsPixmapItem"))
        return static_cast< QGraphicsPixmapItem*>(const_cast< KGraphicsPixmapItem*>(this));
    return QObject::qt_metacast(_clname);
}

int KGraphicsPixmapItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void KGraphicsPixmapItem::emitMousePressEvent(QGraphicsSceneMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void KGraphicsPixmapItem::emitMouseReleaseEvent(QGraphicsSceneMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void KGraphicsPixmapItem::emitMouseMoveEvent(QGraphicsSceneMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void KGraphicsPixmapItem::emitDragEnterEvent(QGraphicsSceneDragDropEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
static const uint qt_meta_data_KScene[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,    8,    7,    7, 0x0a,
      57,    8,    7,    7, 0x0a,
      99,    8,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_KScene[] = {
    "KScene\0\0event\0mousePressEvent(QGraphicsSceneMouseEvent*)\0"
    "mouseMoveEvent(QGraphicsSceneMouseEvent*)\0"
    "mouseReleaseEvent(QGraphicsSceneMouseEvent*)\0"
};

const QMetaObject KScene::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_KScene,
      qt_meta_data_KScene, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KScene::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KScene::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KScene))
        return static_cast<void*>(const_cast< KScene*>(this));
    return QObject::qt_metacast(_clname);
}

int KScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: mousePressEvent((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        case 1: mouseMoveEvent((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        case 2: mouseReleaseEvent((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
static const uint qt_meta_data_KLine[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
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
     192,    7,    6,    6, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_KLine[] = {
    "KLine\0\0event\0mousePressEvent(QGraphicsSceneMouseEvent*)\0"
    "mouseMoveEvent(QGraphicsSceneMouseEvent*)\0"
    "mouseReleaseEvent(QGraphicsSceneMouseEvent*)\0"
    "mouseDoubleClickEvent(QGraphicsSceneMouseEvent*)\0"
    "dragEnterEvent(QGraphicsSceneDragDropEvent*)\0"
};

const QMetaObject KLine::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_KLine,
      qt_meta_data_KLine, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KLine::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KLine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KLine::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KLine))
        return static_cast<void*>(const_cast< KLine*>(this));
    return QObject::qt_metacast(_clname);
}

int KLine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: mousePressEvent((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        case 1: mouseMoveEvent((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        case 2: mouseReleaseEvent((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        case 3: mouseDoubleClickEvent((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        case 4: dragEnterEvent((*reinterpret_cast< QGraphicsSceneDragDropEvent*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
static const uint qt_meta_data_KRect[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
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
     192,    7,    6,    6, 0x0a,
     237,    7,    6,    6, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_KRect[] = {
    "KRect\0\0event\0mousePressEvent(QGraphicsSceneMouseEvent*)\0"
    "mouseMoveEvent(QGraphicsSceneMouseEvent*)\0"
    "mouseReleaseEvent(QGraphicsSceneMouseEvent*)\0"
    "mouseDoubleClickEvent(QGraphicsSceneMouseEvent*)\0"
    "dragEnterEvent(QGraphicsSceneDragDropEvent*)\0"
    "wheelEvent(QGraphicsSceneWheelEvent*)\0"
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
        case 3: mouseDoubleClickEvent((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        case 4: dragEnterEvent((*reinterpret_cast< QGraphicsSceneDragDropEvent*(*)>(_a[1]))); break;
        case 5: wheelEvent((*reinterpret_cast< QGraphicsSceneWheelEvent*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
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
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   10,    9,    9, 0x0a,
      59,   10,    9,    9, 0x0a,
     101,   10,    9,    9, 0x0a,
     146,   10,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_KTexture[] = {
    "KTexture\0\0event\0"
    "mousePressEvent(QGraphicsSceneMouseEvent*)\0"
    "mouseMoveEvent(QGraphicsSceneMouseEvent*)\0"
    "mouseReleaseEvent(QGraphicsSceneMouseEvent*)\0"
    "dragEnterEvent(QGraphicsSceneDragDropEvent*)\0"
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
static const uint qt_meta_data_KComplexItem[] = {

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

static const char qt_meta_stringdata_KComplexItem[] = {
    "KComplexItem\0"
};

const QMetaObject KComplexItem::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_KComplexItem,
      qt_meta_data_KComplexItem, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KComplexItem::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KComplexItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KComplexItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KComplexItem))
        return static_cast<void*>(const_cast< KComplexItem*>(this));
    return QObject::qt_metacast(_clname);
}

int KComplexItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
