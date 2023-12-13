## 立ち上げ

```bash
docker compose up
```

## 入る

```bash
docker compose run ndn bash
```

## 初期設定手順

### インストール

`sh /work/NDN-original/setup.sh` で OK

```
cd /work/ndn-cxx
./waf configure --with-examples
./waf
./waf install
sudo ldconfig
cd /work/NFD
./waf configure
./waf
./waf install
sudo cp /work/nfd.conf /usr/local/etc/ndn/nfd.conf
```

## 共有ライブラリがあるかどうかの確認

```
ldd /work/ndn-cxx/build/examples/producer
```

### 実行

```
# nfd起動
nfd-start

cd /work/NDN-FC-plus/ndn-cxx-FC
or
cd work/NDN-original/ndn-cxx

# trivial producer app
./build/examples/producer

# trivial consumer app
./build/examples/consumer

# trivial consumer app with timers
./build/examples/consumer-with-timer
```

## エラー

```
ERROR: Failed to read configuration file: trust-schema.conf
```

## コンフィグ変更時

```
cp /work/nfd.conf /usr/local/etc/ndn/nfd.conf
```

## リビルド

```
sh setup.sh
nfd-start
sh prod_re.sh 
sh cons_re.sh
```

## 実行

```provider
cd /work/NDN-original/
sh setup.sh
nfd-start
sh prod_re.sh
```

```consumer
cd /work/NDN-original/
sh reinstall.sh
nfd-start
sh cons_re.sh
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
```

```consumer
cd /home/NDN-FC-WorkflowPlus/work/NDN-original
sh ./sh_k8s/setup.sh
nfd-start
ifconfig
nfdc face create udp://10.244.18.7
nfdc route add prefix / nexthop udp://10.244.18.7
./ndn-cxx/build/examples/examples/my-consumer
```

```producer
cd /home/NDN-FC-WorkflowPlus/work/NDN-original
sh ./sh_k8s/setup.sh
nfd-start
ifconfig
nfdc face create udp://10.244.2.69
nfdc route add prefix / nexthop udp://10.244.2.69
./ndn-cxx/build/examples/my-producer
```

```function
cd /home/NDN-FC-WorkflowPlus/work/NDN-original
sh ./sh_k8s/setup.sh
nfd-start
ifconfig
nfdc face create udp://10.244.2.69
nfdc route add prefix / nexthop udp://10.244.2.69
./ndn-cxx/build/examples/my-producer
```