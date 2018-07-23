bind(
    name = "pybind11-lib",
    actual = "@pybind11//:pybind11"
)

new_git_repository(
    name = "pybind11",
    remote = "https://github.com/pybind/pybind11.git",
    tag = "v2.2.3",
    build_file = "pybind11.BUILD",
)

bind(
    name = "cityhash-lib",
    actual = "@cityhash//:cityhash"
)

new_git_repository(
    name = "cityhash",
    remote = "https://github.com/google/cityhash.git",
    commit = "8af9b8c2b889d80c22d6bc26ba0df1afb79a30db",
    build_file = "cityhash.BUILD",
)


new_local_repository(
    name = "pyheaders",
    path = "/usr/local/Cellar/python/3.6.5/Frameworks/Python.framework/Versions/3.6/include/python3.6m",
    build_file_content = """
package(
    default_visibility = [
        "//visibility:public",
    ],
)

cc_library(
    name = "headers",
    srcs = glob(["**/*.h"]),
)
""",
)