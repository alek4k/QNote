#ifndef SIMPLENOTE_H
#define SIMPLENOTE_H

#pragma once

#include "nota.h"

class SimpleNote : public Nota
{
public:
    SimpleNote(const QString& titolo, const QString& descrizione = "", const QVector<QString>& tag = QVector<QString>(), const QDateTime& data = QDateTime::currentDateTime()) noexcept;

    SimpleNote(const SimpleNote&) noexcept;

    SimpleNote& operator=(const SimpleNote&) = delete;

    ~SimpleNote() override = default;

    SimpleNote* clone() const noexcept override;
};

#endif // SIMPLENOTE_H
