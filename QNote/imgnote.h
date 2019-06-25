#ifndef IMGNOTE_H
#define IMGNOTE_H

#pragma once

#include "nota.h"

class ImgNote : public Nota
{
private:
    QString img64;
public:
    ImgNote(const QString&, const QString&, const QVector<QString>&, const QString&) noexcept;

    ImgNote(const ImgNote&) noexcept;

    ImgNote* clone() const noexcept override;

    QString getImage() const;

    void setImage(const QString&) noexcept;
};

#endif // IMGNOTE_H
