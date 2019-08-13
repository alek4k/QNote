#ifndef IMGNOTE_H
#define IMGNOTE_H

#pragma once

#include "nota.h"

class ImgNote : public Nota
{
private:
    //immagine salvata in base64
    QString img64;
public:
    /**
     * @brief ImgNote Costruttore di note con immagine
     * @param titolo Titolo della nota
     * @param descrizione Descrizione della nota
     * @param tag Contenitore di tag
     * @param immagine Immagine convertita in base64
     * @param data Data di creazione della nota (orario corrente di default)
     */
    ImgNote(const QString& titolo, const QString& descrizione, const QVector<QString>& tag, const QString& immagine, const QDateTime& data = QDateTime::currentDateTime());

    /**
     * @brief ImgNote costruttore di copia
     */
    ImgNote(const ImgNote&);

    /**
     * @brief clone Clona l'oggetto di inocazione
     * @return la copia dell'oggetto di invocazione
     */
    ImgNote* clone() const override;

    /**
     * @brief getImage Ritorna l'immagine della nota
     * @return l'immagine in base64 della nota
     */
    QString getImage() const;

    /**
     * @brief setImage Imposta l'immagine della nota in base64
     */
    void setImage(const QString&);
};

#endif // IMGNOTE_H
