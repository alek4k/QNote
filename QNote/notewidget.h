#ifndef NOTEWIDGET_H
#define NOTEWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include <QGridLayout>
#include <QListView>
#include <QListWidget>
#include <QLineEdit>
#include <QImage>
#include "notelistwidget.h"
#include "note.h"

class NoteWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NoteWidget(ListaNote& note, QWidget *parent = nullptr);
    virtual ~NoteWidget() = default;

protected slots:
    void refreshList();

private:
    QPlainTextEdit * textArea;
    QLineEdit* const lineText;
    QGridLayout* const layout;
    QLineEdit* const searchBar;
    QPixmap* const image;

    ListaNote& note;

    NoteListWidget* lista;


};

#endif // NOTEWIDGET_H
