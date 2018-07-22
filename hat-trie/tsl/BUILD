package(default_visibility = ["//visibility:public"])

C_OPTS = [
    "-O3",
    "-std=c++11",
    "-Iexternal/pyheaders", 
    ]

cc_library(
    name = "tsl_libs",
    hdrs = glob(["**/*.h"]),
)


cc_library(
   name = "htrie_hash_nodes",
   srcs = [ "htrie_hash_nodes.cc" ],
   hdrs = ["htrie_hash.h"],
   deps = [
       "@pyheaders//:headers", 
       "//external:pybind11-lib",
       ":tsl_libs"
    ],
   copts = C_OPTS,
)