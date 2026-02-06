SUMMARY = "Modern C++20 library for gRPC and PostgreSQL database utilities"
DESCRIPTION = "A library providing database and gRPC utility functions for PostgreSQL operations"
HOMEPAGE = "https://github.com/FabianSchurig/my-lib"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS = "grpc protobuf libpqxx postgresql"

SRC_URI = "git://github.com/FabianSchurig/my-lib.git;protocol=https;branch=main"
SRCREV = "${AUTOREV}"

S = "${WORKDIR}/git"

inherit cmake

# CMake configuration
EXTRA_OECMAKE = " \
    -DCMAKE_CXX_STANDARD=20 \
    -DBUILD_SHARED_LIBS=ON \
    -DMYLIB_BUILD_TESTS=OFF \
    -DMYLIB_BUILD_EXAMPLES=OFF \
"

# Specify required packages for runtime
RDEPENDS:${PN} = " \
    grpc \
    protobuf \
    libpqxx \
    postgresql \
"

# Development package dependencies
RDEPENDS:${PN}-dev = " \
    ${PN} \
    grpc-dev \
    protobuf-dev \
    libpqxx-dev \
    postgresql-dev \
"

# Package the library and headers
FILES:${PN} = " \
    ${libdir}/libmylib.so.* \
"

FILES:${PN}-dev = " \
    ${includedir}/mylib/* \
    ${libdir}/libmylib.so \
    ${libdir}/cmake/mylib/* \
"

FILES:${PN}-staticdev = " \
    ${libdir}/libmylib.a \
"

# Ensure the library is staged properly
do_install:append() {
    # Ensure all generated protobuf headers are installed
    install -d ${D}${includedir}/mylib
}

BBCLASSEXTEND = "native nativesdk"
