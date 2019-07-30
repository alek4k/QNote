#ifndef CONTAINER_H
#define CONTAINER_H

#pragma once

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

        Nodo& operator = (const Nodo&) = delete;

        ~Nodo() {
            if (info)
                delete info;
        }

        friend class Container;
        friend class Iterator;
    };

    Nodo* first;
public:
    class Ricerca {
    public:
        Ricerca() = default;

        Ricerca(const Ricerca&) = delete;

        // Implementa la ricerca (un risultato è presente? SI/NO)
        virtual bool operator()(const T&) const { return true; }

        // Implementa l'ordine dei risultati di ricerca
        virtual Container<const ConstIterator> getResults(Container<const ConstIterator>& risultatiDisordinati) const {
            return risultatiDisordinati;
        }

        virtual Container<const Iterator> getResults(Container<const Iterator>& risultatiDisordinati) const {
            return risultatiDisordinati;
        }
    };

    Container<const ConstIterator> search(const Ricerca& cr) {
        Container<const ConstIterator> risultatiInDisordine;
        for (auto it = cbegin(); it != cend(); ++it) // ci sono elementi
        {
            if (cr(*it))
                risultatiInDisordine.push_front(it);
        }

        //return cr.getResults(risultatiInDisordine);
        return risultatiInDisordine;
    }

    Container<const Iterator> simpleSearch(const Ricerca& cr) {
        Container<const Iterator> risultatiInDisordine;
        for (auto it = begin(); it != end(); ++it) // ci sono elementi
        {
            if (cr(*it))
                risultatiInDisordine.push_front(it);
        }

        //return cr.getResults(risultatiInDisordine);
        return risultatiInDisordine;
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
        Nodo* nodo;

        Iterator(Nodo* nodo = nullptr) noexcept
            : nodo(nodo) {}

        /*void destroy() const noexcept {
            if (!nodo)
                return;
            if (nodo->previous)
                nodo->previous->next = nodo->next;
            if (nodo->next)
                nodo->next->previous = nodo->previous;
            delete nodo;
        }*/
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
    };

    class ConstIterator {
        friend class Container;
    private:
        Nodo* nodo;

        ConstIterator(Nodo* nodo = nullptr) noexcept
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

        const T* operator->() const noexcept {
            return nodo->info;
        }

        operator bool() const {
            return nodo;
        }
    };

    Container() noexcept : first(nullptr) {}
    Container(const Container<T>&) noexcept = default;

    virtual ~Container() {
        while(first)
            remove(begin());
    }

    bool empty() const noexcept {
        return !first;
    }

    void push_front(const T& item) noexcept {
        push_front(new T(item));
    }

    void push_front(T* item) {
        first = new Nodo(item, nullptr, first);
    }

    void push_end(const T& item) noexcept {
        push_end(new T(item));
    }

    void push_end(T* item) {
        if (!first) push_front(item);
        else {
            Nodo* temp = first;
            while (temp->next)
                temp = temp->next;

            temp = new Nodo(item, temp, nullptr);
        }
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
            push_end(item);
            return;
        }

        Nodo* nuovo = new Nodo(item, it.nodo->previous, it.nodo);
    }

    void remove(const Iterator& it) noexcept {
        //if (!it.nodo) return;

        if (it.nodo == first) {
            Nodo* temp = first;
            first = first->next;
            if (first) {
                first->previous = nullptr;
            }
            delete temp;
            return;
        }

        if (it.nodo->previous)
            it.nodo->previous->next = it.nodo->next;
        if (it.nodo->next)
            it.nodo->next->previous = it.nodo->previous;
        delete it.nodo;
        //it.destroy();
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
        return Iterator();
    }

    ConstIterator cbegin() const noexcept {
        return ConstIterator(first);
    }

    ConstIterator cend() const noexcept {
        return ConstIterator();
    }
};

#endif // CONTAINER_H
