#include "todoitem.h"

ToDoItem::ToDoItem(const QString& target, const bool& completato) noexcept
    : target(target), completato(completato) {}

void ToDoItem::updateItem(const QString &target, const bool &completato) noexcept {
    this->target = target;
    this->completato = completato;
}

QString ToDoItem::getTarget() const noexcept {
    return target;
}

ToDoItem::operator bool() const noexcept {
    return completato;
}
