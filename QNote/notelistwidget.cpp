#include "notelistwidget.h"
#include "container.h"

NoteListWidget::NoteListWidget(QWidget* parent) : QListWidget(parent)/*, w(new QWidget(this)), tags(new QLabel(this)), hl(new QVBoxLayout(this))*/ {
    //permetto la selezione di una sola nota per volta (niente combo Ctrl/Shift)
    setSelectionMode(QAbstractItemView::SingleSelection);
}

NoteListWidgetItem::NoteListWidgetItem(QListWidget * parent, const Container<Nota>::Iterator& it)
    : QListWidgetItem(parent), it(it) {}


Container<Nota>::Iterator NoteListWidgetItem::getNota() const {
    return it;
}

void NoteListWidget::addEntry(const ListaNote::Iterator& it) {
    if (!it.isValid()) return;

    QVBoxLayout* hl = new QVBoxLayout(this);
    QWidget* w = new QWidget(this);
    QLabel* tags = new QLabel(this);

    //tags
    for (int i = 0; i < it->getTag().length(); i++) {
        if (i==0)
            tags->setText(it->getTag()[i]);
        else
            tags->setText(tags->text() + ", " + it->getTag()[i]);
    }
    tags->setStyleSheet("margin-left: 10px; margin-top:30px;");
    QFont f( "Arial", 10, QFont::Light);
    tags->setFont(f);
    tags->setAlignment(Qt::AlignBottom);

    hl->addWidget(tags);
    w->setLayout(hl);

    QListWidgetItem *item = new NoteListWidgetItem(this, it);

    QString titolo = (*it).getTitolo();
    if (auto toDoNoteInsert = dynamic_cast<const ToDoNote*>(&(*it))) {
        unsigned int completed = toDoNoteInsert->targetCompletati();
        int tot = toDoNoteInsert->getToDoList().size();
        titolo += " ("+QString::number(completed)+"/"+QString::number(tot)+")";
    }

    item->setText(titolo);
    item->setFont(QFont("Arial", 12, QFont::Normal));

    item->setSizeHint(w->sizeHint());

    addItem(item);

    setItemWidget(item, w);
}
