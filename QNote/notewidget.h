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
#include "todolistwidget.h"
#include "note.h"

class NoteWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NoteWidget(ListaNote& note, QString& percorsoFile, QWidget *parent = nullptr);
    ~NoteWidget() override;
    void setPath(QString& percorsoFile);
protected slots:
    void refreshList() const;
    void cancellaNota(const ListaNote::Iterator&);
    void aggiornaNota(ListaNote::Iterator&, Nota*);
    void addTag(const ListaNote::Iterator&);
    void imageOpen(ListaNote::Iterator&);
    bool loadFile(const QString&, ListaNote::Iterator&);
    void highlightChecked(QListWidgetItem*);
private:
    QPlainTextEdit* const textArea;
    QGridLayout* const layout;
    QLineEdit* const searchBar;
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
    ToDoListWidget* const todoList;

    ListaNote& note;

    NoteListWidget* lista;

    QString path;

    int currentRowNota;

    void quickSave() const;
};

#endif // NOTEWIDGET_H
