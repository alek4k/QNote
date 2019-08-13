#ifndef NOTA_H
#define NOTA_H

#pragma once

#include <QString>
#include <QDateTime>
#include <QVector>
#include <QJsonObject>

/**
 * @brief Classe base astratta che rappresenta una nota
 */
class Nota
{
private:
    QString titolo;
    QString descrizione;
    //i tag consentono di catalogare le note per argomenti/aree e vengono sfruttati anche in fase di ricerca
    QVector<QString> tag;
    //la data serve in particolare ad ordinare le note in ordine decrescente di modifica
    QDateTime dataUltimaModifica;
public:
    /**
     * @brief Costruisce una nuova nota
     * @param titolo Titolo della nota
     * @param descrizione Testo della nota
     * @param tag Contenitore di tag della nota
     * @param data Data di creazione della nota (default: data e ora attuali)
     */
    Nota(const QString& titolo, const QString& descrizione, const QVector<QString>& tag, const QDateTime& data = QDateTime::currentDateTime());

    Nota(const Nota&) = default;

    virtual Nota& operator = (const Nota&) = delete;

    virtual ~Nota() = default;

    /**
     * @brief clone Clona la nota in una nuova nota identica
     * @return la copia della nota
     */
    virtual Nota* clone() const = 0;

    /**
     * @brief getTitolo
     * @return il titolo della nota
     */
    QString getTitolo() const;

    /**
     * @brief getDescrizione
     * @return il testo della nota
     */
    QString getDescrizione() const;

    /**
     * @brief getDataModifica
     * @return la data di ultima modifica della nota
     */
    QDateTime getDataModifica() const;

    /**
     * @brief getTag
     * @return il contenitore di tag della nota
     */
    QVector<QString> getTag() const;

    /**
     * @brief setTitolo Imposta il titolo della nota
     * @param titolo Stringa da impostare come titolo della nota
     */
    void setTitolo(const QString& titolo);

    /**
     * @brief setDescrizione Imposta la descrizione della nota
     * @param descrizione Il testo da impostare come descrizione
     */
    void setDescrizione(const QString& descrizione);

    /**
     * @brief setDataModifica Aggiornamento data modifica
     * @param dataModifica La data di ultima modifica della nota
     */
    void setDataModifica(const QDateTime& dataModifica = QDateTime::currentDateTime());

    /**
     * @brief setTag
     * @param tag Tag da impostare come tag della nota
     */
    void setTag(const QVector<QString>& tag);
};

#endif // NOTA_H
