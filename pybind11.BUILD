C_OPTS = [
    "-O3",
    "-std=c++11",
    "-Iexternal/pyheaders", 
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

