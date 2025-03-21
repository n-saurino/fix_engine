# Use an Ubuntu base image
FROM arm64v8/ubuntu:20.04

# Set the working directory
WORKDIR /workspace

# Set environment variables to make the build non-interactive
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=America/Chicago  

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    software-properties-common \
    build-essential \
    wget \
    && add-apt-repository ppa:ubuntu-toolchain-r/test && \
    apt-get update && \
    apt-get install -y \
    gcc-9 g++-9 libstdc++-9-dev \
    libstdc++6 \
    cmake \
    ninja-build \
    gdb \
    git \
    vim \
    ccache \
    clang \
    valgrind \
    libcurl4-openssl-dev \
    libpq-dev \
    libxml2-dev \
    libjsoncpp-dev \
    libgtest-dev \
    libbenchmark-dev \
    clang-tidy \
    clang-format \
    doxygen \
    autoconf \
    automake \
    libtool \
    zlib1g-dev \
    libbz2-dev \
    liblzma-dev \
    neovim \
    cron \
    findutils \
    && rm -rf /var/lib/apt/lists/*

# Set GCC and G++ alternatives to ensure consistent usage of GCC 9
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 100 && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 100
    
# Install Autoconf 2.71
RUN wget https://ftp.gnu.org/gnu/autoconf/autoconf-2.71.tar.gz && \
    tar -xf autoconf-2.71.tar.gz && \
    cd autoconf-2.71 && \
    ./configure && \
    make && \
    make install && \
    cd .. && \
    rm -rf autoconf-2.71 autoconf-2.71.tar.gz

# Install Google Test
RUN cd /usr/src/gtest && \
    cmake . && \
    make && \
    cp lib/*.a /usr/lib && \
    cd / && \
    rm -rf /usr/src/gtest

# Download and install Boost 1.85.0
RUN wget https://archives.boost.io/release/1.85.0/source/boost_1_85_0.tar.gz && \
    tar xfz boost_1_85_0.tar.gz && \
    cd boost_1_85_0 && \
    ./bootstrap.sh --prefix=/usr/local && \
    ./b2 install --with=all --without-stacktrace && \
    cd .. && \
    rm -rf boost_1_85_0 boost_1_85_0.tar.gz

# Copy project files into the container
COPY . /workspace

# Give tests script execute permissions
RUN chmod +x /workspace/scripts/git-commit-file.sh
RUN chmod +x /workspace/scripts/run_tests.sh
RUN chmod +x /workspace/scripts/run_benchmarks.sh
RUN chmod +x /workspace/scripts/cleanup_prometheus_files.sh
RUN chmod +x /workspace/scripts/commit_suite.sh

# Add explicit compiler flags to enable exceptions
RUN export CXXFLAGS="-fexceptions" && \
    export CFLAGS="-fexceptions"

# Configure and build the project
#RUN cmake -S . -B build && \
#    cmake --build build

# Default command to keep the container running
# CMD ["/bin/bash"]

# Add cron job to run the cleanup script every minute
# Create the cron job file in /etc/cron.d/ and set correct permissions.
RUN echo "* * * * * root /workspace/scripts/cleanup_prometheus_files.sh >> /var/log/cleanup.log 2>&1" > /etc/cron.d/cleanup_prometheus && \
    chmod 0644 /etc/cron.d/cleanup_prometheus && \
    touch /var/log/cleanup.log

# Run cron in the foreground
CMD ["cron", "-f"]
#CMD ["tail", "-f", "/dev/null"]
