# Bに貼る
nfdc face create udp://10.244.18.19
nfdc route add prefix /B nexthop udp://10.244.18.19
# Cに貼る
nfdc face create udp://10.244.18.19
nfdc route add prefix /C nexthop udp://10.244.18.19
# Dに貼る
nfdc face create udp://10.244.18.19
nfdc route add prefix /D nexthop udp://10.244.18.19
# Eに貼る
nfdc face create udp://10.244.18.19
nfdc route add prefix /E nexthop udp://10.244.18.19
# Xに貼る
nfdc face create udp://10.244.18.19
nfdc route add prefix /X nexthop udp://10.244.18.19
# Yに貼る
nfdc face create udp://10.244.18.20
nfdc route add prefix /Y nexthop udp://10.244.18.20

/home/NDN-FC-WorkflowPlus/work/NDN-original/ndn-cxx/build/examples/node_c_producer