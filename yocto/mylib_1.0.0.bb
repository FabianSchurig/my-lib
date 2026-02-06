SUMMARY = "C++20 library for gRPC and PostgreSQL database utilities"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS = "grpc protobuf libpqxx postgresql"

SRC_URI = "git://github.com/FabianSchurig/my-lib.git;protocol=https;branch=main"
SRCREV = "${AUTOREV}"

S = "${WORKDIR}/git"

inherit cmake

EXTRA_OECMAKE = "-DCMAKE_CXX_STANDARD=20"

FILES:${PN} = "${libdir}/libmylib.so.*"
FILES:${PN}-dev = "${includedir}/mylib/* ${libdir}/libmylib.so ${libdir}/cmake/mylib/*"

