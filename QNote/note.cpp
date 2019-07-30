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
    risultatoSerializzazione.insert("dataUltimaModifica", nota.getDataModifica().toString());

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
            QJsonObject toDoObj;
            toDoObj.insert("target", (*it).getTarget());
            toDoObj.insert("status", *it ? true : false);
            toDoList.append(toDoObj);
        }
        risultatoSerializzazione.insert("toDoList", toDoList);
        risultatoSerializzazione.insert("tipo", "toDoNote");
    }

    elementi.push_back(risultatoSerializzazione);
}

DeserializzaNote::DeserializzaNote(const QString& path)
    : file(path), fileExists(QFileInfo::exists(path) && QFileInfo(path).isFile()) {
    if (fileExists) {
        if (!file.open(QIODevice::ReadOnly))
            throw DeserializeException("Errore nella lettura del file di salvataggio");

        // Leggo tutto il file
        fileContent = QString(file.readAll());

        // Faccio il parsing del contenuto
        doc = QJsonDocument::fromJson(fileContent.toUtf8());

        // Chiudo il file
        file.close();
    }
}

void DeserializzaNote::operator()(ListaNote& risultato) {
    // Se il file non esiste non ha molto senso cercare di estrarre dati da esso...
    if (!fileExists) return;

    if (!doc.isObject())
        throw DeserializeException("Errore nella interpretazione del file di salvataggio: formato non valido");

    // Scorro tutte le note presenti nel JSON
    const auto note = doc.object()["note"].toArray();
    for (auto it = note.cbegin(); it != note.cend(); ++it) {
        const auto risultatoSerializzazione = it->toObject();

        QDateTime data = QDateTime::fromString(risultatoSerializzazione["dataUltimaModifica"].toString());
        QString titolo = risultatoSerializzazione["titolo"].toString();
        QString descrizione = risultatoSerializzazione["descrizione"].toString();
        QString tipo = risultatoSerializzazione["tipo"].toString();
        const auto tagArray = risultatoSerializzazione["tag"].toArray();
        QVector<QString> tag;
        for(auto it = tagArray.cbegin(); it != tagArray.cend(); ++it) {
            tag.push_front((*it).toString());
        }
        if (tipo == "simpleNote") {
            risultato.push_back(new SimpleNote(titolo, descrizione, tag, data));
        } else if (tipo == "imgNote") {
            QString image = risultatoSerializzazione["image"].toString();
            risultato.push_back(new ImgNote(titolo, descrizione, tag, image, data));
        } else if (tipo == "toDoNote") {
            const auto toDoRaw = risultatoSerializzazione["toDoList"].toArray();
            QList<ToDoItem> toDoList;
            for(auto it = toDoRaw.cbegin(); it != toDoRaw.cend(); ++it) {
                QJsonObject toDoObj = (*it).toObject();
                toDoList.push_front(ToDoItem(toDoObj["target"].toString(), toDoObj["status"].toBool()));
            }
            risultato.push_back(new ToDoNote(titolo, descrizione, tag, toDoList, data));
        } else {
            throw DeserializeException("Tipologia nota non riconosciuta");
        }
    }
}

