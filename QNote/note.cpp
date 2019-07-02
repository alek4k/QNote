#include "note.h"

//VisualizzazioneOrdinata
Container<const ListaNote::ConstIterator> VisualizzazioneOrdinata::getResults(Container<const ListaNote::ConstIterator>& risultatiDisordinati) const {
    return risultatiDisordinati;
}

Container<const ListaNote::Iterator> VisualizzazioneOrdinata::getResults(Container<const ListaNote::Iterator>& risultatiDisordinati) const {
    return risultatiDisordinati;
}

//RicercaTesto
RicercaTesto::RicercaTesto(const QString& text) : text(text) {}

bool RicercaTesto::operator() (const Nota& elemento) const {
    auto tags = elemento.getTag();
    for (auto cit = tags.cbegin(); cit != tags.cend(); ++cit)
        if (cit->compare(text) == 0) return true;

    return false;
}

Container<const ListaNote::ConstIterator> RicercaTesto::getResults(Container<const ListaNote::ConstIterator> &risultatiDisordinati) const {
    return risultatiDisordinati;
}

Container<const ListaNote::Iterator> RicercaTesto::getResults(Container<const ListaNote::Iterator>& risultatiDisordinati) const {
    return risultatiDisordinati;
}
