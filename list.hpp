#pragma once
#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include <memory>
#include <cstddef>

namespace sjtu {

/**
 * @tparam T Type of the elements.
 * Be careful that T may not be default constructable.
 *
 * @brief A list that supports operations like std::list.
 */
template <typename T>
class list {
private:
    struct Node {
        alignas(T) char data_buffer[sizeof(T)];
        bool has_data;
        Node *prev;
        Node *next;

        // Constructor for sentinel node (no data)
        Node() : has_data(false), prev(nullptr), next(nullptr) {}

        // Constructor with data
        Node(const T &value, Node *p = nullptr, Node *n = nullptr)
            : has_data(true), prev(p), next(n) {
            new (data_buffer) T(value);
        }

        // Destructor
        ~Node() {
            if (has_data) {
                reinterpret_cast<T*>(data_buffer)->~T();
            }
        }

        T* data() {
            return reinterpret_cast<T*>(data_buffer);
        }

        const T* data() const {
            return reinterpret_cast<const T*>(data_buffer);
        }
    };

    Node *head;     // sentinel node
    Node *tail;     // sentinel node
    size_t count;

    void init() {
        head = new Node();
        tail = new Node();
        head->next = tail;
        tail->prev = head;
        count = 0;
    }

    void destroy() {
        clear();
        delete head;
        delete tail;
    }

public:
    class iterator;
    class const_iterator;

    /**
     * Constructs & Assignments
     */
    list() {
        init();
    }

    list(const list &other) {
        init();
        for (Node *p = other.head->next; p != other.tail; p = p->next) {
            push_back(*(p->data()));
        }
    }

    list &operator=(const list &other) {
        if (this != &other) {
            clear();
            for (Node *p = other.head->next; p != other.tail; p = p->next) {
                push_back(*(p->data()));
            }
        }
        return *this;
    }

    /* Destructor. */
    ~list() {
        destroy();
    }

    /* Access the first / last element. */
    T &front() noexcept {
        return *(head->next->data());
    }

    T &back() noexcept {
        return *(tail->prev->data());
    }

    const T &front() const noexcept {
        return *(head->next->data());
    }

    const T &back() const noexcept {
        return *(tail->prev->data());
    }

    /* Return an iterator pointing to the first element. */
    iterator begin() noexcept {
        return iterator(head->next, this);
    }

    const_iterator cbegin() const noexcept {
        return const_iterator(head->next, this);
    }

    /* Return an iterator pointing to one past the last element. */
    iterator end() noexcept {
        return iterator(tail, this);
    }

    const_iterator cend() const noexcept {
        return const_iterator(tail, this);
    }

    /* Checks whether the container is empty. */
    bool empty() const noexcept {
        return count == 0;
    }

    /* Return count of elements in the container. */
    size_t size() const noexcept {
        return count;
    }

    /* Clear the contents. */
    void clear() noexcept {
        Node *p = head->next;
        while (p != tail) {
            Node *next = p->next;
            delete p;
            p = next;
        }
        head->next = tail;
        tail->prev = head;
        count = 0;
    }

    /**
     * @brief Insert value before pos (pos may be the end() iterator).
     * @return An iterator pointing to the inserted value.
     */
    iterator insert(iterator pos, const T &value) {
        Node *curr = pos.node;
        Node *newNode = new Node(value, curr->prev, curr);
        curr->prev->next = newNode;
        curr->prev = newNode;
        count++;
        return iterator(newNode, this);
    }

    /**
     * @brief Remove the element at pos.
     * returns an iterator pointing to the following element.
     */
    iterator erase(iterator pos) noexcept {
        Node *curr = pos.node;
        Node *next = curr->next;
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
        delete curr;
        count--;
        return iterator(next, this);
    }

    /* Add an element to the front/back. */
    void push_front(const T &value) {
        insert(begin(), value);
    }

    void push_back(const T &value) {
        insert(end(), value);
    }

    /* Removes the first/last element. */
    void pop_front() noexcept {
        erase(begin());
    }

    void pop_back() noexcept {
        erase(iterator(tail->prev, this));
    }

public:
    class iterator {
    private:
        Node *node;
        const list *container;

        friend class list;
        friend class const_iterator;

        iterator(Node *n, const list *c) : node(n), container(c) {}

    public:
        iterator() : node(nullptr), container(nullptr) {}

        iterator &operator++() {
            node = node->next;
            return *this;
        }

        iterator &operator--() {
            node = node->prev;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            node = node->next;
            return tmp;
        }

        iterator operator--(int) {
            iterator tmp = *this;
            node = node->prev;
            return tmp;
        }

        T &operator*() const noexcept {
            return *(node->data());
        }

        T *operator->() const noexcept {
            return node->data();
        }

        friend bool operator==(const iterator &a, const iterator &b) {
            return a.node == b.node;
        }

        friend bool operator!=(const iterator &a, const iterator &b) {
            return a.node != b.node;
        }
    };

    class const_iterator {
    private:
        const Node *node;
        const list *container;

        friend class list;

        const_iterator(const Node *n, const list *c) : node(n), container(c) {}

    public:
        const_iterator() : node(nullptr), container(nullptr) {}

        const_iterator(const iterator &it) : node(it.node), container(it.container) {}

        const_iterator &operator++() {
            node = node->next;
            return *this;
        }

        const_iterator &operator--() {
            node = node->prev;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator tmp = *this;
            node = node->next;
            return tmp;
        }

        const_iterator operator--(int) {
            const_iterator tmp = *this;
            node = node->prev;
            return tmp;
        }

        const T &operator*() const noexcept {
            return *(node->data());
        }

        const T *operator->() const noexcept {
            return node->data();
        }

        friend bool operator==(const const_iterator &a, const const_iterator &b) {
            return a.node == b.node;
        }

        friend bool operator!=(const const_iterator &a, const const_iterator &b) {
            return a.node != b.node;
        }

        friend bool operator==(const iterator &a, const const_iterator &b) {
            return a.node == b.node;
        }

        friend bool operator!=(const iterator &a, const const_iterator &b) {
            return a.node != b.node;
        }

        friend bool operator==(const const_iterator &a, const iterator &b) {
            return a.node == b.node;
        }

        friend bool operator!=(const const_iterator &a, const iterator &b) {
            return a.node != b.node;
        }
    };
};

} // namespace sjtu

#endif // SJTU_LIST_HPP
