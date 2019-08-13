#include "todoitem.h"

ToDoItem::ToDoItem(const QString& target, const bool& completato)
    : target(target), completato(completato) {}

void ToDoItem::updateItem(const QString &target, const bool &completato) {
    this->target = target;
    this->completato = completato;
}

QString ToDoItem::getTarget() const {
    return target;
}

ToDoItem::operator bool() const {
    return completato;
}
