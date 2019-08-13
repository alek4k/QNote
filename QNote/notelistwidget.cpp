#include "notelistwidget.h"
#include "container.h"

NoteListWidget::NoteListWidget(QWidget* parent) : QListWidget(parent) {
    //permetto la selezione di una sola nota per volta (niente combo Ctrl/Shift)
    setSelectionMode(QAbstractItemView::SingleSelection);
}

NoteListWidgetItem::NoteListWidgetItem(QListWidget * parent, const Container<Nota>::Iterator& it)
    : QListWidgetItem(parent), it(it), w(new QWidget(parent)), tags(new QLabel(parent)), hl(new QVBoxLayout(parent)) {
    //tags da visualizzare sotto al titolo
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

    this->setSizeHint(w->sizeHint());

    parent->setItemWidget(this, w);
}

Container<Nota>::Iterator NoteListWidgetItem::getNota() const {
    return it;
}

void NoteListWidget::addEntry(const ListaNote::Iterator& it) {
    QListWidgetItem *item = new NoteListWidgetItem(this, it);

    QString titolo = it->getTitolo();
    //se si tratta di una todonote visualizzo anche il conteggio di obiettivi completati
    if (auto toDoNoteInsert = dynamic_cast<const ToDoNote*>(&(*it))) {
        unsigned int completed = toDoNoteInsert->targetCompletati();
        int tot = toDoNoteInsert->getToDoList().size();
        titolo += " ("+QString::number(completed)+"/"+QString::number(tot)+")";
    }

    item->setText(titolo);
    item->setFont(QFont("Arial", 12, QFont::Normal));

    //aggiungo l'elemento alla lista
    addItem(item);
}
