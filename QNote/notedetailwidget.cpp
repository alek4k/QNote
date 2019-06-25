#include "notedetailwidget.h"

NoteDetailWidget::NoteDetailWidget(QWidget* parent)
    : nota(nullptr), textArea(new QTextEdit)
{
    QVBoxLayout *hl = new QVBoxLayout;

    hl->addWidget(textArea);
}


void NoteDetailWidget::showNota(Nota &nota) {
    clear();
    this->nota = &nota;

    textArea = new QTextEdit();
    textArea->setText(nota.getDescrizione());
}

void NoteDetailWidget::clear() {
    nota = nullptr;
}
