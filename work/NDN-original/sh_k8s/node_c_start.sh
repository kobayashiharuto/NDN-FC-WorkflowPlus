# Aに貼る
nfdc face create udp://10.244.18.16
nfdc route add prefix /A nexthop udp://10.244.18.16
# Bに貼る
nfdc face create udp://10.244.2.71
nfdc route add prefix /B nexthop udp://10.244.2.71

/home/NDN-FC-WorkflowPlus/work/NDN-original/ndn-cxx/build/examples/node_c_producer