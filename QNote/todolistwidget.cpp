#include "todolistwidget.h"

ToDoListWidget::ToDoListWidget(QWidget *parent) : QListWidget(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
}

ToDoListWidgetItem::ToDoListWidgetItem(ToDoItem* item)
    : item(item) {
    setText(item->getTarget());
    //setCheckState((*item) ? Qt::Checked : Qt::Unchecked);
}

ToDoItem* ToDoListWidgetItem::getToDo() const
{
    return item;
}

void ToDoListWidget::addEntry(ToDoItem* it)
{
    QListWidgetItem *item = new ToDoListWidgetItem(it);
    //item->setText(it.getTarget());
    item->setFont(QFont("Arial", 12, QFont::Normal));

    //rendo oggetti della lista editabili e con checkbox
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
    //attivo o meno il checkbox in base allo stato
    item->setCheckState((*it) ? Qt::Checked : Qt::Unchecked);

    //item->setBackground(QColor("#7fc97f"));

    /*QWidget *w = new QWidget;
    QVBoxLayout *hl = new QVBoxLayout;
    hl->addWidget(new QCheckBox());
    w->setLayout(hl);*/

    //aggiungo l'oggetto alla lista
    addItem(item);

    //setItemWidget(item, w);

}
