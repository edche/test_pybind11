C_OPTS = [
    "-O3",
    "-std=c++11",
    "-Iexternal/pyheaders", 
    ]

cc_library(
    name = "cityhash",
    hdrs = glob([
      "src/*.h",
    ]),
    copts = C_OPTS,
    visibility = ["//visibility:public"],
)