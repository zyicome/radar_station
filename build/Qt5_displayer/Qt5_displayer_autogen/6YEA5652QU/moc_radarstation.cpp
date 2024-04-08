/****************************************************************************
** Meta object code from reading C++ file 'radarstation.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../src/Qt5_displayer/include/radarstation.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'radarstation.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_radarStation_t {
    QByteArrayData data[13];
    char stringdata0[215];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_radarStation_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_radarStation_t qt_meta_stringdata_radarStation = {
    {
QT_MOC_LITERAL(0, 0, 12), // "radarStation"
QT_MOC_LITERAL(1, 13, 17), // "changeToMapWidget"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 17), // "changeToPnpWidget"
QT_MOC_LITERAL(4, 50, 19), // "changeToMapWidget_2"
QT_MOC_LITERAL(5, 70, 19), // "changeToPnpWidget_2"
QT_MOC_LITERAL(6, 90, 14), // "farImageUpdate"
QT_MOC_LITERAL(7, 105, 16), // "closeImageUpdate"
QT_MOC_LITERAL(8, 122, 19), // "farDepthImageUpdate"
QT_MOC_LITERAL(9, 142, 21), // "closeDepthImageUpdate"
QT_MOC_LITERAL(10, 164, 16), // "publishPnpResult"
QT_MOC_LITERAL(11, 181, 15), // "farPointsUpdate"
QT_MOC_LITERAL(12, 197, 17) // "closePointsUpdate"

    },
    "radarStation\0changeToMapWidget\0\0"
    "changeToPnpWidget\0changeToMapWidget_2\0"
    "changeToPnpWidget_2\0farImageUpdate\0"
    "closeImageUpdate\0farDepthImageUpdate\0"
    "closeDepthImageUpdate\0publishPnpResult\0"
    "farPointsUpdate\0closePointsUpdate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_radarStation[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x0a /* Public */,
       3,    0,   70,    2, 0x0a /* Public */,
       4,    0,   71,    2, 0x0a /* Public */,
       5,    0,   72,    2, 0x0a /* Public */,
       6,    0,   73,    2, 0x0a /* Public */,
       7,    0,   74,    2, 0x0a /* Public */,
       8,    0,   75,    2, 0x0a /* Public */,
       9,    0,   76,    2, 0x0a /* Public */,
      10,    0,   77,    2, 0x0a /* Public */,
      11,    0,   78,    2, 0x0a /* Public */,
      12,    0,   79,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void radarStation::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<radarStation *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->changeToMapWidget(); break;
        case 1: _t->changeToPnpWidget(); break;
        case 2: _t->changeToMapWidget_2(); break;
        case 3: _t->changeToPnpWidget_2(); break;
        case 4: _t->farImageUpdate(); break;
        case 5: _t->closeImageUpdate(); break;
        case 6: _t->farDepthImageUpdate(); break;
        case 7: _t->closeDepthImageUpdate(); break;
        case 8: _t->publishPnpResult(); break;
        case 9: _t->farPointsUpdate(); break;
        case 10: _t->closePointsUpdate(); break;
        default: ;
        }
    }
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject radarStation::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_radarStation.data,
    qt_meta_data_radarStation,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *radarStation::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *radarStation::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_radarStation.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int radarStation::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
