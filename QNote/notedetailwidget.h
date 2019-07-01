#ifndef NOTEDETAILWIDGET_H
#define NOTEDETAILWIDGET_H

#include <QtWidgets>
#include "nota.h"
#include "container.h"

class NoteDetailWidget : public QWidget
{
    Q_OBJECT
private:
    Nota* nota;
    QTextEdit* textArea;
public:
    NoteDetailWidget(QWidget* parent = nullptr);

    void showNota(Nota& nota);

    void clear();
};

#endif // NOTEDETAILWIDGET_H
