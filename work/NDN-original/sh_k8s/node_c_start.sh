# Aに貼る
nfdc face create udp://10.244.18.11
nfdc route add prefix /A nexthop udp://10.244.18.11
# Bに貼る
nfdc face create udp://10.244.18.12
nfdc route add prefix /B nexthop udp://10.244.18.12

./ndn-cxx/build/examples/node_c_producer