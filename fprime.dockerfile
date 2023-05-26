FROM debian

ARG DEBIAN_FRONTEND=noninteractive

# SETUP

# Dependencies for pyion: python3-dev
RUN apt-get update && apt-get install --no-install-recommends -y \
    automake \
    curl \
    git \
    net-tools \
    netcat \
    man \
    python3-dev \
    silversearcher-ag \
    ssh \
    tcpdump \
    tree \
    tshark \
    vim \
    build-essential g++ gdb cmake python3 python3-venv python3-pip \
    gcc-aarch64-linux-gnu g++-aarch64-linux-gnu binutils-aarch64-linux-gnu

# ION

RUN mkdir -p /home/ptl/lib

RUN curl -L 'https://sourceforge.net/projects/ion-dtn/files/ion-open-source-4.1.1.tar.gz/download' | \
    tar -xz -C /home/ptl/lib --no-same-owner

# TODO remove once ion-core outputs .a libraries
WORKDIR /home/ptl/lib/ion-open-source-4.1.1
RUN autoreconf
RUN ./configure
RUN make
# Installing for pyion
RUN make install

COPY lib/ion-core /home/ptl/lib/ion-core

WORKDIR /home/ptl/lib/ion-core
RUN scripts/extract.sh /home/ptl/lib/ion-open*
RUN make linux
RUN make install

# PYION

COPY lib/pyion /home/ptl/lib/pyion

# TODO pyion is on `v4.1.2` branch but the underlying ION used here is 4.1.1.
# Once ion-core is at 4.1.2 this won't be a potential issue
ENV ION_HOME /home/ptl/lib/ion-open-source-4.1.1
ENV PYION_HOME /home/ptl/lib/pyion
ENV PYION_BP_VERSION BPv7
ENV LD_LIBRARY_PATH /home/ptl/lib/ion-open-source-4.1.1/.libs
RUN pip install --user /home/ptl/lib/pyion
RUN pip install --user numpy

# F PRIME

COPY lib/fprime /home/ptl/lib/fprime
RUN pip install -r /home/ptl/lib/fprime/requirements.txt

COPY lib/fprime-gds-dtn /home/ptl/lib/fprime-gds-dtn
RUN pip install -e /home/ptl/lib/fprime-gds-dtn

COPY lib/fprime-arm-linux /home/ptl/lib/fprime-arm-linux

COPY resources/gds/RefTopologyAppDictionary.xml /home/ptl
COPY resources/gds/host3.rc                     /home/ptl/lib/ion-core
COPY resources/flight/host2.rc                  /home/ptl/lib/ion-core

# RUN

WORKDIR /home/ptl
ENTRYPOINT bash
