#ifndef NOTE_H
#define NOTE_H

#include "container.h"
#include "nota.h"
#include "simplenote.h"
#include "todonote.h"
#include "imgnote.h"
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

typedef Container<Nota> ListaNote;

/**
 * @brief Classe derivata da Ricerca che consente di ottenere un Container con gli elementi ordinati per data di modifica decrescente
 */
class VisualizzazioneOrdinata : public ListaNote::Ricerca {
public:
    /**
     * @brief getResults Costruisce un vettore ordinato per data di modifica decrescente dei suoi elementi
     * @param risultatiDisordinati Container non ordinato
     * @return Container con risultati ordinati per data di modifica decrescente
     */
    Container<const ListaNote::Iterator> getResults(Container<const ListaNote::Iterator> & risultatiDisordinati) const override;

    virtual ~VisualizzazioneOrdinata() = default;
};

/**
 * @brief Classe derivata da Ricerca che consente di ricercare elementi nel vettore in base ad uno o più termini di ricerca
 */
class RicercaTesto : public ListaNote::Ricerca {
public:
    /**
     * @brief RicercaTesto
     * @param text Termini di ricerca da utilizzare
     */
    RicercaTesto(const QString& text);

    virtual ~RicercaTesto() = default;

    /**
     * @brief operator () Utilizzato per definire e verificare l'algoritmo di ricerca per ogni elemento
     * @param elemento in cui effettuare la ricerca
     * @return true se l'elemento soddisfa la ricerca, false altrimenti
     */
    virtual bool operator() (const Nota& elemento) const final;
private:
    //stringa da utilizzare per la ricerca tra titoli, descrizioni e tag delle note
    QString text;
};

/**
 * @brief Classe per gestire errori nel processo di serializzazione su file json
 */
class SerializeException : public std::runtime_error {
public:
    SerializeException(const std::string& msg);
};

/**
 * @brief Classe per gestire errori nel processo di deserializzazione da file json
 */
class DeserializeException : public std::runtime_error {
public:
    DeserializeException(const std::string& msg);
};

/**
 * @brief classe per serializzare note
 */
class SerializzaNote : public ListaNote::Serializzazione {
public:
    /**
     * @brief SerializzaNote costruttore
     * @param path Il percorso del file su cui salvare le note in formato json
     */
    SerializzaNote(const QString& path);
    ~SerializzaNote() override;
    /**
     * @brief operator () Tratta una singola nota e la processa ottenendone il json relativo
     * @param nota Nota da serializzare
     */
    void operator()(const Nota& nota) override;
private:
    //file di destinazione
    QFile file;
    //tutti gli elementi da scrivere su file
    QJsonArray elementi;
};

/**
 * @brief classe per deserializzare note
 */
class DeserializzaNote : public ListaNote::Deserializzazione {
public:
    /**
     * @brief DeserializzaNote Costruttore
     * @param path Il percorso del file (json) da cui importare le note
     */
    DeserializzaNote(const QString& path);
    /**
     * @brief operator () Deserializza tutte le note dal file selezionato
     * @param risultato Il container in cui importare le note
     */
    void operator()(ListaNote& risultato) override;
private:
    //file di origine
    QFile file;
    bool fileExists;
    //tutto il contenuto del file di origine
    QString fileContent;
    //contenuto json da deserializzare
    QJsonDocument doc;
};

#endif // NOTE_H
