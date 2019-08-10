#ifndef CONTAINER_H
#define CONTAINER_H

#pragma once

#include <utility>

template <typename T>
class Container {
public:
    class Iterator;
    class ConstIterator;
    class Ricerca;
    class Serializzazione;
    class Deserializzazione;
private:
    class Nodo {
        T* info;
        Nodo* previous, *next;
        Nodo(T* info, Nodo* prev = nullptr, Nodo* next = nullptr) noexcept
            : info(info), previous(prev), next(next) {
            if (prev) {
                prev->next = this;
            }
            if (next) {
                next->previous = this;
            }
        }

        Nodo(const Nodo&) = delete;

        Nodo& operator=(const Nodo&) = delete;

        ~Nodo() {
            delete info;
        }

        friend class Container;
        friend class Iterator;
    };

    static Nodo* clone(Nodo* src) noexcept {
        Nodo* first = src;
        if (!first) return nullptr;

        while(first->previous) {
            first = first->previous;
        }

        Nodo* lista = nullptr;
        Nodo* result = first;
        while (first) {
            Nodo* temp = new Nodo(first->info->clone(), lista, nullptr);

            if(!lista)
                lista = temp;
            else
                lista->next = temp;

            lista = lista->next;
            first = first->next;
        }

         return result;
    }

    Nodo* first;
public:
    class Ricerca {
    public:
        Ricerca() = default;

        Ricerca(const Ricerca&) = delete;

        // Implementa la ricerca (un risultato è presente? SI/NO)
        virtual bool operator()(const T&) const { return true; }

        // Implementa l'ordine dei risultati di ricerca
        virtual Container<const Iterator> getResults(Container<const Iterator> &/*&*/ risultatiDisordinati) const {
            auto daRiordinare = Container<const Iterator>(
                        /*std::move(*/risultatiDisordinati/*)*/);
            //return daRiordinare;
            return risultatiDisordinati;
        }
    };

    Container<const Iterator> simpleSearch(const Ricerca& cr) {
        Container<const Iterator> risultatiInDisordine;
        for (auto it = begin(); it != end(); ++it) // ci sono elementi
        {
            if (cr(*it))
                risultatiInDisordine.push_back(it.clone());
        }

        //return risultatiInDisordine;
        return cr.getResults(/*std::move(*/risultatiInDisordine/*)*/);
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
        friend class Nodo;
    private:
        Nodo* nodo;

        Iterator(Nodo* nodo = nullptr) noexcept
            : nodo(nodo) {}
    public:
        Iterator(const Iterator& it) noexcept
            : nodo(it.nodo) {}

        virtual ~Iterator() = default;

        Iterator& operator = (const Iterator& it) noexcept {
            if (this != &it)
                nodo = it.nodo;

            return *this;
        }

        Iterator& operator ++() noexcept {
            nodo = nodo->next;

            return *this;
        }

        bool operator == (const Iterator& it) const noexcept {
            return nodo == it.nodo;
        }

        bool operator != (const Iterator& it) const noexcept {
            return nodo != it.nodo;
        }

        T& operator * () const noexcept {
            return *(nodo->info);
        }

        T* operator->() const noexcept {
            return nodo->info;
        }

        Iterator* clone() const noexcept {
            return new Iterator(nodo);
        }
    };

    class ConstIterator {
        friend class Container;
    private:
        const Nodo* nodo;

        ConstIterator(const Nodo* const nodo = nullptr) noexcept
            : nodo(nodo) {}
    public:
        ConstIterator(const ConstIterator& it) noexcept
            : nodo(it.nodo) {}

        virtual ~ConstIterator() = default;

        ConstIterator& operator = (const ConstIterator& it) noexcept {
            if (this != &it)
                nodo = it.nodo;

            return *this;
        }

        ConstIterator& operator ++() noexcept {
            nodo = nodo->next;

            return *this;
        }

        bool operator == (const ConstIterator& it) const noexcept {
            return nodo == it.nodo;
        }

        bool operator != (const ConstIterator& it) const noexcept {
            return nodo != it.nodo;
        }

        const T& operator * () const noexcept {
            return *(nodo->info);
        }

        ConstIterator* clone() const noexcept {
            return new ConstIterator(nodo);
        }

        const T* operator->() const noexcept {
            return nodo->info;
        }

        operator bool() const {
            return nodo;
        }
    };

    Container() noexcept : first(nullptr) {}

    Container(Container<T>&& move) noexcept : first(move.first) {
        move.first = nullptr;
    }

    Container(const Container<T>& src) noexcept : first(clone(src.first)) {}

    Container& operator=(const Container<T>& src) {
        if (this != &src) {
            first = clone(src.first);
        }

        return *this;
    }

    void swap(Container<T>& other) noexcept {
        auto firstTemp = this->first;

        this->first = other.first;
        other.first = firstTemp;
    }

    virtual ~Container() {
        while(!empty()) remove(begin());
    }

    bool empty() const noexcept {
        return !first;
    }

    void push_front(const T& item) noexcept {
        push_front(new T(item));
    }

    void push_front(T* item) {
        if (!first) {
            first = new Nodo(item->clone(), nullptr, nullptr);
            return;
        }

        while (first->previous) first = first->previous;

        first = new Nodo(item, nullptr, first);
    }

    void push_back(const T& item) noexcept {
        push_back(new T(item));
    }

    void push_back(T* item) {
        if (!first) {
            push_front(item);
            return;
        }

        Nodo* ultimo = first;
        while (ultimo->next) ultimo = ultimo->next;
        ultimo->next = new Nodo(item->clone(), ultimo, nullptr);
    }

    int count() const {
        Nodo* temp = first;
        int result = 0;
        while (temp) {
            result++;
            temp = temp->next;
        }
        return result;
    }

    void insert(const Iterator& it, T* item) noexcept {
        if (it.nodo == first) {
            push_front(item);
            return;
        }

        if (!it.nodo) {
            push_back(item);
            return;
        }

        new Nodo(item, it.nodo->previous, it.nodo);
    }

    void remove(const Iterator& it) noexcept {
        if (!it.nodo) return;

        auto toRemove = it.nodo;
        if (toRemove == first) {
            first = first->next;
            if (first) first->previous = toRemove->previous;
            return;
        } else {
            if (toRemove->next) {
                if (toRemove->previous) toRemove->previous->next = toRemove->next;
                if (toRemove->next) toRemove->next->previous = toRemove->previous;
            }
        }

        delete toRemove;
    }

    //brasa tutto
    void erase() noexcept {
        while(first)
            remove(begin());
    }

    Iterator begin() noexcept {
        return Iterator(first);
    }

    Iterator end() noexcept {
        return Iterator(nullptr);
    }

    ConstIterator cbegin() const noexcept {
        return ConstIterator(first);
    }

    ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }
};

#endif // CONTAINER_H
