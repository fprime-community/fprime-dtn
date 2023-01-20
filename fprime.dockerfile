FROM debian

ARG DEBIAN_FRONTEND=noninteractive

# SETUP

RUN apt-get update && apt-get install --no-install-recommends -y \
    automake \
    curl \
    git \
    net-tools \
    netcat \
    man \
    silversearcher-ag \
    ssh \
    tree \
    vim \
    build-essential g++ gdb cmake python3 python3-venv python3-pip  \
    gcc-aarch64-linux-gnu g++-aarch64-linux-gnu binutils-aarch64-linux-gnu

RUN python3 -m venv /home/ptl/class-venv
RUN . /home/ptl/class-venv/bin/activate
RUN pip install -U setuptools setuptools_scm wheel pip

# ION

RUN mkdir -p /home/ptl/lib

RUN curl -L 'https://sourceforge.net/projects/ion-dtn/files/ion-open-source-4.1.1.tar.gz/download' | \
    tar -xz -C /home/ptl/lib --no-same-owner

COPY lib/ion-core /home/ptl/lib/ion-core

# TODO compiling ion-core
# WORKDIR /home/ptl/lib/ion-core
# scripts/extract.sh /home/ptl/lib/ion-open*
# make linux
# scripts/make-man-pages.sh ../ion-open-source-4.1.1
# scripts/host.sh <node a IP> <node b IP>
# scripts/host.sh <node b IP> <node a IP>

# TODO compiling ion
# WORKDIR /home/ptl/lib/ion-open-source-4.1.1
# autoreconf
# ./configure
# make
# make install
# ldconfig

# COPY

COPY lib/fprime           /home/ptl/lib/fprime
COPY lib/fprime-arm-linux /home/ptl/lib/fprime-arm-linux

# RUN

WORKDIR /home/ptl
ENTRYPOINT bash
