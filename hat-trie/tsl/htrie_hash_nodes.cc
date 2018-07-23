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

using tsl_anode = tsl::detail_htrie_hash::htrie_hash<CharT, T, Hash, KeySizeT>::anode;
using tsl_trie_node = tsl::detail_htrie_hash::htrie_hash<CharT, T, Hash, KeySizeT>::trie_node;
using tsl_hash_node = tsl::detail_htrie_hash::htrie_hash<CharT, T, Hash, KeySizeT>::hash_node;
using array_hash_type = tsl::detail_htrie_hash::htrie_hash<CharT, T, Hash, KeySizeT>::array_hash_type;
using trie_node_children = std::array<std::unique_ptr<tsl_anode>, ALPHABET_SIZE>;

using value_node = tsl::detail_htrie_hash::value_node<T>;

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

void InitTrieNodeChildrenPy(py::module &m) {
    py::class_<trie_node_children>(m, "TrieNodeChildren")
    .def(py::init<>())
    .def("__len__", [](const trie_node_children &v) { return v.size(); });
    /*
    .def("__iter__", [](trie_node_children &v) {
       return py::make_iterator(v.begin(), v.end());
    }, py::keep_alive<0, 1>()); 
    */
}
void InitTrieNodePy(py::class_<tsl_trie_node> &trie_node) {
    /*
    class trie_node: public anode {
    public:
        FAILED typename std::array<std::unique_ptr<anode>, ALPHABET_SIZE>::iterator begin()
        FAILED typename std::array<std::unique_ptr<anode>, ALPHABET_SIZE>::iterator end()

        FAILED void set_child(CharT for_char, std::unique_ptr<anode> child)
        
        std::unique_ptr<anode>& child(CharT for_char)
        const std::unique_ptr<anode>& child(CharT for_char)
                
        std::unique_ptr<value_node>& val_node()
        const std::unique_ptr<value_node>& val_node()
        
*/

    trie_node.def(py::init<>())
    .def(py::init<const tsl_trie_node &>())
    .def("nb_children", &tsl_trie_node::nb_children)
    .def("empty", &tsl_trie_node::empty)
    .def("first_child", (tsl_anode* (tsl_trie_node::*)()) &tsl_trie_node::first_child) 
    .def("first_child", (const tsl_anode* (tsl_trie_node::*)() const) &tsl_trie_node::first_child) 
    .def("next_child", (tsl_anode* (tsl_trie_node::*)(const tsl_anode&)) &tsl_trie_node::next_child)
    .def("next_child", (const tsl_anode* (tsl_trie_node::*)(const tsl_anode&) const) &tsl_trie_node::next_child) 
    .def("most_left_descendant_value_trie_node", (tsl_trie_node& (tsl_trie_node::*)()) &tsl_trie_node::most_left_descendant_value_trie_node)
    .def("most_left_descendant_value_trie_node", (const tsl_trie_node& (tsl_trie_node::*)() const) &tsl_trie_node::most_left_descendant_value_trie_node);
    //.def("val_node",  (std::unique_ptr<value_node>& (tsl_trie_node::*)()) &tsl_trie_node::val_node)
    //.def("child", (std::unique_ptr<tsl_anode>& (tsl_trie_node::*)(CharT)) &tsl_trie_node::child)
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
    InitTrieNodeChildrenPy(m);
    InitTrieNodePy(trie_node);
}