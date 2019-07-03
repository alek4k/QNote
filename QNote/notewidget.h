#ifndef NOTEWIDGET_H
#define NOTEWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include <QGridLayout>
#include <QListView>
#include <QListWidget>
#include <QLineEdit>
#include <QImage>
#include <QMessageBox>
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
    void cancellaNota(ListaNote::Iterator&);
private:
    QPlainTextEdit* const textArea;
    QGridLayout* const layout;
    QLineEdit* const searchBar;
    QPixmap* image;
    QLabel* const imageLabel;
    QVBoxLayout* const colonnaSx;
    QVBoxLayout* const colonnaDx;
    QGridLayout* const barraTopLeft;
    QGridLayout* const barraTopRight;
    QToolButton* const addNotaButton;
    QToolButton* const deleteNotaButton;
    QToolButton* const addImgButton;
    QToolButton* const addToDoListButton;
    QToolButton* const addTagButton;

    ListaNote& note;

    NoteListWidget* lista;

};

#endif // NOTEWIDGET_H
