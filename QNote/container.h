#ifndef CONTAINER_H
#define CONTAINER_H

#pragma once

#include <utility>
#include <cstring>

template <typename T>
class Container {
public:
    class Iterator;
    class Ricerca;
    class Serializzazione;
    class Deserializzazione;
private:
    /**
     * @brief struttura che rappresenta un vettore e la sua dimensione
     * @struct base_vector
     */
    struct base_vector {
        /**
         * @brief base_vector costruttore
         * @param vector
         * @param size dimensione del vettore
         */
        base_vector(T** vector = nullptr, const size_t& size = 0) : vector(vector), size(size) {}

        /**
         * @brief base_vector costruttore di copia
         * @param src base_vector da clonare
         */
        base_vector(const base_vector& src) : size(src.size) {
            T** clonedVector = (size && src.vector) ? new T*[size] : nullptr;

            for (size_t i = 0; i < size; ++i)
                clonedVector[i] = (src.vector[i] != nullptr) ? src.vector[i]->clone() : nullptr;

            vector = clonedVector;
        }

        /**
         * @brief base_vector costruttore di spostamento
         * @param move base_vector da spostare
         */
        base_vector(base_vector && move) : vector(move.vector), size(move.size) {
            // Pulisco quello originale per evitare condivisione di memoria
            move.size = 0;
            move.vector = nullptr;
        }

        /**
          * @brief distruttore del vettore
          */
        ~base_vector() {
            for (size_t i = 0; i < size; ++i)
                delete vector[i];

            delete vector;
        }

        /**
         * @brief copy copia contenuto da un vettore di origine ad uno di destinazione
         * @param dest vettore di destinazione
         * @param src vettore di origine
         * @param dim dimensione da copiare
         */
        static void copy(T** dest, const T** src, size_t dim) {
            for (size_t i = 0; i < dim; ++i) {
                dest[i] = const_cast<T*>(src[i]);
            }
        }

        /**
         * @brief add aggiunge un elemento in una determinata posizione del vettore
         * @param el elemento da aggiungere
         * @param pos posizione in cui aggiungere l'elemento
         */
        void add(const T& el, size_t pos) {
            // non aggiungo posti vuoti nel vettore (al massimo aggiungo in posizione pari a size)
            pos = (pos > size) ? size : pos;

            size_t firstHalfSize = pos, secondHalfSize = size - pos;

            T** firstHalf = (firstHalfSize) ? new T*[firstHalfSize] : nullptr;
            T** secondHalf = (secondHalfSize) ? new T*[secondHalfSize] : nullptr;
            copy(firstHalf, const_cast<const T**>(vector), firstHalfSize);
            copy(secondHalf, const_cast<const T**>(&vector[firstHalfSize]), secondHalfSize);

            delete vector;
            vector = new T*[++size];

            copy(vector, const_cast<const T**>(firstHalf), firstHalfSize);
            vector[firstHalfSize] = el.clone();
            copy(&vector[firstHalfSize + 1], const_cast<const T**>(secondHalf), secondHalfSize);

            delete firstHalf;
            delete secondHalf;
        }

        /**
         * @brief remove rimuove un elemento dal vettore
         * @param it iteratore che indica l'elemento da rimuovere
         */
        void remove(const Iterator& it) {
            size_t firstHalfSize = it.index, secondHalfSize = size - it.index;

            T** firstHalf = (firstHalfSize) ? new T*[firstHalfSize] : nullptr;
            T** secondHalf = (secondHalfSize) ? new T*[secondHalfSize] : nullptr;
            copy(firstHalf, const_cast<const T**>(vector), firstHalfSize);
            copy(secondHalf, const_cast<const T**>(&vector[firstHalfSize + 1]), secondHalfSize - 1);

            delete vector;
            vector = new T*[--size];

            copy(vector, const_cast<const T**>(firstHalf), firstHalfSize);
            copy(&vector[firstHalfSize], const_cast<const T**>(secondHalf), secondHalfSize - 1);

            delete firstHalf;
            delete secondHalf;
        }

        /**
         * @brief empty
         * @return true se il vettore è vuoto, false altrimenti
         */
        bool empty() const { return !size; }

        /**
         * @brief swap scambio di contenuto (e dimensione) tra due vettori
         * @param src vettore da scambiare
         */
        void swap(base_vector& src) {
            auto tempVect = vector;
            auto tempSize = size;

            vector = src.vector;
            size = src.size;

            src.vector = tempVect;
            src.size = tempSize;
        }

        /**
         * @brief operator [] overload dell'operatore []
         * @param index indice dell'elemento a cui si vuole accedere
         * @return elemento in posizione index
         */
        T& operator[](size_t index) {
            return *(vector[index]);
        }

        T** vector;
        size_t size;
    };

    base_vector vect;

public:
    Container() = default;

    /**
     * @brief Container costruttore di spostamento
     * @param move Container da spostare e che viene poi invalidato
     */
    Container(Container<T>&& move) : vect(base_vector(std::move(move.vect))) {}

    /**
     * @brief Container costruttore di copia
     * @param src Container da clonare
     */
    Container(const Container<T>& src) : vect(src.vect) {}

    /**
     * @brief operator = overload operatore = per copia profonda
     * @param src Container da clonare
     * @return copia del Container src
     */
    Container& operator=(const Container<T>& src) {
        if (this != &src) {
            vect = base_vector(src);
        }

        return *this;
    }

