#ifndef SIMPLENOTE_H
#define SIMPLENOTE_H

#pragma once

#include "nota.h"

class SimpleNote : public Nota
{
public:
    SimpleNote(const QString&, const QString&, const QVector<QString>&) noexcept;

    SimpleNote(const SimpleNote&) noexcept;

    SimpleNote& operator=(const SimpleNote&) = delete;

    ~SimpleNote() override = default;

    SimpleNote* clone() const noexcept override;

    void read(const QJsonObject& json) const override;

    void write(const QJsonObject& json) const override;
};

#endif // SIMPLENOTE_H
