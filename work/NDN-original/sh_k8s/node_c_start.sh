# Aに貼る
nfdc face create udp://10.244.18.19
nfdc route add prefix /A nexthop udp://10.244.18.19
# Bに貼る
nfdc face create udp://10.244.18.20
nfdc route add prefix /B nexthop udp://10.244.18.20

/home/NDN-FC-WorkflowPlus/work/NDN-original/ndn-cxx/build/examples/node_c_producer