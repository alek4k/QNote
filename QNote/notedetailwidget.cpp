#include "notedetailwidget.h"

NoteDetailWidget::NoteDetailWidget(QWidget* parent)
    : nota(nullptr), textArea(new QTextEdit)
{
    QWidget *w = new QWidget;
    QHBoxLayout *hl = new QHBoxLayout;

    hl->addWidget(textArea);

    w->setLayout(hl);

}


void NoteDetailWidget::showNota(Nota &nota) {
    clear();
    this->nota = &nota;

    textArea->setText(this->nota->getDescrizione());
}

void NoteDetailWidget::clear() {
    nota = nullptr;
}
