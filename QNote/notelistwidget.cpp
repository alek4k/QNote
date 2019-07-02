#include "notelistwidget.h"
#include "container.h"

NoteListWidget::NoteListWidget(QWidget* parent)
    : QListWidget(parent) {
    setSelectionMode(QAbstractItemView::ExtendedSelection);
}

NoteListWidgetItem::NoteListWidgetItem(const Container<Nota>::Iterator& it)
    : QListWidgetItem(), it(it) {}


Container<Nota>::Iterator NoteListWidgetItem::getNota() const {
    return it;
}

void NoteListWidget::addEntry(const Container<Nota>::Iterator& it) {
    QWidget *w = new QWidget;
    QVBoxLayout *hl = new QVBoxLayout;

    //color
    /*QPixmap pm(10,10);
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
    hl->addWidget(l);*/

    //title
    QLabel* title = new QLabel(it->getTitolo());
    title->setFont(QFont("Arial", 12, QFont::Normal));
    hl->addWidget(title);

    //tag
    QLabel* tags = new QLabel("");
    //hl->addWidget(new QLabel("test"));
    for (int i = 0; i < it->getTag().length(); i++) {
        QString temp = it->getTag()[i];
        if (i==0)
            tags->setText(it->getTag()[i]);
        else
            tags->setText(tags->text() + ", " + it->getTag()[i]);
    }
    tags->setStyleSheet("margin-left: 10px;");
    //tags->show();
    //tags->resize(100,100);
    QFont f( "Arial", 10, QFont::Light);
    tags->setFont(f);

    hl->addWidget(tags);

    w->setLayout(hl);

    QListWidgetItem *item = new NoteListWidgetItem(it);

    addItem(item);

    item->setSizeHint(w->sizeHint());
    setItemWidget(item, w);

}
