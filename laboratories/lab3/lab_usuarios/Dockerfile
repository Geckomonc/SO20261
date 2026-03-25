FROM debian:stable-slim

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    sudo \
    passwd \
    login \
    procps \
    nano \
    vim \
    libpam-pwquality \
    && rm -rf /var/lib/apt/lists/*

CMD ["/bin/bash"]
