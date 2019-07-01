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
    void addEntry(const ListaNote::Iterator& it);
};

class NoteListWidgetItem : public QListWidgetItem
{
friend class NoteListWidget;
private:
   ListaNote::Iterator it;
    NoteListWidgetItem(const ListaNote::Iterator& it);
public:
    ListaNote::Iterator getNota() const;
};

#endif // NOTELISTWIDGET_H
