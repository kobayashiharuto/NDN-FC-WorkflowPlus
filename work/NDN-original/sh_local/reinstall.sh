nfd-stop
cd /work/NDN-original/ndn-cxx
./waf install
sudo ldconfig
cd /work/NDN-original/NFD
./waf install
sudo cp /work/nfd.conf /usr/local/etc/ndn/nfd.conf