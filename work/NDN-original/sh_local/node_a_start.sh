# Yに貼る
nfdc face create udp://172.18.0.6
nfdc route add prefix /Y nexthop udp://172.18.0.6
# Zに貼る
nfdc face create udp://172.18.0.7
nfdc route add prefix /Z nexthop udp://172.18.0.7

/work/NDN-original/ndn-cxx/build/examples/node_a_producer