# Bに貼る
nfdc face create udp://10.244.2.71
nfdc route add prefix /B nexthop udp://10.244.2.71
# Cに貼る
nfdc face create udp://10.244.9.130
nfdc route add prefix /C nexthop udp://10.244.9.130

/home/NDN-FC-WorkflowPlus/work/NDN-original/ndn-cxx/build/examples/node_a_producer