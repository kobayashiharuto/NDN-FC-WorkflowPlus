nfd-stop

cd /home/NDN-FC-WorkflowPlus/work/NDN-original/ndn-cxx
./waf configure --with-examples
./waf
./waf install
sudo ldconfig
cd /home/NDN-FC-WorkflowPlus/work/NDN-original/NFD
./waf configure
./waf
./waf install
sudo cp /home/NDN-FC-WorkflowPlus/work/nfd.conf /usr/local/etc/ndn/nfd.conf