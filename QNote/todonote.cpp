#include "todonote.h"

ToDoNote::ToDoNote(const QString& titolo, const QString& descrizione, const QVector<QString>& tag, const QList<ToDoItem>& toDoList, const QDateTime& data)
    : Nota(titolo, descrizione, tag, data), toDoList(toDoList) {}

ToDoNote::ToDoNote(const ToDoNote& tdnt)
    : ToDoNote(tdnt.getTitolo(), tdnt.getDescrizione(), tdnt.getTag(), tdnt.toDoList, tdnt.getDataModifica()) {}

ToDoNote* ToDoNote::clone() const {
    return new ToDoNote(*this);
}

QList<ToDoItem> ToDoNote::getToDoList() const {
    return toDoList;
}

void ToDoNote::setToDoList(const QList<ToDoItem>& list) {
    toDoList = list;
}

unsigned int ToDoNote::targetCompletati() const {
    unsigned int completati = 0;
    for (auto it = toDoList.begin(); it != toDoList.end(); ++it) {
        if (*it) completati++;
    }
    return completati;
}
