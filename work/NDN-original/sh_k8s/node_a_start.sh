# Bに貼る
nfdc face create udp://10.244.2.72
nfdc route add prefix /B nexthop udp://10.244.2.72
# Cに貼る
nfdc face create udp://10.244.9.131
nfdc route add prefix /C nexthop udp://10.244.9.131

/home/NDN-FC-WorkflowPlus/work/NDN-original/ndn-cxx/build/examples/node_a_producer