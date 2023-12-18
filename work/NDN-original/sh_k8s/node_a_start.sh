# Bに貼る
nfdc face create udp://10.244.18.20
nfdc route add prefix /B nexthop udp://10.244.18.20
# Cに貼る
nfdc face create udp://10.244.18.18
nfdc route add prefix /C nexthop udp://10.244.18.18

/home/NDN-FC-WorkflowPlus/work/NDN-original/ndn-cxx/build/examples/node_a_producer