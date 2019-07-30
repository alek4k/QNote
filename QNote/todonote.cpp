#include "todonote.h"

ToDoNote::ToDoNote(const QString& titolo, const QString& descrizione, const QVector<QString>& tag, const QList<ToDoItem>& toDoList, const QDateTime& data) noexcept
    : Nota(titolo, descrizione, tag, data), toDoList(toDoList) {}

ToDoNote::ToDoNote(const ToDoNote& tdnt) noexcept
    : ToDoNote(tdnt.getTitolo(), tdnt.getDescrizione(), tdnt.getTag(), tdnt.toDoList) {}

ToDoNote* ToDoNote::clone() const noexcept {
    return new ToDoNote(*this);
}

QList<ToDoItem> ToDoNote::getToDoList() const noexcept {
    return toDoList;
}

void ToDoNote::setToDoList(const QList<ToDoItem>& list) noexcept {
    toDoList = list;
}

int ToDoNote::targetCompletati() const noexcept {
    int completati = 0;
    for (auto it = toDoList.begin(); it != toDoList.end(); ++it) {
        if (*it) completati++;
    }
    return completati;
}
