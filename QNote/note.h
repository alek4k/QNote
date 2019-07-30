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

class VisualizzazioneOrdinata : public ListaNote::Ricerca {
public:
    virtual ~VisualizzazioneOrdinata() = default;

    Container<const ListaNote::ConstIterator> getResults(Container<const ListaNote::ConstIterator>& risultatiDisordinati) const override;

    Container<const ListaNote::Iterator> getResults(Container<const ListaNote::Iterator>& risultatiDisordinati) const override;
};

class RicercaTesto : public ListaNote::Ricerca {
public:
    RicercaTesto(const QString&);

    virtual ~RicercaTesto() = default;

    virtual bool operator() (const Nota& elemento) const final;

    Container<const ListaNote::ConstIterator> getResults(Container<const ListaNote::ConstIterator>& risultatiDisordinati) const override;

    Container<const ListaNote::Iterator> getResults(Container<const ListaNote::Iterator>& risultatiDisordinati) const override;
private:
    QString text;
};

class SerializeException : public std::runtime_error {
public:
    SerializeException(const std::string& msg) noexcept;
};

class DeserializeException : public std::runtime_error {
public:
    DeserializeException(const std::string& msg) noexcept;
};

class SerializzaNote : public ListaNote::Serializzazione {
public:
    SerializzaNote(const QString& path);
    ~SerializzaNote() override;
    void operator()(const Nota& nota) override;
private:
    QFile file;
    QJsonArray elementi;
};

class DeserializzaNote : public ListaNote::Deserializzazione {
public:
    DeserializzaNote(const QString& path);
    ~DeserializzaNote() override = default;
    void operator()(ListaNote& risultato) override;
private:
    QFile file;
    bool fileExists;
    QString fileContent;
    QJsonDocument doc;
};

#endif // NOTE_H
