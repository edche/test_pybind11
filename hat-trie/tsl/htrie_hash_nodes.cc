#include <pybind11/pybind11.h>
#include "htrie_hash.h"

namespace py = pybind11;

// Template parameters
using CharT = char;
using T = int64_t;
using Hash = tsl::ah::str_hash<CharT>;
using KeySizeT = std::uint8_t;

using tsl_anode = tsl::detail_htrie_hash::htrie_hash<CharT, T, Hash, KeySizeT>::anode;
using tsl_trie_node = tsl::detail_htrie_hash::htrie_hash<CharT, T, Hash, KeySizeT>::trie_node;
using tsl_hash_node = tsl::detail_htrie_hash::htrie_hash<CharT, T, Hash, KeySizeT>::hash_node;
using array_hash_type = tsl::detail_htrie_hash::htrie_hash<CharT, T, Hash, KeySizeT>::array_hash_type;

//using value_node = tsl::detail_htrie_hash::value_node<T>;

/*
class trie_node: public anode {
    public:
        trie_node(): anode(anode::node_type::TRIE_NODE),
                     m_value_node(nullptr), m_children() 
        
        trie_node(const trie_node& other): anode(anode::node_type::TRIE_NODE, other.m_child_of_char),
                                           m_value_node(nullptr), m_children()

        typename std::array<std::unique_ptr<anode>, ALPHABET_SIZE>::iterator begin()
        typename std::array<std::unique_ptr<anode>, ALPHABET_SIZE>::iterator end()
        void set_child(CharT for_char, std::unique_ptr<anode> child)
        size_type nb_children()
        bool empty() const noexcept
        
        anode* first_child() 
        const anode* first_child() 

        anode* next_child(const anode& current_child) noexcept {
        const anode* next_child(const anode& current_child) const noexcept {
        
        trie_node& most_left_descendant_value_trie_node() 
        const trie_node& most_left_descendant_value_trie_node() const noexcept {
      
        std::unique_ptr<anode>& child(CharT for_char)
        const std::unique_ptr<anode>& child(CharT for_char)
                
        std::unique_ptr<value_node>& val_node()
        const std::unique_ptr<value_node>& val_node()
        
*/


PYBIND11_MODULE(libhtrie_hash_nodes, m) {

    py::class_<tsl_anode> node(m, "ANode");

    node.def(py::init<tsl_anode::node_type>())
    .def(py::init<tsl_anode::node_type, CharT>())
    .def("is_trie_node", &tsl_anode::is_trie_node)
    .def("is_hash_node", &tsl_anode::is_hash_node);

    py::enum_<tsl_anode::node_type>(node, "node_type")
    .value("TRIE_NODE", tsl_anode::node_type::TRIE_NODE)
    .value("HASH_NODE", tsl_anode::node_type::HASH_NODE)
    .export_values();

    py::class_<tsl_hash_node>(m, "HashNode")
    .def(py::init<const Hash &, float>())
    .def(py::init<std::size_t, const Hash &, float>())
    .def("array_hash", (array_hash_type& (tsl_hash_node::*)()) &tsl_hash_node::array_hash)
    .def("array_hash", (const array_hash_type& (tsl_hash_node::*)() const) &tsl_hash_node::array_hash);

    py::class_<tsl_trie_node>(m, "TrieNode")
    .def(py::init<>())
    .def(py::init<const tsl_trie_node &>());
}