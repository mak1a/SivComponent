# OpenSiv3d OnlineGame

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

SceneManager -> Scene -> GameObject -> Component　のような保持関係になっています。上位オブジェクトは Get[Hoge]().lock() で取得することができます。(シーン遷移するためにはSceneManagerまで辿って呼ぶ必要があるのでかなり面倒ですが、あまり頻繁に書く機能ではないので…)

1. ComponentEngine::ISceneを継承したクラスを作ります。
1. ComponentEngine::AttachbleComponentを継承したクラスを作り、Start()やUpdate()に処理を書いていきます。Unityと同様です。
1. void IScene::Setup() override にてゲームオブジェクトを生成していきます。生成と同時にオブジェクトのポインタを受け取れる、CreateAndGetGameObject()がおすすめです。オブジェクトにはAddComponentでコンポーネントをくっつけていきましょう。
1. シーンマネージャーを作り、クラスを登録します。シーン登録については、Siv3Dのシーンマネージャーに似せています。最初に登録したシーンが最初のシーンになります。
1. SceneManager.UpdateCurrentScene()をゲームループで毎フレーム呼びます。

大まかにはこのような形です。わからないことがあればTwitterかSlackで連絡でもください。テストユーザー大歓迎です。

# ライブラリ
一切同梱していません。ビルドする場合はパス等を自力で通す必要があります

boost 1.72.0  


Photon Realtime SDK v4.1.15.2  
<https://www.photonengine.com/ja/sdks>

OpenSiv3D v0.4.2  
Copyright (c) 2008-2019 Ryo Suzuki  
Copyright (c) 2016-2019 OpenSiv3D Project  
<https://github.com/Siv3D/OpenSiv3D>



ICU  
Copyright © 1991-2020 Unicode, Inc.  