#ifndef TODOLIST_H
#define TODOLIST_H

#include "nota.h"
#include "todoitem.h"

class ToDoNote : public Nota
{
private:
    QList<ToDoItem> toDoList;
public:
    /**
     * @brief ToDoNote Costruttore di note con lista di obiettivi (ToDoItem)
     * @param titolo Titolo della nota
     * @param descrizione Descrizione della nota
     * @param tag Contenitore di tag
     * @param toDoList Contenitore di obiettivi
     * @param data Data di creazione della nota (orario corrente di default)
     */
    ToDoNote(const QString& titolo, const QString& descrizione, const QVector<QString>& tag, const QList<ToDoItem>& toDoList = QList<ToDoItem>(), const QDateTime& data = QDateTime::currentDateTime());

    ToDoNote(const ToDoNote&);

    ToDoNote& operator = (const ToDoNote&) = delete;

    ~ToDoNote() override = default;

    /**
     * @brief clone Clona la nota in una nuova nota identica
     * @return la copia della nota
     */
    ToDoNote* clone() const override;

    /**
     * @brief getToDoList
     * @return Contenitore di obiettivi (ToDoItem) della nota
     */
    QList<ToDoItem> getToDoList() const;

    /**
     * @brief setToDoList Imposta gli obiettivi legati alla nota
     */
    void setToDoList(const QList<ToDoItem>&);

    /**
     * @brief targetCompletati
     * @return Il numero di obiettivi completati
     */
    unsigned int targetCompletati() const;
};

#endif // TODOLIST_H
