#include "todolistwidget.h"

ToDoListWidget::ToDoListWidget(QWidget *parent) : QListWidget(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
}

ToDoListWidgetItem::ToDoListWidgetItem(const ToDoItem& item)
    : QListWidgetItem(), item(item) {}

ToDoItem ToDoListWidgetItem::getToDo() const
{
    return item;
}

void ToDoListWidget::addEntry(const ToDoItem &it)
{
    QListWidgetItem *item = new ToDoListWidgetItem(it);
    item->setText(it.getTarget());
    item->setFont(QFont("Arial", 12, QFont::Normal));

    //rendo oggetti della lista editabili e con checkbox
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEditable);
    //attivo o meno il checkbox in base allo stato
    item->setCheckState(it ? Qt::Checked : Qt::Unchecked);

    //testo oggetto editabile
    editItem(item);

    //item->setBackground(QColor("#7fc97f"));

    //aggiungo l'oggetto alla lista
    addItem(item);
}
