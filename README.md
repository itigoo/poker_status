# これは何？
ポーカースターズのhistoryを元にプレイヤープール全体のアクション傾向を分析するレポジトリです
機能は今後追加していきます

# 使い方
1.`src/input/data`にhistoryファイルを入れます

　- 複数ファイル入れて問題ありません
 
　- zoomを想定しています。他のゲームでは動かないと思われます

2.`src/Environment/Environment.cpp`40行目の`YourID`を自分のスターズIDに書き換える

3.`src/`元で`main.cpp`をコンパイルして実行

# 機能一覧

##機能変更

`src/main.cpp`の`analyse`内の文字列を変えることで使う機能を指定できます
使える機能は以下の通りです。

### FrequencyOfFoldToRiverBet
* Riverの各種サイズのベットに対するfold率を求めます

### GameTreeExplorer
* checkとbetのみで構成される各ゲームツリーにおけるbetへのfold率を調べます

* ゲームツリーはx:check,B:betを用いてcallを除いた文字列で表されます

    * 例：xBxxB flop : xbc, turn : xx, river : Bc

comming soon...

# 環境
GCC 8.1以降 or Clang 7以降
