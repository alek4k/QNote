#ifndef QONTAINER_H
#define QONTAINER_H

#pragma once

#include <typeinfo>
#include <utility> // Usato per std::move
#include <cstddef>
#include <cinttypes>

template <class T>
/**
 * @brief Definisce un generico puntatore a T
 */
class Pointer {
public:
    Pointer(T* const = nullptr) noexcept;
    Pointer(const Pointer<T>&) = delete;
    Pointer(Pointer<T>&&) noexcept;
    Pointer<T>& operator=(const Pointer<T>&) noexcept;
    virtual Pointer<T>& operator=(T* const) noexcept;
    virtual ~Pointer();
    T& operator*() noexcept;
    const T& operator*() const noexcept;
    T* operator->() noexcept;
    const T* operator->() const noexcept;
    operator bool() const noexcept;
    void pulisci() noexcept;

private:
    T* ptr;
};

template <typename T>
Pointer<T>::Pointer(T* const ptr) noexcept
    : ptr(ptr) {}

template <typename T>
Pointer<T>::Pointer(Pointer<T>&& move) noexcept
    : ptr(move.ptr) {
    move.ptr = nullptr;
}

template <typename T>
Pointer<T>& Pointer<T>::operator=(T* const src) noexcept {
    ptr = src;

    return *this;
}

template <typename T>
Pointer<T>& Pointer<T>::operator=(const Pointer<T>& src) noexcept {
    ptr = src.ptr;

    return *this;
}

template <typename T>
Pointer<T>::~Pointer() {}

template <typename T>
T& Pointer<T>::operator*() noexcept {
    return *ptr;
}

template <typename T>
const T& Pointer<T>::operator*() const noexcept {
    return *ptr;
}

template <typename T>
T* Pointer<T>::operator->() noexcept {
    return ptr;
}

template <typename T>
const T* Pointer<T>::operator->() const noexcept {
    return ptr;
}

template <typename T>
Pointer<T>::operator bool() const noexcept {
    return ptr != nullptr;
}

template <typename T>
void Pointer<T>::pulisci() noexcept {
    delete ptr;
    ptr = nullptr;
}

template <class T, bool is_copy_constructible = std::is_copy_constructible<T>::value>
class DeepPtr : public Pointer<T> {
public:
    DeepPtr(const T&) noexcept;
    DeepPtr(const DeepPtr<T, is_copy_constructible>&) noexcept;

    DeepPtr(T * const = nullptr) noexcept;
    DeepPtr(DeepPtr<T, is_copy_constructible>&&) noexcept;

    DeepPtr<T, is_copy_constructible>& operator=(const DeepPtr<T, is_copy_constructible>&) noexcept;
    DeepPtr<T, is_copy_constructible>& operator=(T* const) noexcept override;

    virtual ~DeepPtr();
};

template <class T>
class DeepPtr<T, false> : public Pointer<T> {
public:
    DeepPtr(const T&) noexcept;
    DeepPtr(const DeepPtr<T, false>&) noexcept;

    DeepPtr(T * const = nullptr) noexcept;
    DeepPtr(DeepPtr<T, false>&&) noexcept;

    DeepPtr<T, false>& operator=(const DeepPtr<T, false>&) noexcept;
    DeepPtr<T, false>& operator=(T* const) noexcept override;

    virtual ~DeepPtr();
};

// ================= Definizione del puntatore con gestione pronda della memoria, nel caso di default =============== //

template <typename T, bool is_copy_constructible>
DeepPtr<T, is_copy_constructible>::~DeepPtr() {
    Pointer<T>::pulisci();
}

template <typename T, bool is_copy_constructible>
DeepPtr<T, is_copy_constructible>::DeepPtr(const T& src) noexcept
    : Pointer<T>(new T(src)) {}

template <typename T, bool is_copy_constructible>
DeepPtr<T, is_copy_constructible>::DeepPtr(const DeepPtr<T, is_copy_constructible>& src) noexcept
    : Pointer<T>(new T(*src)) {}

template <typename T, bool is_copy_constructible>
DeepPtr<T, is_copy_constructible>::DeepPtr(DeepPtr<T, is_copy_constructible>&& move) noexcept
    : Pointer<T>(std::move(move)) {}

template <typename T, bool is_copy_constructible>
DeepPtr<T, is_copy_constructible>::DeepPtr(T * const ptr) noexcept
    : Pointer<T>(ptr) {}

