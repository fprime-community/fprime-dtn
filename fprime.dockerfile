FROM debian

ARG DEBIAN_FRONTEND=noninteractive

# SETUP

RUN apt-get update && apt-get install --no-install-recommends -y \
    curl \
    git \
    net-tools \
    netcat \
    silversearcher-ag \
    ssh \
    tree \
    vim \
    build-essential g++ gdb cmake python3 python3-venv python3-pip  \
    gcc-aarch64-linux-gnu g++-aarch64-linux-gnu binutils-aarch64-linux-gnu

RUN python3 -m venv /home/ptl/class-venv
RUN . /home/ptl/class-venv/bin/activate
RUN pip install -U setuptools setuptools_scm wheel pip

# RUN

WORKDIR /home/ptl
ENTRYPOINT bash
