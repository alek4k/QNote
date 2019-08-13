#ifndef SIMPLENOTE_H
#define SIMPLENOTE_H

#pragma once

#include "nota.h"

class SimpleNote : public Nota
{
public:
    /**
     * @brief SimpleNote Costruttore di note semplici (solo titolo, testo e tag)
     * @param titolo Titolo della nota
     * @param descrizione Descrizione della nota
     * @param tag Contenitore di tag
     * @param data Data di creazione della nota (orario corrente di default)
     */
    SimpleNote(const QString& titolo, const QString& descrizione = "", const QVector<QString>& tag = QVector<QString>(), const QDateTime& data = QDateTime::currentDateTime());

    SimpleNote(const SimpleNote&);

    SimpleNote& operator=(const SimpleNote&) = delete;

    ~SimpleNote() override = default;

    /**
     * @brief clone Clona la nota in una nuova nota identica
     * @return la copia della nota
     */
    SimpleNote* clone() const override;
};

#endif // SIMPLENOTE_H
