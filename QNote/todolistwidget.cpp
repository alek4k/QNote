#include "todolistwidget.h"

ToDoListWidget::ToDoListWidget(QWidget *parent) : QListWidget(parent)
{
    //permetto la selezione di un solo obiettivo per volta (niente combo Ctrl/Shift)
    setSelectionMode(QAbstractItemView::SingleSelection);
}

ToDoListWidgetItem::ToDoListWidgetItem(ToDoItem* item)
    : item(item) {
    //imposto il testo dell'elemento della lista
    setText(item->getTarget());
}

ToDoItem* ToDoListWidgetItem::getToDo() const
{
    //ritorno il puntatore all'obiettivo dell'elemento della lista
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
