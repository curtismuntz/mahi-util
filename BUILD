cc_library(
    name = "mahi_util",
    srcs = glob(["src/Mahi/Util/**/*.cpp"]),
    hdrs = glob([
        "include/Mahi/**/*.hpp",
        "include/Mahi/**/*.inl",
    ]),
    copts = [
        "-std=c++2a",
        "-fcoroutines",
    ],
    includes = ["include"],
    linkopts = [
        "-lrt",
        "-lpthread",
    ],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        "//3rdparty/cxxopts",
        "//3rdparty/fmt",
        "//3rdparty/json",
    ],
)
