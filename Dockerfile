FROM ubuntu:trusty

RUN apt-get update && apt-get install -y \
    curl \
    cmake \
    g++ \
 && apt-get clean \
 && ldconfig \
 && rm -rf /var/lib/apt/lists/*

#
# install boost as a dynamic library, only specific sub libraries.
#
ENV BOOST_MINOR_VERSION="58"
ENV BOOST_VERSION=1_${BOOST_MINOR_VERSION}_0

# I cannot build boost 1.58.0 from github...
# If you want to use newer version, get from github.
ENV BOOST_URL=http://downloads.sourceforge.net/project/boost/boost/1.${BOOST_MINOR_VERSION}.0/boost_${BOOST_VERSION}.tar.gz

# DL and build stage separately, make docker's cache work.
RUN mkdir boost_${BOOST_VERSION} \
  && curl -sSLR ${BOOST_URL} | tar xz --strip-components 1 -C boost_${BOOST_VERSION}

ENV BOOST_LIBS=filesystem,system,regex
RUN cd boost_${BOOST_VERSION} \
  && ./bootstrap.sh --prefix=/usr/local --without-icu --with-libraries=${BOOST_LIBS}  \
  && echo "using gcc : 4.8.4 : g++ : <compileflags>-fPIC <cxxflags>-std=c++11 ;" > ~/user-config.jam \
  && ./b2 link=shared --disable-icu -j$(nproc) \
  && ./b2 install \
  && ldconfig \
  && cd / \
  && rm -rf boost_${BOOST_VERSION}* \
  && rm -rf ~/user-config.jam

ENV TERM=xterm

WORKDIR /usr/src/app

CMD ["tail", "-f", "/dev/null"]
