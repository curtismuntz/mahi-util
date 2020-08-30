cc_library(
    name = "mahi_util",
    srcs = glob(
        ["src/Mahi/Util/**/*.cpp"],
        exclude = ["src/Mahi/Util/Coroutine.cpp"],
    ),
    hdrs = glob(
        [
            "include/Mahi/**/*.hpp",
            "include/Mahi/**/*.inl",
        ],
        exclude = ["include/Mahi/Util/Coroutine.hpp"],
    ),
    copts = [
        "-std=c++11",
        #        "-fcoroutines",
        "-DLinux",
        "-D_GNU_SOURCE=1",
    ],
    includes = ["include"],
    linkopts = [
        "-lrt",
        #        "-lstdc++coroutines",
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
