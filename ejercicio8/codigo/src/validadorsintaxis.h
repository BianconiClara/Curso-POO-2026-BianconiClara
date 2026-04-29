#ifndef VALIDADORSINTAXIS_H
#define VALIDADORSINTAXIS_H

#include "models.h"

class ValidadorSintaxis
{
public:
    virtual ~ValidadorSintaxis() = default;

    virtual QString lenguaje() const = 0;
    virtual ValidationResult validarLinea(const QString &linea) const = 0;
};

class ValidadorCpp : public ValidadorSintaxis
{
public:
    QString lenguaje() const override;
    ValidationResult validarLinea(const QString &linea) const override;
};

class ValidadorPython : public ValidadorSintaxis
{
public:
    QString lenguaje() const override;
    ValidationResult validarLinea(const QString &linea) const override;
};

class ValidadorJava : public ValidadorSintaxis
{
public:
    QString lenguaje() const override;
    ValidationResult validarLinea(const QString &linea) const override;
};

#endif // VALIDADORSINTAXIS_H
