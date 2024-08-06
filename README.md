# これは何？
ポーカースターズのhistoryを元にプレイヤープール全体のアクション傾向を分析するレポジトリです
機能は今後追加していきます

# 使い方
1.`src/input/data`にhistoryファイルを入れます。
    * 複数ファイル入れて問題ありません
    * zoomを想定しています。他のゲームでは動かないと思われます
2.`src/Environment/Environment.cpp`40行目の`YourID`を自分のスターズIDに書き換える
    * 書き換えなくても動作はしますが、エクスプロイトしたいのに自分のデータが混ざると邪魔なので...
3.`src/`元で`main.cpp`をコンパイルして実行

# 機能一覧
### RaitoOfFoldToRiverBet
* Riverの各種サイズのベットに対するfold率を求めます

comming soon...

# 環境
GCC 8.1以降 or Clang 7以降