# Bに貼る
nfdc face create udp://10.244.18.12
nfdc route add prefix /B nexthop udp://10.244.18.12
# Cに貼る
nfdc face create udp://10.244.18.10
nfdc route add prefix /C nexthop udp://10.244.18.10

/home/NDN-FC-WorkflowPlus/work/NDN-original/ndn-cxx/build/examples/node_a_producer