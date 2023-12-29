/****************************************************************************
** Meta object code from reading C++ file 'modificarsaida.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Circuito/modificarsaida.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'modificarsaida.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSModificarSaidaENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSModificarSaidaENDCLASS = QtMocHelpers::stringData(
    "ModificarSaida",
    "signModificarSaida",
    "",
    "idSaida",
    "idOrigemSaida",
    "slotSetRangeInputs",
    "minimo",
    "maximo",
    "slotShowModificarSaida",
    "IdSaida",
    "IdOrigemSaida",
    "on_spinOrigemSaida_valueChanged",
    "arg1",
    "on_buttonBox_accepted"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSModificarSaidaENDCLASS_t {
    uint offsetsAndSizes[28];
    char stringdata0[15];
    char stringdata1[19];
    char stringdata2[1];
    char stringdata3[8];
    char stringdata4[14];
    char stringdata5[19];
    char stringdata6[7];
    char stringdata7[7];
    char stringdata8[23];
    char stringdata9[8];
    char stringdata10[14];
    char stringdata11[32];
    char stringdata12[5];
    char stringdata13[22];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSModificarSaidaENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSModificarSaidaENDCLASS_t qt_meta_stringdata_CLASSModificarSaidaENDCLASS = {
    {
        QT_MOC_LITERAL(0, 14),  // "ModificarSaida"
        QT_MOC_LITERAL(15, 18),  // "signModificarSaida"
        QT_MOC_LITERAL(34, 0),  // ""
        QT_MOC_LITERAL(35, 7),  // "idSaida"
        QT_MOC_LITERAL(43, 13),  // "idOrigemSaida"
        QT_MOC_LITERAL(57, 18),  // "slotSetRangeInputs"
        QT_MOC_LITERAL(76, 6),  // "minimo"
        QT_MOC_LITERAL(83, 6),  // "maximo"
        QT_MOC_LITERAL(90, 22),  // "slotShowModificarSaida"
        QT_MOC_LITERAL(113, 7),  // "IdSaida"
        QT_MOC_LITERAL(121, 13),  // "IdOrigemSaida"
        QT_MOC_LITERAL(135, 31),  // "on_spinOrigemSaida_valueChanged"
        QT_MOC_LITERAL(167, 4),  // "arg1"
        QT_MOC_LITERAL(172, 21)   // "on_buttonBox_accepted"
    },
    "ModificarSaida",
    "signModificarSaida",
    "",
    "idSaida",
    "idOrigemSaida",
    "slotSetRangeInputs",
    "minimo",
    "maximo",
    "slotShowModificarSaida",
    "IdSaida",
    "IdOrigemSaida",
    "on_spinOrigemSaida_valueChanged",
    "arg1",
    "on_buttonBox_accepted"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSModificarSaidaENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   44,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    2,   49,    2, 0x0a,    4 /* Public */,
       8,    2,   54,    2, 0x0a,    7 /* Public */,
      11,    1,   59,    2, 0x08,   10 /* Private */,
      13,    0,   62,    2, 0x08,   12 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    9,   10,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject ModificarSaida::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_CLASSModificarSaidaENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSModificarSaidaENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSModificarSaidaENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ModificarSaida, std::true_type>,
        // method 'signModificarSaida'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'slotSetRangeInputs'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'slotShowModificarSaida'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_spinOrigemSaida_valueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_buttonBox_accepted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void ModificarSaida::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ModificarSaida *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->signModificarSaida((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 1: _t->slotSetRangeInputs((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 2: _t->slotShowModificarSaida((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 3: _t->on_spinOrigemSaida_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->on_buttonBox_accepted(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ModificarSaida::*)(int , int );
            if (_t _q_method = &ModificarSaida::signModificarSaida; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *ModificarSaida::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ModificarSaida::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSModificarSaidaENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ModificarSaida::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void ModificarSaida::signModificarSaida(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
