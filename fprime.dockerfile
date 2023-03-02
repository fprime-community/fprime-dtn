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
    tcpdump \
    tree \
    tshark \
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

# TODO remove once ion-core outputs .a libraries
WORKDIR /home/ptl/lib/ion-open-source-4.1.1
RUN autoreconf
RUN ./configure
RUN make

COPY lib/ion-core /home/ptl/lib/ion-core

WORKDIR /home/ptl/lib/ion-core
RUN scripts/extract.sh /home/ptl/lib/ion-open*
RUN make linux
RUN make install

# COPY

COPY lib/fprime           /home/ptl/lib/fprime
COPY lib/fprime-arm-linux /home/ptl/lib/fprime-arm-linux

# F-PRIME

RUN pip install -r /home/ptl/lib/fprime/requirements.txt

# RUN

WORKDIR /home/ptl
ENTRYPOINT bash
