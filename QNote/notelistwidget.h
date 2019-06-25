#ifndef NOTELISTWIDGET_H
#define NOTELISTWIDGET_H

#include <QObject>
#include <QtWidgets>
#include "container.h"
#include "nota.h"

class NoteListWidget : public QListWidget
{
private:
    Container<Nota>* list;
public:
    NoteListWidget(Container<Nota>* list, QWidget* parent = nullptr);
    void addEntry(const Container<Nota>::Iterator& it, bool hidden = false);
};

class NoteListWidgetItem : public QListWidgetItem
{
friend class NoteListWidget;
private:
    Container<Nota>::Iterator it;
    NoteListWidgetItem(const Container<Nota>::Iterator& it);
public:
    Container<Nota>::Iterator getNota() const;
};

#endif // NOTELISTWIDGET_H
