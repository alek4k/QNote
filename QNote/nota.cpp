#include "nota.h"

Nota::Nota(const QString& title, const QString& text, const QVector<QString>& tag, const QDateTime& data)
    : titolo(title), descrizione(text), tag(tag), dataUltimaModifica(data) {}

QString Nota::getTitolo() const {
    return titolo;
}

QString Nota::getDescrizione() const {
    return descrizione;
}

QDateTime Nota::getDataModifica() const {
    return dataUltimaModifica;
}

QVector<QString> Nota::getTag() const {
    return tag;
}

void Nota::setTitolo(const QString& title) {
    titolo = title;
}

void Nota::setDescrizione(const QString& text) {
    descrizione = text;
}

void Nota::setDataModifica(const QDateTime& dataModifica) {
    dataUltimaModifica = dataModifica;
}

void Nota::setTag(const QVector<QString>& tag) {
    this->tag = tag;
}
