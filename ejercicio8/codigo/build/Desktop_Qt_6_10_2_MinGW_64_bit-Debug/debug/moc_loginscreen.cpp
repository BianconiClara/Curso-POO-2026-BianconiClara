/****************************************************************************
** Meta object code from reading C++ file 'loginscreen.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/loginscreen.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'loginscreen.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.2. It"
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
struct qt_meta_tag_ZN11LoginScreenE_t {};
} // unnamed namespace

template <> constexpr inline auto LoginScreen::qt_create_metaobjectdata<qt_meta_tag_ZN11LoginScreenE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "LoginScreen",
        "loginExitoso",
        "",
        "userName",
        "bloqueoSolicitado",
        "seconds",
        "intentarLogin",
        "restablecerFormulario"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'loginExitoso'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'bloqueoSolicitado'
        QtMocHelpers::SignalData<void(int)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 5 },
        }}),
        // Slot 'intentarLogin'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'restablecerFormulario'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<LoginScreen, qt_meta_tag_ZN11LoginScreenE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject LoginScreen::staticMetaObject = { {
    QMetaObject::SuperData::link<Pantalla::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11LoginScreenE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11LoginScreenE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11LoginScreenE_t>.metaTypes,
    nullptr
} };

void LoginScreen::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<LoginScreen *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->loginExitoso((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->bloqueoSolicitado((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->intentarLogin(); break;
        case 3: _t->restablecerFormulario(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (LoginScreen::*)(const QString & )>(_a, &LoginScreen::loginExitoso, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (LoginScreen::*)(int )>(_a, &LoginScreen::bloqueoSolicitado, 1))
            return;
    }
}

const QMetaObject *LoginScreen::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LoginScreen::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11LoginScreenE_t>.strings))
        return static_cast<void*>(this);
    return Pantalla::qt_metacast(_clname);
}

int LoginScreen::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Pantalla::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void LoginScreen::loginExitoso(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void LoginScreen::bloqueoSolicitado(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}
QT_WARNING_POP