template <typename T, bool is_copy_constructible>
DeepPtr<T, is_copy_constructible>& DeepPtr<T, is_copy_constructible>::operator=(const DeepPtr<T, is_copy_constructible>& src) noexcept {
    if (&src != this) {
        Pointer<T>::pulisci();
        Pointer<T>::operator=(src ? new T(*src) : nullptr);
    }

    return *this;
}

template <typename T, bool is_copy_constructible>
DeepPtr<T, is_copy_constructible>& DeepPtr<T, is_copy_constructible>::operator=(T* const src) noexcept {
    if (this->operator->() != src) {
        Pointer<T>::pulisci();
        Pointer<T>::operator=(src ? new T(*src) : nullptr);
    }

    return *this;
}

// ================= Definizione del puntatore con gestione pronda della memoria, nel caso in cui T non sia costruibile di copia =============== //

template <typename T>
DeepPtr<T, false>::~DeepPtr() {
    Pointer<T>::pulisci();
}

template <typename T>
DeepPtr<T, false>::DeepPtr(const T& src) noexcept
    : Pointer<T>(src.clone()) {}

template <typename T>
DeepPtr<T, false>::DeepPtr(const DeepPtr<T, false>& src) noexcept
    : Pointer<T>(src.clone()) {}

template <typename T>
DeepPtr<T, false>::DeepPtr(DeepPtr<T, false>&& move) noexcept
    : Pointer<T>(std::move(move)) {}

template <typename T>
DeepPtr<T, false>::DeepPtr(T * const ptr) noexcept
    : Pointer<T>(ptr) {}

template <typename T>
DeepPtr<T, false>& DeepPtr<T, false>::operator=(const DeepPtr<T, false>& src) noexcept {
    if (&src != this) {
        Pointer<T>::pulisci();
        Pointer<T>::operator=(src ? src->clone() : nullptr);
    }

    return *this;
}

template <typename T>
DeepPtr<T, false>& DeepPtr<T, false>::operator=(T* const src) noexcept {
    if (this->operator->() != src) {
        Pointer<T>::pulisci();
        Pointer<T>::operator=(src ? src->clone() : nullptr);
    }

    return *this;
}

template <class T>
/**
 * @brief Il contenitore principale del progetto, implementato come una lista, con supporto a ricerca, serializzazione e deserializzazione
 */
class Container {
private:
    class Nodo {
        friend class Container<T>;
    public:
        Nodo(const DeepPtr<T>& data, Nodo* next = nullptr) noexcept;
        Nodo(DeepPtr<T>&& move, Nodo* next = nullptr) noexcept;
        Nodo(const Nodo&) noexcept = default;
        virtual ~Nodo() = default;

    private:
        DeepPtr<T> info;
        Nodo* next;
    };

    Nodo* nodi;
    static Nodo* copia(const Nodo * const) noexcept;
    static void distruggi(Nodo*&) noexcept;
    static void aggiungi(Nodo*&, const T&) noexcept;

    class IteratorCostante {
        friend class Container<T>;

        const Nodo* const* nodo;
        IteratorCostante(const Nodo* const* const) noexcept;

    public:
        IteratorCostante(const IteratorCostante& src) noexcept = default;
        IteratorCostante& operator=(const IteratorCostante& src) noexcept;
        virtual ~IteratorCostante() = default;

        IteratorCostante& operator++() noexcept;

        bool operator==(const IteratorCostante&) const noexcept;
        bool operator!=(const IteratorCostante&) const noexcept;

        const T& operator*() const noexcept;
        const T* operator->() const noexcept;

        bool isValid() const noexcept {return true;}
    };

public:
    /**
     * @brief Permette l'iterazione sugli elementi del contenitore con possibilità di modifica degli elementi iterati
     */
    class Iterator {
        friend class Container<T>;

        Nodo** nodo;
        Iterator(Nodo** const) noexcept;

    public:
        Iterator(Nodo* corrente = nullptr) noexcept;
        Iterator(const Iterator&) noexcept = default;
        Iterator& operator=(const Iterator&) noexcept;
        virtual ~Iterator() = default;

        Iterator& operator++() noexcept;

        bool operator==(const Iterator&) const noexcept;
        bool operator!=(const Iterator&) const noexcept;

        T& operator*() const noexcept;
        T* operator->() const noexcept;

        bool isValid() const noexcept {return true;}
    };

