//
// Created by !--- legGalaxyCode ---! on 31.12.2020.
//

#ifndef FILE_ANALYZER_HASH_TABLE_H
#define FILE_ANALYZER_HASH_TABLE_H

#include <vector>
#include <string>
#include "hash_functions.h"

struct first_hash
{
    int operator()(const std::string& s, int table_size) const;
};

struct second_hash
{
    int operator()(const std::string& s, int table_size) const;
};

template<typename TKey, typename TValue>
class linked_hash_list
{
private:
    struct node {
    node() : next(nullptr) {};
    node(TKey _k, TValue _v) : key(_k), value(_v), next(nullptr) {};

    TKey key; // hash
    TValue value; // path to dir
    node *next;
    };

    node *head;
public:
    linked_hash_list();
    linked_hash_list(TKey _k, TValue _v);
    ~linked_hash_list();

    void insert(TKey _k, TValue _v);
    void remove();
    void remove(TValue _v);
    void print_node() const noexcept;
    node *get_head_node() const;
    node *get_cur_node() const;

    int get_size() const;

    //node* hash_table<TKey, TValue>::find_head(const TValue &value, first_hash hash1);
    class iterator {
    private:
        node *nod;
    public:
        explicit iterator(node *el) : nod(el) {}
        iterator& operator=(node* _node) {
            this->nod = _node;
            return *this;
        }
        bool operator==(const iterator &other) const {
            if (this == &other)
                return true;
            return nod == other.nod;
        }
        bool operator!=(const iterator &other) const {
            return !operator==(other);
        }
        TValue operator*() const {
            if (nod) {
                return nod->value;
            }
            return TValue();
        }
        iterator& operator++() {
            if (nod)
                nod = nod->next;
            return *this;
        }
        /*iterator operator++(int val) {
            iterator it = *this;
            ++(*nod);
            return it;
        }*/
        /*void operator++() {
            if (nod) {
                nod = nod->next;
            }
        }*/
        node *get_node() const { return nod; };
    };
    iterator begin() const;
    iterator end() const;

    //void set_value(TValue _value);
    //linked_hash_list<TKey, TValue> *get_next();
    //void set_next(linked_hash_list<TKey, TValue> *_next);
};

template<typename TKey, typename TValue> // tkey = hash, tvalue = filename
class hash_table
{
private:
    static const int default_size;
    constexpr static const double rehash_size = 1; //0.75;
    std::vector<linked_hash_list<TKey, TValue>> buckets;
    long unsigned size; // сколько эл-ов в массиве
    int buffer_size; // размер массива (сколько памяти)
public:
    hash_table();
    hash_table(int _size);
    ~hash_table();

    void resize();
    void rehash();
    void print_hash_table() const noexcept;
    //void print_linked_list(const TValue &value, first_hash hash1 = first_hash()) const;
    void print_linked_list() const noexcept;
    long unsigned get_table_size() const noexcept;

    bool find(const TValue &value, first_hash hash1 = first_hash());
    //typename linked_hash_list<TKey, TValue>::node* find_head(const TValue &value, first_hash hash1 = first_hash());
    bool remove(const TValue &value, first_hash hash1 = first_hash());
    bool add(const TValue &value, const TValue &dir_path, first_hash hash1 = first_hash());
};

