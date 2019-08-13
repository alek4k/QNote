#include "imgnote.h"

ImgNote::ImgNote(const QString& titolo, const QString& descrizione, const QVector<QString>& tag, const QString& img64, const QDateTime& data)
    : Nota(titolo, descrizione, tag, data), img64(img64) {}

ImgNote::ImgNote(const ImgNote& imgnt)
    : ImgNote(imgnt.getTitolo(), imgnt.getDescrizione(), imgnt.getTag(), imgnt.img64, imgnt.getDataModifica()) {}

ImgNote* ImgNote::clone() const {
    return new ImgNote(*this);
}

QString ImgNote::getImage() const {
    return img64;
}

void ImgNote::setImage(const QString& base64) {
    this->img64 = base64;
}
