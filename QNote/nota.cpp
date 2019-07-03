#include "nota.h"

Nota::Nota(const QString& title, const QString& text, const QVector<QString>& tag) noexcept
    : titolo(title), descrizione(text), tag(tag)
{
    dataUltimaModifica = QDateTime::currentDateTime();
    //il titolo è la prima riga della descrizione
    descrizione = (titolo.compare("") == 0) ? titolo : titolo + "\n" + descrizione;
}

QString Nota::getTitolo() const noexcept {
    return titolo;
}

QString Nota::getDescrizione() const noexcept {
    return descrizione;
}

QDateTime Nota::getDataModifica() const noexcept {
    return dataUltimaModifica;
}

QVector<QString> Nota::getTag() const {
    return tag;
}

void Nota::setTitolo(const QString& title) noexcept {
    titolo = title;
}

void Nota::setDescrizione(const QString& text) noexcept {
    descrizione = text;
}

void Nota::setDataModifica(const QDateTime& dataModifica) noexcept {
    dataUltimaModifica = dataModifica;
}

void Nota::setTag(const QVector<QString> tag) {
    this->tag = tag;
}
