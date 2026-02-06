FROM ubuntu:22.04

# Avoid warnings by switching to noninteractive
ENV DEBIAN_FRONTEND=noninteractive

# Configure apt and install packages
RUN apt-get update && apt-get -y install --no-install-recommends \
    build-essential \
    cmake \
    ninja-build \
    git \
    wget \
    curl \
    pkg-config \
    ca-certificates \
    gnupg \
    lsb-release \
    software-properties-common \
    # C++ 20 support
    g++-11 \
    gcc-11 \
    # Development tools
    gdb \
    valgrind \
    clang-format \
    clang-tidy \
    # Protocol Buffers and gRPC dependencies
    autoconf \
    automake \
    libtool \
    libssl-dev \
    zlib1g-dev \
    # PostgreSQL client library
    libpq-dev \
    postgresql-client \
    && rm -rf /var/lib/apt/lists/*

# Set GCC 11 as default
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 100 \
    && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 100

# Install Protocol Buffers
WORKDIR /tmp
RUN wget https://github.com/protocolbuffers/protobuf/releases/download/v21.12/protobuf-all-21.12.tar.gz \
    && tar -xzf protobuf-all-21.12.tar.gz \
    && cd protobuf-21.12 \
    && ./configure --prefix=/usr/local \
    && make -j$(nproc) \
    && make install \
    && ldconfig \
    && cd .. \
    && rm -rf protobuf-*

# Install gRPC
RUN git clone --recurse-submodules -b v1.54.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc /tmp/grpc \
    && cd /tmp/grpc \
    && mkdir -p cmake/build \
    && cd cmake/build \
    && cmake -DgRPC_INSTALL=ON \
          -DgRPC_BUILD_TESTS=OFF \
          -DCMAKE_INSTALL_PREFIX=/usr/local \
          -DCMAKE_BUILD_TYPE=Release \
          -DgRPC_SSL_PROVIDER=package \
          -GNinja \
          ../.. \
    && ninja \
    && ninja install \
    && ldconfig \
    && cd /tmp \
    && rm -rf /tmp/grpc

# Install libpqxx
RUN git clone https://github.com/jtv/libpqxx.git /tmp/libpqxx \
    && cd /tmp/libpqxx \
    && git checkout 7.7.5 \
    && mkdir build \
    && cd build \
    && cmake -DCMAKE_BUILD_TYPE=Release \
          -DCMAKE_INSTALL_PREFIX=/usr/local \
          -DBUILD_SHARED_LIBS=ON \
          -GNinja \
          .. \
    && ninja \
    && ninja install \
    && ldconfig \
    && cd /tmp \
    && rm -rf /tmp/libpqxx

# Create a non-root user
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME \
    && apt-get update \
    && apt-get install -y sudo \
    && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME \
    && rm -rf /var/lib/apt/lists/*

# Switch back to dialog for any ad-hoc use of apt-get
ENV DEBIAN_FRONTEND=dialog

WORKDIR /workspace

USER $USERNAME
