C_OPTS = [
    "-O3",
    "-shared",
    "-std=c++11",
    "-undefined dynamic_lookup", 
    "-Iexternal/pyheaders", 
    "-std=c++11"
    ]

cc_library(
    name = "pybind11",
    hdrs = glob([
      "include/**/*.h",
    ]),
    deps = [
        "@pyheaders//:headers",
         ],
    copts = C_OPTS,
    visibility = ["//visibility:public"],
)