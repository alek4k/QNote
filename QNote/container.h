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
    struct base_vector {
        base_vector(T** vector = nullptr, const size_t& size = 0) : vector(vector), size(size) {}

        base_vector(const base_vector& src) noexcept : size(src.size) {
            T** clonedVector = (size && src.vector) ? new T*[size] : nullptr;

            for (size_t i = 0; i < size; ++i)
                clonedVector[i] = (src.vector[i] != nullptr) ? src.vector[i]->clone() : nullptr;

            vector = clonedVector;
        }

        base_vector(base_vector && move) noexcept : vector(move.vector), size(move.size) {
            // Pulisco quello originale per evitare condivisione di memoria
            move.size = 0;
            move.vector = nullptr;
        }

        ~base_vector() {
            for (size_t i = 0; i < size; ++i)
                delete vector[i];

            delete vector;
        }

        static void copy(T** dest, const T** src, size_t dim) {
            for (size_t i = 0; i < dim; ++i) {
                dest[i] = const_cast<T*>(src[i]);
            }
        }

        void add(const T& el, size_t pos) {
            // non aggiungo posti vuoti nel vettore
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

        bool empty() const noexcept { return !size; }

        void swap(base_vector& src) noexcept {
            auto tempVect = vector;
            auto tempSize = size;

            vector = src.vector;
            size = src.size;

            src.vector = tempVect;
            src.size = tempSize;
        }

        T& operator[](size_t index) noexcept {
            return *(vector[index]);
        }

        T** vector;
        size_t size;
    };

    base_vector vect;

public:
    Container() = default;

    Container(Container<T>&& move) noexcept : vect(base_vector(std::move(move.vect))) {}

    Container(const Container<T>& src) noexcept : vect(src.vect) {}

    Container& operator=(const Container<T>& src) {
        if (this != &src) {
            vect = base_vector(src);
        }

        return *this;
    }

    void swap(Container<T>& other) noexcept {
        other.vect.swap(vect);
    }

    virtual ~Container() = default;

    class Ricerca {
    public:
        Ricerca() = default;

        Ricerca(const Ricerca&) = delete;

        // Implementa la ricerca (un risultato è presente? SI/NO)
        virtual bool operator()(const T&) const { return true; }

        // Implementa l'ordine dei risultati di ricerca
        virtual Container<const Iterator> getResults(Container<const Iterator> &/*&*/ risultatiDisordinati) const {
            return risultatiDisordinati;
        }
    };

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

    class Serializzazione {
    public:
        virtual ~Serializzazione() = default;
        virtual void operator() (const T& elemento) = 0;
    };

    class Deserializzazione {
    public:
        virtual ~Deserializzazione() = default;
        virtual void operator()(Container<T>& risultato) = 0;
    };

    void serializza(Serializzazione& cs) {
        for (auto it = begin(); it != end(); ++it)
            cs(*it);
    }

    static Container<T> deserializza(Deserializzazione& cd) {
        Container<T> risultato;
        cd(risultato);
        return risultato;
    }

    class Iterator {
        friend class Container;
    private:
        Container<T>::base_vector* vect;
        size_t index;

        Iterator(Container<T>::base_vector* vect, size_t index) noexcept : vect(vect), index(index) {}

    public:
        Iterator(const Iterator& it) = default;

        virtual ~Iterator() = default;

        Iterator& operator = (const Iterator& it) = default;

        Iterator& operator ++() noexcept {
            ++index;

            return *this;
        }

        bool operator == (const Iterator& it) const noexcept {
            return (it.vect == vect) && (it.index == index);
        }

        bool operator != (const Iterator& it) const noexcept {
            return !Iterator::operator==(it);
        }

        T& operator * () const noexcept {
            return (*vect)[index];
        }

        T* operator->() const noexcept {
            return &((*vect)[index]);
        }

        Iterator* clone() const noexcept {
            return new Iterator(*this);
        }
    };

    T& operator[](size_t index) noexcept {
        return vect[index];
    }

    bool empty() const { return vect.empty(); }

    void push_front(const T& item) noexcept {
        vect.add(item, 0);
    }

    void push_back(const T& item) noexcept {
        vect.add(item, count());
    }

    void remove(const Iterator& it) {
        if(it.vect == &vect)
            vect.remove(it);
    }

    int count() const {
        return vect.size;
    }

    void erase() noexcept {
        vect = base_vector();
    }

    Iterator begin() noexcept {
        return Iterator(&vect, 0);
    }

    Iterator end() noexcept {
        return Iterator(&vect, count());
    }
};

#endif // CONTAINER_H
