#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "htrie_hash.h"

namespace py = pybind11;

// Template parameters
using CharT = char;
using T = int64_t;
using Hash = tsl::ah::str_hash<CharT>;
using KeySizeT = std::uint8_t;

static const std::size_t ALPHABET_SIZE = 
                        std::numeric_limits<typename std::make_unsigned<CharT>::type>::max() + 1;

using htrie_hash = tsl::detail_htrie_hash::htrie_hash<CharT, T, Hash, KeySizeT>;
using tsl_anode = htrie_hash::anode;
using tsl_trie_node = htrie_hash::trie_node;
using tsl_hash_node = htrie_hash::hash_node;
using array_hash_type = htrie_hash::array_hash_type;
using trie_node_children = std::array<std::unique_ptr<tsl_anode>, ALPHABET_SIZE>;
using value_node = tsl::detail_htrie_hash::value_node<T>;

using htrie_hash_iterator = htrie_hash::htrie_hash_iterator<false, false>;
using htrie_hash_const_iterator = htrie_hash::htrie_hash_iterator<true, false>;
using htrie_hash_prefix_iterator = htrie_hash::htrie_hash_iterator<false, true>;
using htrie_hash_const_prefix_iterator = htrie_hash::htrie_hash_iterator<true, true>;

/*
    std::pair<iterator, bool> insert(const CharT* key, size_type key_size, int&& value) {
       if(key_size > max_key_size()) {
            throw std::length_error("Key is too long.");
        }
        
        if(m_root == nullptr) {
            m_root.reset(new hash_node(m_hash, m_max_load_factor));
        }
        
        return insert_impl(*m_root, key, key_size, value);
    }

    struct str_hash {
    std::size_t operator()(const CharT* key, std::size_t key_size) const {
        static const std::size_t init = std::size_t((sizeof(std::size_t) == 8)?0xcbf29ce484222325:0x811c9dc5);
        static const std::size_t multiplier = std::size_t((sizeof(std::size_t) == 8)?0x100000001b3:0x1000193);
        
        std::size_t hash = init;
        for (std::size_t i = 0; i < key_size; ++i) {
            hash ^= key[i];
            hash *= multiplier;
        }
        
        return hash;
    }
};  
*/


void InitHatTrieHash(py::module &m) {
    py::class_<htrie_hash>(m, "HatTrieHash")
    .def(py::init<Hash&, float, std::size_t>())
    .def(py::init<htrie_hash>())
    .def("begin", (htrie_hash_const_iterator (htrie_hash::*)() const) &htrie_hash::begin)
    .def("cbegin", (htrie_hash_const_iterator (htrie_hash::*)() const) &htrie_hash::cbegin)
    .def("end", (htrie_hash_const_iterator (htrie_hash::*)() const) &htrie_hash::end) 
    .def("cend", (htrie_hash_const_iterator (htrie_hash::*)() const) &htrie_hash::cend)
    .def("empty", &htrie_hash::empty)
    .def("size", &htrie_hash::size)
    .def("max_size", &htrie_hash::max_size)
    .def("max_key_size", &htrie_hash::max_key_size)
    .def("shrink_to_fit", &htrie_hash::shrink_to_fit)
    .def("clear", &htrie_hash::clear)
    .def("erase", (htrie_hash_iterator (htrie_hash::*)(htrie_hash_const_iterator)) &htrie_hash::erase)
    .def("erase", (htrie_hash_iterator (htrie_hash::*)(htrie_hash_const_iterator, htrie_hash_const_iterator)) &htrie_hash::erase) 
    .def("erase", (std::size_t (htrie_hash::*)(const CharT*, std::size_t)) &htrie_hash::erase)
    .def("erase_prefix", (std::size_t (htrie_hash::*)(const CharT*, std::size_t)) &htrie_hash::erase_prefix)
    .def("swap", &htrie_hash::swap)
    .def("insert", (std::pair<htrie_hash::iterator, bool> (htrie_hash::*)(const CharT*, std::size_t, int)) &htrie_hash::insert)
    ;
}

void InitHTrieHashIterator(py::module &m) {
    py::class_<htrie_hash_iterator>(m, "HatTrieHashIterator")
    .def(py::init<>())
    .def(py::init<const htrie_hash_iterator&>())
    .def("key", (void (htrie_hash_iterator::*)(std::basic_string<CharT>& key_buffer_out) const) &htrie_hash_iterator::key)
    .def("key", (std::basic_string<CharT> (htrie_hash_iterator::*)() const) &htrie_hash_iterator::key)
    .def("get_current_trie_node", &htrie_hash_iterator::get_current_trie_node)
    .def("get_current_hash_node", &htrie_hash_iterator::get_current_hash_node);
}

void InitHTrieHashConstIterator(py::module &m) {
    py::class_<htrie_hash_const_iterator>(m, "HatTrieHashConstIterator")
    .def(py::init<>())
    .def(py::init<const htrie_hash_iterator&>())
    .def("key", (void (htrie_hash_const_iterator::*)(std::basic_string<CharT>& key_buffer_out) const) &htrie_hash_const_iterator::key)
    .def("key", (std::basic_string<CharT> (htrie_hash_const_iterator::*)() const) &htrie_hash_const_iterator::key)
    .def("get_current_trie_node", &htrie_hash_const_iterator::get_current_trie_node)
    .def("get_current_hash_node", &htrie_hash_const_iterator::get_current_hash_node);
}

