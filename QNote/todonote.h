#ifndef TODOLIST_H
#define TODOLIST_H

#include "nota.h"
#include "todoitem.h"

class ToDoNote : public Nota
{
private:
    QList<ToDoItem> toDoList;
public:
    ToDoNote(const QString& titolo, const QString& descrizione, const QVector<QString>& tag, const QList<ToDoItem>& toDoList = QList<ToDoItem>()) noexcept;

    ToDoNote(const ToDoNote&) noexcept;

    ToDoNote& operator = (const ToDoNote&) = delete;

    ~ToDoNote() override = default;

    ToDoNote* clone() const noexcept override;

    QList<ToDoItem> getToDoList() const noexcept;

    void setToDoList(const QList<ToDoItem>&) noexcept;

    int targetCompletati() const noexcept;
};

#endif // TODOLIST_H
