#include "todolistwidget.h"

ToDoListWidget::ToDoListWidget(QWidget *parent) : QListWidget(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
}

ToDoListWidgetItem::ToDoListWidgetItem(ToDoItem* item)
    : item(item) {
    setText(item->getTarget());
}

ToDoItem* ToDoListWidgetItem::getToDo() const
{
    return item;
}

void ToDoListWidget::addEntry(ToDoItem* it)
{
    QListWidgetItem *item = new ToDoListWidgetItem(it);
    item->setFont(QFont("Arial", 12, QFont::Normal));

    //rendo oggetti della lista editabili e con checkbox
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEditable);
    //attivo o meno il checkbox in base allo stato
    item->setCheckState((*it) ? Qt::Checked : Qt::Unchecked);

    //aggiungo l'oggetto alla lista
    addItem(item);
}
