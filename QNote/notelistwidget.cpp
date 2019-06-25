#include "notelistwidget.h"
#include "container.h"

NoteListWidget::NoteListWidget(Container<Nota>* list, QWidget* parent)
    : QListWidget(parent), list(list)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
}

NoteListWidgetItem::NoteListWidgetItem(const Container<Nota>::Iterator& it)
    : QListWidgetItem(), it(it) {}


Container<Nota>::Iterator NoteListWidgetItem::getNota() const {
    return it;
}

void NoteListWidget::addEntry(const Container<Nota>::Iterator& it, bool hidden) {
    QWidget *w = new QWidget;
    QHBoxLayout *hl = new QHBoxLayout;

    //color
    QPixmap pm(10,10);
    pm.fill();
    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing, true);
    QPen pen(Qt::black, 0);
    p.setPen(pen);
    QColor color;
    color.setRgb(255, 255, 000);
    QBrush brush(color);
    p.setBrush(brush);
    p.drawEllipse(0, 0, 10, 10);
    QLabel *l = new QLabel;
    l->setPixmap(pm);
    l->setMaximumSize(20, 20);
    hl->addWidget(l);

    //title
    hl->addWidget(new QLabel(it->getTitolo()));

    w->setLayout(hl);

    QListWidgetItem *item = new NoteListWidgetItem(it);
    addItem(item);
    item->setSizeHint(w->sizeHint());
    item->setHidden(hidden);
    setItemWidget(item, w);

}
