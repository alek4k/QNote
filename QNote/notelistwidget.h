#ifndef NOTELISTWIDGET_H
#define NOTELISTWIDGET_H

#include <QObject>
#include <QtWidgets>
#include "container.h"
#include "note.h"

class NoteListWidget : public QListWidget
{
private:
    ListaNote* list;
public:
    NoteListWidget(QWidget* parent = nullptr);
    /**
     * @brief addEntry Inserisce una nota nella lista
     * @param it Iteratore alla nota da inserire nella lista
     */
    void addEntry(const ListaNote::Iterator& it);
};

class NoteListWidgetItem : public QListWidgetItem
{
friend class NoteListWidget;
private:
    ListaNote::Iterator it;
    NoteListWidgetItem(QListWidget * parent, const ListaNote::Iterator& it);
    //widget che serve a visualizzare i tag sotto al titolo della nota nella lista
    QWidget *w = nullptr;
    //widget che contiene il nome dei tags della nota
    QLabel *tags = nullptr;
    //layout che contiene tags da aggiungere a w
    QVBoxLayout *hl = nullptr;
public:
    /**
     * @brief getNota Accesso alla nota dalla lista
     * @return L'iteratore alla nota dell'elemento della lista
     */
    ListaNote::Iterator getNota() const;
};

#endif // NOTELISTWIDGET_H