/*
 * template <class THash1 = hash_func1, class THash2 = hash_func2>
class hash_table1
{
private:
    static const int default_size = 8; // начальный размер таблицы
    constexpr static const double rehash_size = 0.75; // коэф-т расширения таблицы
    struct Node {
        std::string value;
        bool state; //

        Node(std::string  _value) : value(std::move(_value)), state(true) {}
    };
    Node** arr; // массив нодов
    int size; // сколько эл-ов в массиве без флага deleted
    int buffer_size; // размер массива (сколько памяти)
    int size_all_non_nullptr; // сколько эл-ов в массиве
public:
    hash_table() {
        buffer_size = default_size;
        size = 0;
        size_all_non_nullptr = 0;
        arr = new Node*[buffer_size];
        for (int i = 0; i < buffer_size; ++i)
            arr[i] = nullptr;
    }
    ~hash_table() {
        for (int i = 0; i < buffer_size; ++i)
            if (arr[i])
                delete arr[i];
        delete[] arr;
    }
    void resize() {
        int past_buffer_size = buffer_size;
        buffer_size *= 2;
        size_all_non_nullptr = 0;
        size = 0;
        Node** arr2 = new Node*[buffer_size];
        for (int i = 0; i < buffer_size; ++i)
            arr2[i] = nullptr;
        std::swap(arr, arr2);
        for (int i = 0; i < past_buffer_size; ++i) {
            if (arr2[i] && arr2[i]->state)
                Add(arr2[i]->value);
        }
        for (int i = 0; i < past_buffer_size; ++i)
            if (arr2[i])
                delete arr2[i];
        delete[] arr2;
    }
    void rehash() {
        size_all_non_nullptr = 0;
        size = 0;
        Node** arr2 = new Node*[buffer_size];
        for (int i = 0; i < buffer_size; ++i)
            arr2[i] = nullptr;
        std::swap(arr, arr2);
        for (int i = 0; i < buffer_size; ++i) {
            if (arr2[i] && arr2[i]->state)
                Add(arr2[i]->value);
        }
        for (int i = 0; i < buffer_size; ++i)
            if (arr2[i])
                delete arr2[i];
        delete[] arr2;
    }
    bool Find(const std::string& value, const THash1 hash1 = THash1(), const THash2 hash2 = THash2()) {
        int h1 = hash1(value, buffer_size); // значение начальной позиции
        int h2 = hash2(value, buffer_size); // значение за "шаг" по таблице
        int i = 0;
        while (arr[h1] != nullptr && i < buffer_size) {
            if (arr[h1]->value == value && arr[h1]->state)
                return true; // такой эл-т есть
            h1 = (h1 + h2) % buffer_size;
            ++i; // если i >= buffer_size, значит мы уже обошли все ячейки таблицы, иначе бы зациклилось
        }
        return false;
    }
    bool remove(const std::string& value, const THash1& hash1 = THash1(), const THash2 hash2 = THash2()) {
        int h1 = hash1(value, buffer_size);
        int h2 = hash2(value, buffer_size);
        int i = 0;
        while (arr[h1] != nullptr && i < buffer_size) {
            if (arr[h1]->value == value && arr[h1]->state) {
                arr[h1]->state = false;
                --size;
                return true;
            }
            h1 = (h1 + h2) % buffer_size;
            ++i;
        }
        return false;
    }
    bool Add(const std::string& value, const THash1& hash1 = THash1(), const THash2& hash2 = THash2()) {
        if (size + 1 > int(rehash_size * buffer_size))
            resize();
        else if (size_all_non_nullptr > size * 2)
            rehash(); // происходит рехеш, т.к. слишком много удаленных эл-в
        int h1 = hash1(value, buffer_size);
        int h2 = hash2(value, buffer_size);
        int i = 0;
        int first_deleted = -1; // запоминаем первый удаленный эл-т
        while (arr[h1] != nullptr && i < buffer_size) {
            if (arr[h1]->value == value && arr[h1]->state)
                return false; // такой эл-т уже есть, а значит его нельзя вставлять
            if (!arr[h1]->state && first_deleted == -1) // находим место нового эл-та
                first_deleted = h1;
            h1 = (h1 + h2) % buffer_size;
            ++i;
        }
        if (first_deleted == -1) { // если не нашлось под-щего место, создаем новый нод
            arr[h1] = new Node(value);
            ++size_all_non_nullptr; // т.к. заполнился пробел, то их кол-во увеличилось
        } else {
            arr[first_deleted]->value = value;
            arr[first_deleted]->state = true;
        }
        ++size; // в любом случае кол-во эл-в увеличилось
        return true;
    }
};
 */
#endif //FILE_ANALYZER_HASH_TABLE_H