    /**
     * @brief Una classe base che va estesa al fine di dichiarare un criterio di ricerca su un singolo elemento
     */
    class Ricerca {
    public:
        virtual ~Ricerca() = default;
        virtual bool operator()(const T& elementoCorrente) const {return true;}
        virtual Container<const Container<T>::Iterator> getResults(Container<const Container<T>::Iterator> & risultatiDisordinati) const {
            return risultatiDisordinati;
        }
    };

    /**
     * @brief Effettua una ricerca su tutti gli elementi del contenitore
     *
     * Effettua una ricerca su tutti gli elementi del contenitore ritornando
     * il primo elemento che verifica il criterio di ricerca.
     *
     * @param criterio un funtore che viene invocato per ogni elemento e che ritorna TRUE se il criterio è soddisfatto
     * @return L'Iterator all'oggetto che verifica il criterio di ricerca, o cend() in caso la ricerca fallisca
     */
    const IteratorCostante cerca(const Ricerca& criterio) const noexcept;

    /**
     * @brief Effettua una ricerca su un range di elementi specificato
     * @param inizio l'Iterator al primo elemento da controllare
     * @param fine l'Iterator all'elemento successivo all'ultimo elemento da controllare
     * @param criterio il criterio di ricerca
     * @return L'Iterator all'oggetto che verifica il criterio di ricerca, o fine in caso la ricerca fallisca
     */
    static const IteratorCostante cerca(
                const IteratorCostante& inizio,
                const IteratorCostante& fine,
                const Ricerca& criterio
            ) noexcept;

    /**
     * @brief Effettua una ricerca completa su un range di elementi specificato
     * @param inizio l'Iterator al primo elemento da controllare
     * @param fine l'Iterator all'elemento successivo all'ultimo elemento da controllare
     * @param criterio il criterio di ricerca
     * @return un contenitore di oggetti che soddisfano il criterio di ricerca
     */
    static Container<T> cercaTutti(
                const IteratorCostante& inizio,
                const IteratorCostante& fine,
                const Ricerca& criterio
            ) noexcept;

    /**
     * @brief Effettua una ricerca su un range di elementi specificato
     * @param inizio l'Iterator (non costante) al primo elemento da controllare
     * @param fine l'Iterator (non costante) all'elemento successivo all'ultimo elemento da controllare
     * @param criterio un funtore che viene invocato per ogni elemento e che ritorna TRUE se il criterio è soddisfatto
     * @return L'Iterator (non costante) all'oggetto che verifica il criterio di ricerca, o fine in caso la ricerca fallisca
     */
    static const Iterator cerca(
            const Iterator& inizio, const Iterator& fine, const Ricerca& criterio) noexcept;

    /**
     * @brief Una classe base che va estesa al fine di dichiarare la serializzazione di un singolo elemento
     */
    class Serializzazione {
    public:
        virtual ~Serializzazione() = default;
        virtual void operator()(const T& elementoCorrente) = 0;
    };

    /**
     * @brief Serializza il container di invocazione come definito nell'algoritmo passato
     * @param serializzatore l'algoritmo da utilizzare nella serializzazione
     */
    void serializza(Serializzazione& serializzatore) const;

    /**
     * @brief Una classe base che va estesa al fine di dichiarare la deserializzazione di un intero contenitore
     */
    class Deserializzazione {
    public:
        virtual ~Deserializzazione() = default;
        virtual void operator()(Container<T>& risultato) = 0;
    };

    /**
     * @brief Deserializza un contenitore come definito nell'algoritmo di deserializzazione
     * @param deserializzatore l'algoritmo da utilizzare nella deserializzazione
     * @return il contenitore deserializzato
     */
    static Container<T> deserializza(Deserializzazione& deserializzatore);

    /**
     * @brief Inizializza una lista vuota di eventi
     */
    Container() noexcept;

    /**
     * @brief Copia il contenitore evitando ogni interferenza di memoria
     */
    Container(const Container<T>&) noexcept;

    /**
     * @brief Crea un nuovo contenitore uguale a quello passato, invalidando il precedente
     */
    Container(Container<T>&& move) noexcept;

    /**
     * @brief Copia il contenitore evitando ogni interferenza di memoria
     * @return il riferimento all'oggetto contenente la nuova copia
     */
    Container& operator=(const Container<T>&) noexcept;

    /**
     * @brief Rimuove tutti gli elementi (liberando memoria) e distrugge l'oggetto
     */
    virtual ~Container();

    /**
     * @brief Rimuove tutti gli elementi
     */
    void pulisci() noexcept;

