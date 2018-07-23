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
htrie_hash(const Hash& hash, float max_load_factor, size_type burst_threshold): 
                                         m_root(nullptr), m_nb_elements(0), 
                                         m_hash(hash), m_max_load_factor(max_load_factor)
    {
        this->burst_threshold(burst_threshold);
    }
    
    htrie_hash(const htrie_hash& other): m_root(nullptr), m_nb_elements(other.m_nb_elements), 
                                         m_hash(other.m_hash), m_max_load_factor(other.m_max_load_factor),
                                         m_burst_threshold(other.m_burst_threshold)
    {
        if(other.m_root != nullptr) {
            if(other.m_root->is_hash_node()) {
                m_root.reset(new hash_node(other.m_root->as_hash_node()));
            }
            else {
                m_root.reset(new trie_node(other.m_root->as_trie_node()));
            }
        }
    }
    
    htrie_hash(htrie_hash&& other) noexcept(std::is_nothrow_move_constructible<Hash>::value)
                                  : m_root(std::move(other.m_root)),
                                    m_nb_elements(other.m_nb_elements),
                                    m_hash(std::move(other.m_hash)),
                                    m_max_load_factor(other.m_max_load_factor),
                                    m_burst_threshold(other.m_burst_threshold)
    {
        other.clear();
    }
*/
void InitHatTrieHash(py::module &m) {
    py::class_<htrie_hash>(m, "HatTrieHash")
    .def(py::init<Hash&, float, std::size_t>());
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
    .def("as_trie_node", (tsl_trie_node& (tsl_anode::*)()) &tsl_anode::as_trie_node)
    .def("as_trie_node", (const tsl_trie_node& (tsl_anode::*)() const) &tsl_anode::as_trie_node)
    .def("as_hash_node", (tsl_hash_node& (tsl_anode::*)()) &tsl_anode::as_hash_node)
    .def("as_hash_node", (const tsl_hash_node& (tsl_anode::*)() const) &tsl_anode::as_hash_node)
    .def("child_of_char", &tsl_anode::child_of_char)
    .def("parent", (tsl_trie_node* (tsl_anode::*)()) &tsl_anode::parent)
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
    .def("first_child", (tsl_anode* (tsl_trie_node::*)()) &tsl_trie_node::first_child) 
    .def("first_child", (const tsl_anode* (tsl_trie_node::*)() const) &tsl_trie_node::first_child) 
    .def("next_child", (tsl_anode* (tsl_trie_node::*)(const tsl_anode&)) &tsl_trie_node::next_child)
    .def("next_child", (const tsl_anode* (tsl_trie_node::*)(const tsl_anode&) const) &tsl_trie_node::next_child) 
    .def("most_left_descendant_value_trie_node", (tsl_trie_node& (tsl_trie_node::*)()) &tsl_trie_node::most_left_descendant_value_trie_node)
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