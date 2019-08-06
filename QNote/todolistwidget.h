#ifndef TODOLISTWIDGET_H
#define TODOLISTWIDGET_H

#include <QObject>
#include <QtWidgets>
#include "container.h"
#include "todoitem.h"
#include "todonote.h"

class ToDoListWidget : public QListWidget
{
private:
    QList<ToDoItem>* toDoList;
public:
    ToDoListWidget(QWidget *parent = nullptr);
    void addEntry(ToDoItem* item);
};

class ToDoListWidgetItem : public QListWidgetItem
{
friend class ToDoListWidget;
private:
    ToDoItem* item;
    ToDoListWidgetItem(ToDoItem* item);
public:
    ToDoItem* getToDo() const;
};

#endif // TODOLISTWIDGET_H