    /**
     * @brief Testa se la lista è vuota
     * @return TRUE se la lista è vuota, altrimenti FALSE
     */
    bool vuota() const noexcept;

    /**
     * @brief Conteggia il numero di elementi presenti nel contenitore
     * @return il numero di elementi nel contenitore di invocazione
     */
    std::size_t conteggio() const noexcept;
    std::size_t count() const noexcept {return conteggio();}

    /**
     * @brief Inserisce un elemento all'interno del contenitore
     * @param elemento l'elemento da inserire
     */
    void inserisci(const T& elemento) noexcept;

    /**
     * @brief Inserisce un elemento all'interno del contenitore nella posizione specificata
     * @param elemento l'elemento da inserire
     * @param posizione la posizione in cui inserire il nuovo elemento
     */
    void inserisci(const T& elemento, const Container<T>::Iterator& posizione) noexcept;
    void push_front(T* elemento) noexcept {push_front(*elemento);}
    void push_front(const T& elemento) noexcept {inserisci(elemento, begin());}
    void push_back(T* elemento) noexcept {push_back(*elemento);}
    void push_back(const T& elemento) noexcept {inserisci(elemento, end());}

    bool empty() const noexcept {return this->vuota();}

    /**
     * @brief Rimuove un elemento parte del contenitore
     * @param posizione l'Iterator all'elemento da rimuovere
     */
    void rimuovi(Container<T>::Iterator& posizione) noexcept;

    void remove(const Container<T>::Iterator& posizione) noexcept  {
        // Salvo il puntatore all'elemento corrente (ovvero quello da cancellare)
        Container<T>::Nodo* daRimuovere = *(posizione.nodo);

        // Connetto il next dell'elemento precedente al next dell'elemento corrente (ovvero quello da cancellare)
        *(posizione.nodo) = (*(posizione.nodo))->next;

        delete daRimuovere;
    }

    /**
     * @brief Esegue la comparazione di due contenitori
     * @param cmp il contenitore a destra dell'operatore di confronto ==
     * @return TRUE se e solo se gli elementi del primo contenitore sono uguali agli elementi del secondo
     */
    bool operator==(const Container<T>& cmp) const noexcept;

    /**
     * @brief Esegue la comparazione di due contenitori
     * @param cmp il contenitore a destra dell'operatore di confronto !=
     * @return TRUE se e solo se gli elementi del primo contenitore NON sono uguali agli elementi del secondo
     */
    bool operator!=(const Container<T>& cmp) const noexcept;

    /**
     * @brief Ritorna un Iterator costante al primo elemento della lista
     * @return L'Iterator costante al primo elemento della lista
     */
    IteratorCostante cbegin() const noexcept;

    /**
     * @brief Ritorna un Iterator costante all'elemento past-the-end della lista
     * @return L'Iterator costante all'elemento past-the-end della lista
     */
    IteratorCostante cend() const noexcept;

    /**
     * @brief Ritorna un Iterator NON costante al primo elemento della lista
     * @return L'Iterator NON costante al primo elemento della lista
     */
    Iterator begin() noexcept;

    /**
     * @brief Ritorna un Iterator NON costante all'elemento past-the-end della lista
     * @return L'Iterator NON costante all'elemento past-the-end della lista
     */
    Iterator end() noexcept;

    /**
     * @brief Scambia il contenuto dei due contenitori
     * @param other il contenitore con cui scambiare il contenuto
     */
    void swap(Container<T>& other) noexcept;

    Container<const Iterator> simpleSearch(const Ricerca& cr) {
            Container<const Iterator> risultatiInDisordine;
            for (auto it = begin(); it != end(); ++it) // ci sono elementi
            {
                if (cr(*it))
                    risultatiInDisordine.push_back(it);
            }

            //return risultatiInDisordine;
            return cr.getResults(/*std::move(*/risultatiInDisordine/*)*/);
        }
};

template <typename T>
Container<T>::Nodo::Nodo(const DeepPtr<T>& info, Nodo* next) noexcept : info(info), next(next) {}

template <typename T>
Container<T>::Nodo::Nodo(DeepPtr<T>&& src, Nodo* next) noexcept
    : info(std::move(src)), next(next) {}

template <typename T>
Container<T>::Container() noexcept
    : nodi(nullptr) {}

template <typename T>
Container<T>::Container(const Container<T>& src) noexcept
    : nodi(copia(src.nodi)) {}

template <typename T>
Container<T>::Container(Container<T>&& move) noexcept
    : nodi(move.nodi) {
    move.nodi = nullptr;
}

