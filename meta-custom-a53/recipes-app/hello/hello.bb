SUMMARY = "hello program"
DESCRIPTION = "simple hello world example"
LICENSE = "CLOSED"

SRC_URI = "file://hello.c \
	   file://CMakeLists.txt \
	   file://hello.service"

S = "${WORKDIR}"

inherit cmake systemd

SYSTEMD_SERVICE:${PN} = "hello.service"
SYSTEMD_AUTO_ENABLE:${PN} = "enable"

do_install:append() {
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/hello.service ${D}${systemd_system_unitdir}
}
FILES:${PN} += "${systemd_system_unitdir}/hello.service"
