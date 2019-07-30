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

SerializeException::SerializeException(const std::string& msg) noexcept
    : std::runtime_error(msg) {}

DeserializeException::DeserializeException(const std::string& msg) noexcept
    : std::runtime_error(msg) {}

SerializzaNote::SerializzaNote(const QString& path)
    : file(path) {
    if (!file.open(QIODevice::WriteOnly))
        throw SerializeException("Errore nella creazione del file di salvataggio");
}

SerializzaNote::~SerializzaNote() {
    QJsonObject serializzazione;
    serializzazione.insert("note", elementi);

    // Scrivo il json nel file
    file.write(QJsonDocument(serializzazione).toJson(QJsonDocument::Indented));

    // E chiudo il file per non fare brutte figure
    file.close();
}

void SerializzaNote::operator()(const Nota& nota) {
    QJsonObject risultatoSerializzazione;
    risultatoSerializzazione.insert("titolo", nota.getTitolo());
    risultatoSerializzazione.insert("descrizione", nota.getDescrizione());
    auto tagArray = nota.getTag();
    QJsonArray tag;
    for(auto it = tagArray.cbegin(); it != tagArray.cend(); ++it) {
        tag.append(*it);
    }
    risultatoSerializzazione.insert("tag", tag);
    risultatoSerializzazione.insert("dataUltimaModifica", nota.getDataModifica().date().toJulianDay());

    const auto simpleNote = dynamic_cast<const SimpleNote*>(&nota);
    const auto imgNote = dynamic_cast<const ImgNote*>(&nota);
    const auto toDoNote = dynamic_cast<const ToDoNote*>(&nota);

    if (simpleNote) {
        risultatoSerializzazione.insert("tipo", "simpleNote");
    }
    else if(imgNote) {
        risultatoSerializzazione.insert("image", imgNote->getImage());
        risultatoSerializzazione.insert("tipo", "imgNote");
    }
    else if(toDoNote) {
        auto toDoRaw = toDoNote->getToDoList();
        QJsonArray toDoList;
        for(auto it = toDoRaw.cbegin(); it != toDoRaw.cend(); ++it) {
            toDoList.append((*it).getTarget());
            *it ? toDoList.append(true) : toDoList.append(false);
        }
        risultatoSerializzazione.insert("toDoList", toDoList);
        risultatoSerializzazione.insert("tipo", "toDoNote");
    }

    elementi.push_back(risultatoSerializzazione);
}


