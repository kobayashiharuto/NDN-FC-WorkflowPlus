nfd-stop

cd /work/NDN-original/ndn-cxx
./waf configure --with-examples
./waf
./waf install
sudo ldconfig
cd /work/NDN-original/NFD
./waf configure
./waf
./waf install
sudo cp /work/nfd.conf /usr/local/etc/ndn/nfd.conf