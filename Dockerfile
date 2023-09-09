# Fetch ubuntu image
FROM ubuntu:22.04

# Install build tools
RUN apt update && \
    apt install -y wget build-essential autoconf automake libtool
    
# Copy project into image
RUN mkdir project
COPY src project/Test/src
COPY tests project/Test/tests
COPY Makefile project/Test/Makefile

# Download and build CppUTest
RUN mkdir /project/Test/tools/ && \
    cd /project/ && \
    wget https://github.com/cpputest/cpputest/releases/download/v4.0/cpputest-4.0.tar.gz && \
    tar xf cpputest-4.0.tar.gz && \
    mv cpputest-4.0/ Test/tools/cpputest/ && \
    cd Test/tools/cpputest/ && \
    autoreconf -i && \
    ./configure && \
    make
    
# Execute script
ENTRYPOINT ["make", "test", "-C", "/project/"]