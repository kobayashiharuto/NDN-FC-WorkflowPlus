# Bに貼る
nfdc face create udp://172.18.0.5
nfdc route add prefix /B nexthop udp://172.18.0.5
# Cに貼る
nfdc face create udp://172.18.0.5
nfdc route add prefix /C nexthop udp://172.18.0.5
# Dに貼る
nfdc face create udp://172.18.0.5
nfdc route add prefix /D nexthop udp://172.18.0.5
# Eに貼る
nfdc face create udp://172.18.0.5
nfdc route add prefix /E nexthop udp://172.18.0.5
# Fに貼る
nfdc face create udp://172.18.0.5
nfdc route add prefix /F nexthop udp://172.18.0.5
# Xに貼る
nfdc face create udp://172.18.0.5
nfdc route add prefix /X nexthop udp://172.18.0.5
# Zに貼る
nfdc face create udp://172.18.0.7
nfdc route add prefix /Z nexthop udp://172.18.0.7

/work/NDN-original/ndn-cxx/build/examples/node_b_producer