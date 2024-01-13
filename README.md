## local

### コンテナ構築

```
docker compose up
```

### コンテナに入る
```
docker compose run ndn-node-a bash
docker compose run ndn-node-b bash
docker compose run ndn-node-c bash
```

### 実行

```node-a
sh /work/NDN-original/sh_local/setup.sh
nfd-start
sh /work/NDN-original/sh_local/node_a_start.sh
```

```node-b
sh /work/NDN-original/sh_local/reinstall.sh
nfd-start
sh /work/NDN-original/sh_local/node_b_start.sh
```

```node-c
sh /work/NDN-original/sh_local/reinstall.sh
nfd-start
sh /work/NDN-original/sh_local/node_c_start.sh
```

```node-a(consumer)
sh /work/NDN-original/sh_local/reinstall.sh
nfd-start
sh /work/NDN-original/sh_local/node_a_consumer_start.sh
```


## k8s

### 実行
```
kubectl apply -f ./k8s
```

### コンテナに入る
```
kubectl get pods
kubectl exec -it ポッド名 -- /bin/bash
```

### それぞれにコードを持ってくる

```
cd /home
git clone https://github.com/kobayashiharuto/NDN-FC-WorkflowPlus.git
export PKG_CONFIG_PATH="/home/NDN-FC-WorkflowPlus/work/NDN-original"
```

```A_producer
cd /home/NDN-FC-WorkflowPlus/work/NDN-original/sh_k8s/
sh rebuild.sh
nfd-start
sh node_a_start.sh
```

```_B
cd /home/NDN-FC-WorkflowPlus/work/NDN-original/sh_k8s/
sh rebuild.sh
nfd-start
sh node_b_start.sh
```

```_C
cd /home/NDN-FC-WorkflowPlus/work/NDN-original/sh_k8s/
sh rebuild.sh
nfd-start
sh node_c_start.sh
```


```A_consumer
/home/NDN-FC-WorkflowPlus/work/NDN-original/ndn-cxx/build/examples/node_a_consumer
```