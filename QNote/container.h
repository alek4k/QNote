#ifndef CONTAINER_H
#define CONTAINER_H

#pragma once

template <typename T>
class Container {
public:
    class Iterator;
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

    Nodo* first, *last;
public:
    class Iterator {
        friend class Container;
    private:
        Nodo* nodo;

        Iterator(Nodo* const nodo = nullptr) noexcept
            : nodo(nodo) {}

        void destroy() const noexcept {
            if (!nodo)
                return;
            if (nodo->previous)
                nodo->previous->next = nodo->next;
            if (nodo->next)
                nodo->next->previous = nodo->previous;
            delete nodo;
        }
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
            return nodo == (*it)->nodo;
        }

        bool operator != (const Iterator& it) const noexcept {
            return nodo != (*it)->nodo;
        }

        T& operator * () const noexcept {
            return *(nodo->info);
        }

        T* operator->() const noexcept {
            return nodo->info;
        }
    };

    Container() noexcept : first(nullptr), last(nullptr) {}
    Container(const Container<T>&) noexcept = delete;

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
        if (!last)
            last = first;
    }

    void push_back(const T& item) noexcept {
        push_back(new T(item));
    }

    void push_back(T* item) {
        last = new Nodo(item, last);
        if (!first)
            first = last;
    }

    void move_front(const Iterator& it) noexcept {
        if (!it.nodo || it == begin()) return;

        it.nodo->previous->next = it.nodo->next;
        if (it == end()) {
            last = it.nodo->previous;
        }
        else {
            it.nodo->next->previous = it.nodo->previous;
        }
        first->previous = it.nodo;
        it.nodo->next = first;
        it.nodo->previous = nullptr;
        first = it.nodo;
    }

    void remove(const Iterator& it) noexcept {
        if (it.nodo == first)
            first = first->next;
        else if (it.nodo == last)
            last = last->previous;
        it.destroy();
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
        return Iterator(last);
    }
};

#endif // CONTAINER_H
