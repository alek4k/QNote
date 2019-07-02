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
    QRegExp rx("*"+text+"*");
    rx.setPatternSyntax(QRegExp::Wildcard);

    //controllo sui tag
    auto tags = elemento.getTag();
    for (auto cit = tags.cbegin(); cit != tags.cend(); ++cit) {
        //if (cit->compare(text) == 0) return true;

        if (rx.exactMatch(*cit)) return true;
    }

    //controllo sulla descrizione
    if (rx.exactMatch(elemento.getDescrizione())) return true;

    //controllo sul titolo
    if (rx.exactMatch(elemento.getTitolo())) return true;

    return false;
}

Container<const ListaNote::ConstIterator> RicercaTesto::getResults(Container<const ListaNote::ConstIterator> &risultatiDisordinati) const {
    return risultatiDisordinati;
}

Container<const ListaNote::Iterator> RicercaTesto::getResults(Container<const ListaNote::Iterator>& risultatiDisordinati) const {
    return risultatiDisordinati;
}
