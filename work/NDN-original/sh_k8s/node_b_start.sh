# Aに貼る
nfdc face create udp://10.244.18.11
nfdc route add prefix /A nexthop udp://10.244.18.11
# Cに貼る
nfdc face create udp://10.244.18.10
nfdc route add prefix /C nexthop udp://10.244.18.10

/home/NDN-FC-WorkflowPlus/work/NDN-original/ndn-cxx/build/examples/node_b_producer