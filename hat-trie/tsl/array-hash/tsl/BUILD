package(default_visibility = ["//visibility:public"])

C_OPTS = [
    "-O3",
    "-std=c++11",
    "-Iexternal/pyheaders", 
    ]

cc_library(
    name = "tsl_libs",
    hdrs = glob(["**/*.h"]),
    deps = ["//external:cityhash-lib"],
)
