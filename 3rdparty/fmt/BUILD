licenses(["notice"])

cc_library(
    name = "fmt",
    srcs = [
        "src/format.cc",
        "src/os.cc",
    ],
    hdrs = glob(["include/fmt/*.h"]),
    copts = ["-std=c++11"],
    includes = ["include"],
    visibility = ["//visibility:public"],
)
