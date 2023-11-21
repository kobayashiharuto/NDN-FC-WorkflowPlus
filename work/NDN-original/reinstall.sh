nfd-stop

cd /work/NDN-original/ndn-cxx
./waf install
sudo ldconfig
cd /work/NDN-original/NFD
./waf install
sudo cp /usr/local/etc/ndn/nfd.conf.sample /usr/local/etc/ndn/nfd.conf