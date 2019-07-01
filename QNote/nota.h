#ifndef NOTA_H
#define NOTA_H

#pragma once

#include <QString>
#include <QDateTime>
#include <QVector>
#include <QJsonObject>

/**
 * @brief Rappresentazione di una nota
 */
class Nota
{
private:
    QString titolo;
    QString descrizione;
    QDateTime dataUltimaModifica;
    QVector<QString> tag;
public:
    /**
     * @brief Costruisce una nuova nota
     * @param titolo il titolo della nota
     * @param descrizione il testo della nota
     */
    Nota(const QString& titolo, const QString& descrizione, const QVector<QString>& tag) noexcept;

    Nota(const Nota&) = default;

    virtual Nota& operator = (const Nota&) = delete;

    virtual ~Nota() = default;

    virtual Nota* clone() const noexcept = 0;

    QString getTitolo() const noexcept;

    QString getDescrizione() const noexcept;

    QDateTime getDataModifica() const noexcept;

    QVector<QString> getTag() const;

    void setTitolo(const QString& titolo) noexcept;

    void setDescrizione(const QString& descrizione) noexcept;

    void setDataModifica(const QDateTime& dataModifica) noexcept;

    void setTag(const QVector<QString> tag);
};

#endif // NOTA_H
