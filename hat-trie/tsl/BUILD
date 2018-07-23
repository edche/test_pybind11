package(default_visibility = ["//visibility:public"])

C_OPTS = [
    "-O3",
    "-std=c++11",
    "-Iexternal/pyheaders", 
    ]

cc_library(
   name = "htrie_hash_nodes",
   srcs = [ "htrie_hash_nodes.cc" ],
   hdrs = ["htrie_hash.h"],
   deps = [
       "@pyheaders//:headers", 
       "//external:pybind11-lib",
       "//external:cityhash-lib",
       "//hat-trie/tsl/array-hash/tsl:tsl_libs"
    ],
   copts = C_OPTS,
)

#/Users/edche/work/test_pybind11/hat-trie/tsl/array-hash/tsl/BUILD