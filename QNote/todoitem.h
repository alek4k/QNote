#ifndef TODOITEM_H
#define TODOITEM_H

#pragma once

#include <QString>

class ToDoItem
{
private:
    //nome dell'obiettivo
    QString target;
    //stato dell'obiettivo (completato? SI/NO)
    bool completato;
public:
    /**
     * @brief ToDoItem Singolo obiettivo
     * @param target Titolo dell'obiettivo
     * @param completato Stato dell'obiettivo (true = completato; di default = false)
     */
    ToDoItem(const QString& target, const bool& completato = false);

    ToDoItem(const ToDoItem&) = default;

    virtual ~ToDoItem() = default;

    /**
     * @brief updateItem Aggiorna obiettivo
     * @param target Nuovo titolo per l'obiettivo
     * @param completato Nuovo stato dell'obiettivo
     */
    void updateItem(const QString& target, const bool& completato);

    /**
     * @brief getTarget
     * @return Il titolo dell'obiettivo
     */
    QString getTarget() const;

    /**
     * @brief overload bool operator: True se l'obiettivo è stato completato, False altrimenti
     */
    operator bool () const;
};

#endif // TODOITEM_H
