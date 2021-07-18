//
// Created by !--- legGalaxyCode ---! on 31.12.2020.
//

#include "hash_table.h"

int first_hash::operator()(const std::string &s, int table_size) const {
    return hash_func_seed(s, table_size, table_size - 1);
}

int second_hash::operator()(const std::string &s, int table_size) const {
    return hash_func_seed(s, table_size, table_size + 1);
}

template<typename TKey, typename TValue>
linked_hash_list<TKey, TValue>::linked_hash_list() : head(nullptr) {}

template<typename TKey, typename TValue>
linked_hash_list<TKey, TValue>::linked_hash_list(TKey _k, TValue _v) {
    this->insert(_k, _v);
}

template<typename TKey, typename TValue>
linked_hash_list<TKey, TValue>::~linked_hash_list<TKey, TValue>() {
    while (head)
        remove();
}

template <typename TKey, typename TValue>
void linked_hash_list<TKey, TValue>::insert(TKey _k, TValue _v) {
    node *elem = new node(_k, _v);
    elem->next = head;
    head = elem;
}

template<typename TKey, typename TValue>
void linked_hash_list<TKey, TValue>::remove() {
    while (head) {
        node* new_head = head->next;
        delete head;
        head = new_head;
    }
}

template<typename TKey, typename TValue>
void linked_hash_list<TKey, TValue>::remove(TValue _v) { // not sure that it works
    node *prev = head;
    node *cur = head->next;
    while (cur != nullptr) {
        if (cur->value == _v)
            break;
        else {
            prev = cur;
            cur = cur->next;
        }
    }
    if (cur != nullptr) {
        prev->next = cur->next;
        delete cur;
    }
}

template<typename TKey, typename TValue>
typename linked_hash_list<TKey, TValue>::iterator linked_hash_list<TKey, TValue>::begin() const {
    return iterator(head);
}

template<typename TKey, typename TValue>
typename linked_hash_list<TKey, TValue>::iterator linked_hash_list<TKey, TValue>::end() const {
    return iterator(nullptr);
}

template<typename TKey, typename TValue>
int linked_hash_list<TKey, TValue>::get_size() const {
    int s = 0;
    for (iterator it = begin(); it != end(); ++it)
        ++s;
    return s;
}

template<typename TKey, typename TValue>
typename linked_hash_list<TKey, TValue>::node* linked_hash_list<TKey, TValue>::get_head_node() const {
    return head;
}

template<typename TKey, typename TValue>
typename linked_hash_list<TKey, TValue>::node* linked_hash_list<TKey, TValue>::get_cur_node() const {
    return head;
}

template <typename TKey, typename TValue>
void linked_hash_list<TKey, TValue>::print_node() const noexcept{
    node* start = head;
    while (start != nullptr) {
        std::cout << start->value << "\n";
        start = start->next;
    }
}

template<typename TKey, typename TValue>
const int hash_table<TKey, TValue>::default_size = 504200;

template<typename TKey, typename TValue>
hash_table<TKey, TValue>::hash_table() : buffer_size(default_size), size(0) {
    buckets.resize(this->default_size);
}

template<typename TKey, typename TValue>
hash_table<TKey, TValue>::hash_table(int _size) : buffer_size(_size) {
    buckets.resize(this->buffer_size);
}

template<typename TKey, typename TValue>
hash_table<TKey, TValue>::~hash_table() {
    buckets.clear();
}

template<typename TKey, typename TValue>
void hash_table<TKey, TValue>::resize() {
    for (int i = 0; i < buffer_size; ++i)
        buckets.push_back(linked_hash_list<TKey, TValue>());
    buffer_size *= 2;
    // may be need realloc func in linked_list
    //std::vector<linked_hash_list<TKey, TValue>> temp_buckets;
    //temp_buckets.reserve(buffer_size);
    //std::swap(buckets, temp_buckets);
    //std::copy(buckets.begin(), buckets.end(), temp_buckets.end());
    //temp_buckets.reserve(buffer_size);
    //buckets = temp_buckets; // error in stack */
}

template<typename TKey, typename TValue>
void hash_table<TKey, TValue>::rehash() {}

template<typename TKey, typename TValue>
void hash_table<TKey, TValue>::print_hash_table() const noexcept {
    int num_of_collision = 0;

};

template< typename TKey, typename TValue>
long unsigned hash_table<TKey, TValue>::get_table_size() const noexcept {
    return size;
}

template<typename TKey, typename TValue>
bool hash_table<TKey, TValue>::find(const TValue &value, const first_hash hash1) {
    int h1 = hash1(value, buffer_size);
    linked_hash_list<TKey, TValue> &lst = buckets.at(h1); // h1 = key
    return lst.get_head_node() != nullptr;
}

/*template<typename TKey, typename TValue>
typename linked_hash_list<TKey, TValue>::node* hash_table<TKey, TValue>::find_head(const TValue &value, first_hash hash1) {
    int h1 = hash1(value, buffer_size);
    linked_hash_list<TKey, TValue> &lst = buckets.at(h1); // h1 = key
    //auto list = lst.get_head_node();
    //return list;
    //return lst.get_head_node() != nullptr ? lst.get_head_node() : nullptr;
}*/

template<typename TKey, typename TValue>
bool hash_table<TKey, TValue>::remove(const TValue &value, const first_hash hash1) {
    int h1 = hash1(value, buffer_size);
    linked_hash_list<TKey, TValue> &lst = buckets.at(h1);
    if (lst.get_head_node() == nullptr) {
        return false;
    } else {
        lst.~linked_hash_list();
        --size;
    }
}

template<typename TKey, typename TValue>
bool hash_table<TKey, TValue>::add(const TValue &value, const TValue &dir_path, const first_hash hash1) {
    /*if (size + 1 >= int(rehash_size * buffer_size))
        resize();*/
    int h1 = hash1(value, buffer_size); // key
    linked_hash_list<TKey, TValue> &lst = buckets.at(h1);
    if (lst.get_size() != 0) {
        lst.insert(h1, dir_path);
        ++size;
        return true;
    }
    buckets.at(h1).insert(h1, dir_path); // may be error malloc
    ++size;
}

template <typename TKey, typename TValue>
void hash_table<TKey, TValue>::print_linked_list() const noexcept {
    for (int i = 0; i < buffer_size; ++i) {
        buckets.at(i).print_node();
    }
}

template class linked_hash_list<int, std::string>;
template class hash_table<int, std::string>;