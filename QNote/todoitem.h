#ifndef TODOITEM_H
#define TODOITEM_H

#pragma once

#include <QString>

class ToDoItem
{
private:
    QString target;
    bool completato;
public:
    ToDoItem(const QString& target, const bool& completato = false) noexcept;

    ToDoItem(const ToDoItem&) noexcept = default;

    virtual ~ToDoItem() = default;

    void updateItem(const QString& target, const bool& completato) noexcept;

    operator bool () const noexcept;
};

#endif // TODOITEM_H
