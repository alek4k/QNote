#include "simplenote.h"

SimpleNote::SimpleNote(const QString& title, const QString& text, const QVector<QString>& tag, const QDateTime& data) noexcept
    : Nota(title, text, tag, data) {}

SimpleNote::SimpleNote(const SimpleNote& smplnt) noexcept
    : SimpleNote(smplnt.getTitolo(), smplnt.getDescrizione(), smplnt.getTag()) {}

SimpleNote* SimpleNote::clone() const noexcept {
    return new SimpleNote(*this);
}
