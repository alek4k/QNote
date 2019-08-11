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
    void salvato();
    bool modificheInSospeso() const;
protected slots:
    void refreshList() const;
    void creaNota();
    void cancellaNota();
    void scritturaNota();
    void trasformaInObiettivo();
    void cercaNote(const QString ricerca) const;
    void aggiornaNota(ListaNote::Iterator&, Nota*);
    void addTag();
    void imageOpen();
    void selezioneNota();
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
    bool modificato;
};

#endif // NOTEWIDGET_H
