SUMMARY = "test package images"
LICENSE = "CLOSED"

IMAGE_INSTALL:append = " \
    hello net-tools openssl openssl-bin tcpdump \
    htop memtester vim iproute2 devmem2 gdb strace my-driver ethtool \
"
DISTRO_FEATURES:remove = " bluetooth"
IMAGE_FEATURES += " ssh-server-dropbear"
inherit core-image
