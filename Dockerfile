FROM ubuntu:20.04

ENV PKG_CONFIG_PATH="/work/NDN-FC-plus/ndn-cxx-FC"
# ENV PKG_CONFIG_PATH="/work/ndn-cxx"
ENV LD_LIBRARY_PATH="/usr/local/lib"


RUN apt update
RUN apt install -y git g++ libpcap-dev libsystemd-dev
RUN DEBIAN_FRONTEND=noninteractive apt install -y pkg-config
RUN apt update && apt install -y build-essential python3-minimal \
  libboost-all-dev libssl-dev libsqlite3-dev libpcap-dev libsystemd-dev tcpdump dnsutils iputils-ping net-tools netcat \
  doxygen graphviz python3-pip sudo psmisc