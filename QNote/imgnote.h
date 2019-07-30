#ifndef IMGNOTE_H
#define IMGNOTE_H

#pragma once

#include "nota.h"

class ImgNote : public Nota
{
private:
    QString img64;
public:
    ImgNote(const QString& titolo, const QString& descrizione, const QVector<QString>& tag, const QString& immagine, const QDateTime& data = QDateTime::currentDateTime()) noexcept;

    ImgNote(const ImgNote&) noexcept;

    ImgNote* clone() const noexcept override;

    QString getImage() const;

    void setImage(const QString&) noexcept;
};

#endif // IMGNOTE_H