template <typename T>
void Container<T>::swap(Container<T>& other) noexcept {
    auto contenuto_altro = other.nodi;

    other.nodi = nodi;
    nodi = contenuto_altro;
}

template <typename T>
Container<T>& Container<T>::operator=(const Container<T>& src) noexcept {
    if (&src != this) {
        distruggi(nodi);
        nodi = copia(src.nodi);
    }

    return *this;
}

template <typename T>
bool Container<T>::operator==(const Container<T>& cmp) const noexcept {
    auto cit_rhs = cmp.cbegin();
    auto cit_lhs = this->cbegin();

    while ((cit_rhs != cmp.cend()) && (cit_lhs != this->cend())) {
        const bool stessoTipo = typeid(*cit_lhs) == typeid(*cit_rhs);
        const bool uguaglianza = *cit_lhs == *cit_rhs;

        if ((!stessoTipo) || (!uguaglianza)) return false;
    }

    // Non c'è una lista con un elemento in più
    return (cit_rhs == cmp.cend()) && (cit_lhs == this->cend());
}

template <typename T>
bool Container<T>::operator!=(const Container<T>& cmp) const noexcept {
    return !this->operator==(cmp);
}

template <typename T>
Container<T>::~Container() {
    distruggi(nodi);
}

template <typename T>
typename Container<T>::IteratorCostante Container<T>::cbegin() const noexcept {
    return Container<T>::IteratorCostante(&nodi);
}

template <typename T>
typename Container<T>::IteratorCostante Container<T>::cend() const noexcept {
    Nodo* const* ultimoNodo = &nodi;

    // Questo evita figuracce nel caso di lista vuota...
    if (!*ultimoNodo) return ultimoNodo;

    // Trovo un riferimoento al puntatore all'ultimo nodo della lista
    while ((*ultimoNodo)->next)
        ultimoNodo = &(*ultimoNodo)->next;

    return &((*ultimoNodo)->next);
}

template <typename T>
typename Container<T>::Iterator Container<T>::begin() noexcept {
    return Container<T>::Iterator(&nodi);
}

template <typename T>
typename Container<T>::Iterator Container<T>::end() noexcept {
    Nodo** ultimoNodo = &nodi;

    // Questo evita figuracce nel caso di lista vuota...
    if (!*ultimoNodo) return ultimoNodo;

    // Trovo un riferimoento al puntatore all'ultimo nodo della lista
    while ((*ultimoNodo)->next)
        ultimoNodo = &(*ultimoNodo)->next;

    return &((*ultimoNodo)->next);
}

template <typename T>
std::size_t Container<T>::conteggio() const noexcept {
    std::size_t numero(0);

    auto contatore(cbegin());

    while (contatore != cend()) {
        ++contatore;
        ++numero;
    }

    return numero;
}

template <typename T>
const typename Container<T>::IteratorCostante Container<T>::cerca(
        const Container<T>::IteratorCostante& inizio, const Container<T>::IteratorCostante& fine,
        const Container<T>::Ricerca& criterio) noexcept {
    auto it = inizio;

    while (it != fine) {
        if (criterio(*it)) return it;
        ++it;
    }

    return fine;
}

template <typename T>
const typename Container<T>::Iterator Container<T>::cerca(
        const Container<T>::Iterator& inizio, const Container<T>::Iterator& fine,
        const Container<T>::Ricerca& criterio) noexcept {
    auto it = inizio;

    while (it != fine) {
        if (criterio(*it)) return it;
        ++it;
    }

    return fine;
}

template <typename T>
const typename Container<T>::IteratorCostante Container<T>::cerca(const Container<T>::Ricerca& criterio) const noexcept {
    return Container<T>::cerca(cbegin(), cend(), criterio);
}

template <typename T>
void Container<T>::pulisci() noexcept {
    distruggi(nodi);
}

template <typename T>
bool Container<T>::vuota() const noexcept {
    return cbegin() == cend();
}

template <typename T>
void Container<T>::inserisci(const T& elemento) noexcept {
    Container<T>::aggiungi(nodi, elemento);
}

template <typename T>
void Container<T>::inserisci(const T& elemento, const typename Container<T>::Iterator& posizione) noexcept {
    *(posizione.nodo) = new typename Container<T>::Nodo(std::move(DeepPtr<T>(elemento)), *posizione.nodo);
}

