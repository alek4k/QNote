#include "simplenote.h"

SimpleNote::SimpleNote(const QString& title, const QString& text, const QVector<QString>& tag, const QDateTime& data)
    : Nota(title, text, tag, data) {}

SimpleNote::SimpleNote(const SimpleNote& smplnt)
    : SimpleNote(smplnt.getTitolo(), smplnt.getDescrizione(), smplnt.getTag(), smplnt.getDataModifica()) {}

SimpleNote* SimpleNote::clone() const {
    return new SimpleNote(*this);
}
