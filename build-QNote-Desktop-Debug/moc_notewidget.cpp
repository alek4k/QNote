/****************************************************************************
** Meta object code from reading C++ file 'notewidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../QNote/notewidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'notewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NoteWidget_t {
    QByteArrayData data[11];
    char stringdata0[123];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NoteWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NoteWidget_t qt_meta_stringdata_NoteWidget = {
    {
QT_MOC_LITERAL(0, 0, 10), // "NoteWidget"
QT_MOC_LITERAL(1, 11, 11), // "refreshList"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 12), // "cancellaNota"
QT_MOC_LITERAL(4, 37, 19), // "ListaNote::Iterator"
QT_MOC_LITERAL(5, 57, 12), // "aggiornaNota"
QT_MOC_LITERAL(6, 70, 20), // "ListaNote::Iterator&"
QT_MOC_LITERAL(7, 91, 5), // "Nota*"
QT_MOC_LITERAL(8, 97, 6), // "addTag"
QT_MOC_LITERAL(9, 104, 9), // "imageOpen"
QT_MOC_LITERAL(10, 114, 8) // "loadFile"

    },
    "NoteWidget\0refreshList\0\0cancellaNota\0"
    "ListaNote::Iterator\0aggiornaNota\0"
    "ListaNote::Iterator&\0Nota*\0addTag\0"
    "imageOpen\0loadFile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NoteWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x09 /* Protected */,
       3,    1,   45,    2, 0x09 /* Protected */,
       5,    2,   48,    2, 0x09 /* Protected */,
       8,    1,   53,    2, 0x09 /* Protected */,
       9,    1,   56,    2, 0x09 /* Protected */,
      10,    2,   59,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    2,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 7,    2,    2,
    QMetaType::Void, 0x80000000 | 4,    2,
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Bool, QMetaType::QString, 0x80000000 | 6,    2,    2,

       0        // eod
};

void NoteWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NoteWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->refreshList(); break;
        case 1: _t->cancellaNota((*reinterpret_cast< const ListaNote::Iterator(*)>(_a[1]))); break;
        case 2: _t->aggiornaNota((*reinterpret_cast< ListaNote::Iterator(*)>(_a[1])),(*reinterpret_cast< Nota*(*)>(_a[2]))); break;
        case 3: _t->addTag((*reinterpret_cast< const ListaNote::Iterator(*)>(_a[1]))); break;
        case 4: _t->imageOpen((*reinterpret_cast< ListaNote::Iterator(*)>(_a[1]))); break;
        case 5: { bool _r = _t->loadFile((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< ListaNote::Iterator(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NoteWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_NoteWidget.data,
    qt_meta_data_NoteWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NoteWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NoteWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NoteWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int NoteWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