    /**
     * @brief swap effettua lo scambio di contenuto tra due Container
     * @param other Container da scambiare
     */
    void swap(Container<T>& other) {
        other.vect.swap(vect);
    }

    virtual ~Container() = default;

    /**
     * @brief classe base astratta per effettuare ricerche di elementi
     */
    class Ricerca {
    public:
        Ricerca() = default;

        Ricerca(const Ricerca&) = delete;

        /**
         * @brief operator () implementa la ricerca
         * @return true se l'elemento deve essere restituito dalla ricerca, false altrimenti
         */
        virtual bool operator()(const T&) const { return true; }

        /**
         * @brief getResults implementa l'ordine dei risultati di ricerca
         * @param risultatiDisordinati Container di iteratori agli elementi restituiti dalla ricerca non ordinati
         * @return Container di iteratori agli elementi restituiti dalla ricerca e ordinati secondo necessità
         */
        virtual Container<const Iterator> getResults(Container<const Iterator>& risultatiDisordinati) const {
            return risultatiDisordinati;
        }
    };

    /**
     * @brief simpleSearch cerca elementi nel container
     * @param criterioRicerca criterio che devono rispettare gli elementi per figurare tra i risultati di ricerca
     * @return Container di iteratori agli elementi corrispondenti alla ricerca effettuata
     */
    Container<const Iterator> simpleSearch(const Ricerca& criterioRicerca) {
        Container<const Iterator> risultatiInDisordine;
        for (auto it = begin(); it != end(); ++it)
        {
            if (criterioRicerca(*it)) {
                risultatiInDisordine.push_back(it);
            }
        }

        return criterioRicerca.getResults(risultatiInDisordine);
    }

    /**
     * @brief classe base astratta per serializzare un elemento
     */
    class Serializzazione {
    public:
        virtual ~Serializzazione() = default;
        virtual void operator() (const T& elemento) = 0;
    };

    /**
     * @brief classe base astratta per deserializzare un elemento
     */
    class Deserializzazione {
    public:
        virtual ~Deserializzazione() = default;
        virtual void operator()(Container<T>& risultato) = 0;
    };

    /**
     * @brief serializza Serializza il Container
     * @param cs modalità di serializzazione
     */
    void serializza(Serializzazione& cs) {
        for (auto it = begin(); it != end(); ++it)
            cs(*it);
    }

    /**
     * @brief deserializza Deserializza un Container
     * @param cd modalità di deserializzazione
     * @return Container deserializzato di elementi di tipo T
     */
    static Container<T> deserializza(Deserializzazione& cd) {
        Container<T> risultato;
        cd(risultato);
        return risultato;
    }

    /**
     * @brief applica iteratori sul Container offrendo accesso e modifica degli elementi
     */
    class Iterator {
        friend class Container;
    private:
        //Iterator mantiene un puntatore al vettore del Container
        Container<T>::base_vector* vect;
        //indice del vettore a cui punta l'iteratore
        size_t index;

        Iterator(Container<T>::base_vector* vect, size_t index) : vect(vect), index(index) {}

    public:
        Iterator(const Iterator& it) = default;

        virtual ~Iterator() = default;

        Iterator& operator = (const Iterator& it) = default;

        //incremento l'indice per avanzare nel vettore
        Iterator& operator ++() {
            ++index;

            return *this;
        }

        bool operator == (const Iterator& it) const {
            return (it.vect == vect) && (it.index == index);
        }

        bool operator != (const Iterator& it) const {
            return !Iterator::operator==(it);
        }

        //accedo all'elemento del vettore in posizione corrispondente all'indice dell'iteratore
        T& operator * () const {
            return (*vect)[index];
        }

        T* operator->() const {
            return &((*vect)[index]);
        }

        Iterator* clone() const {
            return new Iterator(*this);
        }
    };

    T& operator[](size_t index) {
        return vect[index];
    }

    /**
     * @brief empty
     * @return true se il Container è vuoto, false altrimenti
     */
    bool empty() const { return vect.empty(); }

    /**
     * @brief push_front Inserisce un elemento all'inizio del vettore
     * @param item elemento da inserire
     */
    void push_front(const T& item) {
        vect.add(item, 0);
    }

    /**
     * @brief push_back Inserisce un elemento alla fine del vettore
     * @param item elemento da inserire
     */
    void push_back(const T& item) {
        vect.add(item, count());
    }

    /**
     * @brief remove Rimuove un elemento dal vettore
     * @param it Iteratore che indica l'elemento da rimuovere dal Container
     */
    void remove(const Iterator& it) {
        //controllo che l'iteratore sia del container corrente
        if(it.vect == &vect)
            vect.remove(it);
    }

    /**
     * @brief count Conta gli elementi del Container
     * @return il numero di elementi nel Container
     */
    int count() const {
        return vect.size;
    }

    /**
     * @brief erase Rimuove tutti gli elementi dal Container
     */
    void erase() {
        vect = base_vector();
    }

    /**
     * @brief begin
     * @return Iterator all'inizio del vettore
     */
    Iterator begin() {
        return Iterator(&vect, 0);
    }

    /**
     * @brief end
     * @return Iterator alla fine del vettore
     */
    Iterator end() {
        return Iterator(&vect, count());
    }
};

#endif // CONTAINER_H
