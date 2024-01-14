# NDN-FC-WorkflowPlus

NDN-FC+ を拡張し、ワークフローを追加したもの。


## コードについて

ルーターなどのコードを変更する必要はないので、クライアントコードのみを変えます。

NFD に関しては全く変更していません。ndn-cxx 自体も全く変更していません。

`work/NDN-original/ndn-cxx/examples` 内でクライアントコードを書いて実験しています。

`base_producer.cpp` の `BaseProducer` を継承して `run` だけ適当にオーバーライドして実行することでそれぞれのプロデューサーを作成しています。

コンシューマーは基本的に1個しか使っていなかったため、ベタ書き。

## 実験方法

実験に使ったコードはブランチに分けてあるのでそれぞれ確認してください。

それぞれのブランチの README に詳細が書いてあります。

基本的にはリソースが関係あるものだけ実験室の k8s を使っており、それ以外はローカルで動かします。

`/work/NDN-original/sh_local` や `/work/NDN-original/sh_k8s` に実験のために使うシェルスクリプトをまとめてあります。

ここを使う場合は自身の環境や実験内容に合わせて `face create` 先のIPアドレスを変えるなどする必要があります。


### ローカルでの実験

docker compose にて node-a, b, c のコンテナを使って実験していますが、なぜか同コンテナに入っても ndn の設定が個別で行われるようです。

同じコンテナに入ったとしても ndn-start や face create はシェルごとに行う必要があることに注意してください。

#### コンテナ構築

```
docker compose up
```

#### コンテナに入る
```
docker compose run ndn-node-a bash
docker compose run ndn-node-b bash
docker compose run ndn-node-c bash
```

#### 実行(一例)

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
# マルチで実験する場合
kubectl apply -f ./k8s/multi

# シングルで実験する場合
kubectl apply -f ./k8s/single
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