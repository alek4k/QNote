#ifndef NOTE_H
#define NOTE_H

#include "container.h"
#include "nota.h"

typedef Container<Nota> ListaNote;

class VisualizzazioneOrdinata : public ListaNote::Ricerca {
public:
    virtual ~VisualizzazioneOrdinata() = default;

    Container<const ListaNote::ConstIterator> getResults(Container<const ListaNote::ConstIterator>& risultatiDisordinati) const;
};

class RicercaTesto : public ListaNote::Ricerca {
public:
    RicercaTesto(const QString&);

    virtual ~RicercaTesto() = default;

    virtual bool operator() (const Nota& elemento) const final;

    Container<const ListaNote::ConstIterator> getResults(Container<const ListaNote::ConstIterator>& risultatiDisordinati) const;
private:
    QString text;
};

#endif // NOTE_H