void InitHTrieHashPrefixIterator(py::module &m) {
    py::class_<htrie_hash_prefix_iterator>(m, "HatTrieHashPrefixIterator")
    .def(py::init<>())
    .def(py::init<const htrie_hash_prefix_iterator&>())
    .def("key", (void (htrie_hash_prefix_iterator::*)(std::basic_string<CharT>& key_buffer_out) const) &htrie_hash_prefix_iterator::key)
    .def("key", (std::basic_string<CharT> (htrie_hash_prefix_iterator::*)() const) &htrie_hash_prefix_iterator::key)
    .def("get_current_trie_node", &htrie_hash_prefix_iterator::get_current_trie_node)
    .def("get_current_hash_node", &htrie_hash_prefix_iterator::get_current_hash_node);
}

void InitHTrieHashPrefixConstIterator(py::module &m) {
    py::class_<htrie_hash_const_prefix_iterator>(m, "HatTrieHashPrefixConstIterator")
    .def(py::init<>())
    .def(py::init<const htrie_hash_prefix_iterator&>())
    .def("key", (void (htrie_hash_const_prefix_iterator::*)(std::basic_string<CharT>& key_buffer_out) const) &htrie_hash_const_prefix_iterator::key)
    .def("key", (std::basic_string<CharT> (htrie_hash_const_prefix_iterator::*)() const) &htrie_hash_const_prefix_iterator::key)
    .def("get_current_trie_node", &htrie_hash_const_prefix_iterator::get_current_trie_node)
    .def("get_current_hash_node", &htrie_hash_const_prefix_iterator::get_current_hash_node);
}

void InitANodePy(py::class_<tsl_anode> &node) {
    node.def(py::init<tsl_anode::node_type>())
    .def(py::init<tsl_anode::node_type, CharT>())
    .def("is_trie_node", &tsl_anode::is_trie_node)
    .def("is_hash_node", &tsl_anode::is_hash_node)
    .def("as_trie_node", (const tsl_trie_node& (tsl_anode::*)() const) &tsl_anode::as_trie_node)
    .def("as_hash_node", (const tsl_hash_node& (tsl_anode::*)() const) &tsl_anode::as_hash_node)
    .def("child_of_char", &tsl_anode::child_of_char)
    .def("parent", (const tsl_trie_node* (tsl_anode::*)() const) &tsl_anode::parent);

    py::enum_<tsl_anode::node_type>(node, "node_type")
    .value("TRIE_NODE", tsl_anode::node_type::TRIE_NODE)
    .value("HASH_NODE", tsl_anode::node_type::HASH_NODE)
    .export_values();
}

void InitHashNodePy(py::class_<tsl_hash_node> &hash_node) {
    hash_node.def(py::init<const Hash &, float>())
    .def(py::init<std::size_t, const Hash &, float>())
    .def("array_hash", (array_hash_type& (tsl_hash_node::*)()) &tsl_hash_node::array_hash)
    .def("array_hash", (const array_hash_type& (tsl_hash_node::*)() const) &tsl_hash_node::array_hash);
}

void InitValueNodePy(py::module &m) {
    py::class_<value_node>(m, "ValueNode")
    .def(py::init<>());
}

void InitTrieNodePy(py::class_<tsl_trie_node> &trie_node) {

    trie_node.def(py::init<>())
    .def(py::init<const tsl_trie_node &>())
    .def("nb_children", &tsl_trie_node::nb_children)
    .def("empty", &tsl_trie_node::empty)
    .def("first_child", (const tsl_anode* (tsl_trie_node::*)() const) &tsl_trie_node::first_child) 
    .def("next_child", (const tsl_anode* (tsl_trie_node::*)(const tsl_anode&) const) &tsl_trie_node::next_child) 
    .def("most_left_descendant_value_trie_node", (const tsl_trie_node& (tsl_trie_node::*)() const) &tsl_trie_node::most_left_descendant_value_trie_node)
    .def("get_child", &tsl_trie_node::get_child)
    .def("get_val_node", &tsl_trie_node::get_val_node);
}

/* BINDING CODE */
PYBIND11_MAKE_OPAQUE(trie_node_children);

PYBIND11_MODULE(libhtrie_hash_nodes, m) {
    py::class_<tsl_anode> node(m, "ANode");
    py::class_<tsl_hash_node> hash_node(m, "HashNode", node);
    py::class_<tsl_trie_node> trie_node(m, "TrieNode", node);

    InitANodePy(node);
    InitHashNodePy(hash_node);
    InitValueNodePy(m);
    InitTrieNodePy(trie_node);
    InitHTrieHashIterator(m);
    InitHTrieHashConstIterator(m);
    InitHTrieHashPrefixIterator(m);
    InitHTrieHashPrefixConstIterator(m);
    InitHatTrieHash(m);
}