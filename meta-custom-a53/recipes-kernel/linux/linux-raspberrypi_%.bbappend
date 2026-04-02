FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI +=  "file://devtool-fragment.cfg \
            file://0001-bcm2710-rpi3b-uart0-enable-lec_act-heartbeat.patch \
	    file://0002-dts-change-serial0-to-uart0-set-pins-to-alt5.patch \
            "


