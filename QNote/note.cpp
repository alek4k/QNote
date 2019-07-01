#include "note.h"

/*Container<ListaNote::Iterator> VisualizzazioneOrdinata::getResults(Container<ListaNote::Iterator>& risultatiDisordinati) const {
    return risultatiDisordinati;
}
*/

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