template <typename T>
void Container<T>::rimuovi(Container<T>::Iterator& posizione) noexcept {
    // Salvo il puntatore all'elemento corrente (ovvero quello da cancellare)
    Container<T>::Nodo* daRimuovere = *(posizione.nodo);

    // Connetto il next dell'elemento precedente al next dell'elemento corrente (ovvero quello da cancellare)
    *(posizione.nodo) = (*(posizione.nodo))->next;

    delete daRimuovere;
}

template <typename T>
typename Container<T>::Nodo* Container<T>::copia(const Container<T>::Nodo * const lista) noexcept {
    if (!lista) return nullptr;

    return new Container<T>::Nodo(*(lista->info), copia(lista->next));
}

template <typename T>
void Container<T>::distruggi(Nodo*& lista) noexcept {
    if (!lista) return;

    if (lista->next) distruggi(lista->next);
    delete lista;
    lista = nullptr;
}

template <typename T>
void Container<T>::aggiungi(Nodo*& lista, const T& data) noexcept {
    // Effettuo l'aggiunta nel prossimo nodo se il presente NON è nullo
    if (lista) return aggiungi(lista->next, data);

    // Creo un nuovo nodo e lo aggiungo in testa alla lista
    lista = new Nodo(DeepPtr<T>(data));
}

template <typename T>
Container<T> Container<T>::cercaTutti(
                const Container<T>::IteratorCostante& inizio,
                const Container<T>::IteratorCostante& fine,
                const Container<T>::Ricerca& criterio
        ) noexcept {
    Container<T> risultato;

    auto it = inizio;

    while (it != fine)
        if (criterio(*it)) risultato.inserisci(*it);
}

template <typename T>
void Container<T>::serializza(Container<T>::Serializzazione& serializzatore) const {
    for (auto it = cbegin(); it != cend(); ++it)
        serializzatore(*it);
}

template <typename T>
Container<T> Container<T>::deserializza(Container<T>::Deserializzazione& deserializzatore) {
    Container<T> risultato;

    // sbologno il lavoro al deserializzatore :)
    deserializzatore(risultato);

    return risultato;
}

// =============================================================================================
// =                                         Iteratori                                         =
// =============================================================================================

template <typename T>
Container<T>::IteratorCostante::IteratorCostante(const Container<T>::Nodo* const* const nodo) noexcept
    : nodo(nodo) {}

template <typename T>
typename Container<T>::IteratorCostante& Container<T>::IteratorCostante::operator=(const Container<T>::IteratorCostante& src) noexcept {
    if (this != &src) {
        nodo = src.nodo;
    }

    return *this;
}

template <typename T>
bool Container<T>::IteratorCostante::operator==(const Container<T>::IteratorCostante& cmp) const noexcept {
    return cmp.nodo == nodo;
}

template <typename T>
bool Container<T>::IteratorCostante::operator!=(const Container<T>::IteratorCostante& cmp) const noexcept {
    return !(this->operator==(cmp));
}

template <typename T>
const T& Container<T>::IteratorCostante::operator*() const noexcept {
    return *(*nodo)->info;
}

template <typename T>
const T* Container<T>::IteratorCostante::operator->() const noexcept {
    return &(*(*nodo)->info);
}

template <typename T>
typename Container<T>::IteratorCostante& Container<T>::IteratorCostante::operator++() noexcept {
    nodo = &((*nodo)->next);

    return *this;
}

template <typename T>
Container<T>::Iterator::Iterator(Container<T>::Nodo** nodo) noexcept
    : nodo(nodo) {}

template <typename T>
typename Container<T>::Iterator& Container<T>::Iterator::operator=(const Container<T>::Iterator& src) noexcept {
    if (this != &src) {
        nodo = src.nodo;
    }

    return *this;
}

template <typename T>
bool Container<T>::Iterator::operator==(const Container<T>::Iterator& cmp) const noexcept {
    return cmp.nodo == nodo;
}

template <typename T>
bool Container<T>::Iterator::operator!=(const Container<T>::Iterator& cmp) const noexcept {
    return !(this->operator==(cmp));
}

template <typename T>
T& Container<T>::Iterator::operator*() const noexcept {
    return *(*nodo)->info;
}

template <typename T>
T* Container<T>::Iterator::operator->() const noexcept {
    return &(*(*nodo)->info);
}

template <typename T>
typename Container<T>::Iterator& Container<T>::Iterator::operator++() noexcept {
    nodo = &((*nodo)->next);

    return *this;
}

#endif // QONTAINER_H
