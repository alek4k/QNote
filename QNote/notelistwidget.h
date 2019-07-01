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
    void addEntry(const ListaNote::ConstIterator& it);
};

class NoteListWidgetItem : public QListWidgetItem
{
friend class NoteListWidget;
private:
   ListaNote::ConstIterator it;
    NoteListWidgetItem(const ListaNote::ConstIterator& it);
public:
    ListaNote::ConstIterator getNota() const;
};

#endif // NOTELISTWIDGET_H
