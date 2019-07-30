#include "imgnote.h"

ImgNote::ImgNote(const QString& titolo, const QString& descrizione, const QVector<QString>& tag, const QString& img64, const QDateTime& data) noexcept
    : Nota(titolo, descrizione, tag, data), img64(img64) {}

ImgNote::ImgNote(const ImgNote& imgnt) noexcept
    : ImgNote(imgnt.getTitolo(), imgnt.getDescrizione(), imgnt.getTag(), imgnt.img64) {}

ImgNote* ImgNote::clone() const noexcept {
    return new ImgNote(*this);
}

QString ImgNote::getImage() const {
    return img64;
}

void ImgNote::setImage(const QString& img64) noexcept {
    this->img64 = img64;
}
