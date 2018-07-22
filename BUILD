package(default_visibility = ["//visibility:public"])

C_OPTS = [
    "-O3",
    "-std=c++11",
    "-Iexternal/pyheaders", 
    ]

cc_library(
   name = "example",
   srcs = [ "example.cc" ],
   deps = [
       "@pyheaders//:headers", 
       "//external:pybind11-lib",
    ],
   copts = C_OPTS,
)