﻿# Barrage Storm

# 特徴
コンポーネント指向エンジン  
Photon SDKを利用したオンラインマルチプレイ機能

# 設計概要

### ComponentEngine

エンジンの基礎部分です。基本的なオブジェクトはこの名前空間に入っています。  
基本的にライブラリに依存しない設計にしていますが、transformは例外です。座標の管理などでSiv3Dと密接な連携が必要になるため、  
using Transform = SivTransformと型エイリアスを作ってコーディングしています。DirectX等を使う場合はTransformを新たに作り直す必要があります。  

本README執筆時点ではOpenSiv3D以外への移植がまだ考えられていないこと、GameObjectと密接に関わるTransformに必要な機能について分離等の設計がしっかりできていないので、インターフェース等を作っていません。他ライブラリとの親和性を考えるとそのうち作った方がよさそうですね。

### SivComponent

名前空間ではComponentEngine::Sivになっています。  
Siv3Dに関するコンポーネントの機能はここで実装しています。しかし全然追いついていません。

### PhotonComponent

Photonを使ったオンライン機能をコンポーネントにまとめたものです。しかし現在まとまっていると言える状況ではなく、初期化のために共通オブジェクトの実装が必要だったりしてかなり設計が甘い状況です。一応、きちんとコールバック受け取って動くことは確認が取れているので、扱いが不便なだけであり実用上の機能は揃っています。

# 運用の基本

SceneManager -> Scene -> GameObject -> Component　のような保持関係になっています。上位オブジェクトは Get[Hoge]().lock() で取得することができます。(シーン遷移するためにはSceneManagerまで辿って呼ぶ必要があるのでかなり面倒ですが、あまり頻繁に書く機能ではないので許容しています)

1. ComponentEngine::ISceneを継承したクラスを作ります。
1. ComponentEngine::AttachbleComponentを継承したクラスを作り、Start()やUpdate()に処理を書いていきます。Unityと同様です。
1. void IScene::Setup() override にてゲームオブジェクトを生成していきます。生成と同時にオブジェクトのポインタを受け取れる、CreateAndGetGameObject()がおすすめです。オブジェクトにはAddComponentでコンポーネントをくっつけていきましょう。
1. シーンマネージャーを作り、クラスを登録します。シーン登録については、Siv3Dのシーンマネージャーに似せています。最初に登録したシーンが最初のシーンになります。
1. SceneManager.UpdateCurrentScene()をゲームループで毎フレーム呼びます。

大まかにはこのような形です。わからないことがあればTwitterかSlackで連絡でもください。テストユーザー大歓迎です。

# ライブラリ
ビルドする場合はそれぞれインストールを行い、パス等を自力で通す必要があります

boost 1.72.0  


Photon Realtime SDK v4.1.15.2  
<https://www.photonengine.com/ja/sdks>

OpenSiv3D v0.4.2  
Copyright (c) 2008-2019 Ryo Suzuki  
Copyright (c) 2016-2019 OpenSiv3D Project  
<https://github.com/Siv3D/OpenSiv3D>



~~ICU  
Copyright © 1991-2020 Unicode, Inc.~~  

# ご意見・ご感想
<https://docs.google.com/forms/d/e/1FAIpQLSdV1MVUXTzXItOsQgxnE7dtvRE21YwIkWCQzmX6LoRa-X9hxA/viewform?usp=sf_link>

# 開発用メモ

RequireComponentが欲しい

Prefabの登録がクソすぎるので各クラスに分散させられるような仕組みが必要　シーン登録時にセットで登録か？


衝突判定エンジンに不具合アリ
コライダー（形状）をオフにしても反応するはず
IColliderからGetActiveが取れんのでCollisionObjectの作り直し

-----

敵タイプ
スタンダード
🎃
標準的　コア❇に向かって移動して狙ってくる

アタッカー
⚽
プレイヤーを標的として追尾してくる

ボマー
💣　🎆
コアに体当たりし、爆発する
HPをゼロにしても爆発して周囲に大ダメージ

爆発は敵弾扱い


🎃⚽💣🎆😀😨😱


-----