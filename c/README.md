# Ｃ言語課題

このリポジトリには、C言語で取り組んだ複数のプログラミング課題が含まれています。各フォルダには、個別の課題のソースコードが含まれています。

## 課題一覧

### 課題1: 四点が長方形を形成するか判定するプログラム
- **概要**: 四点が与えられたとき、それらが長方形を形成するかどうかを判定するプログラムを作成します。入力は2D平面上の四つの点で、それぞれの点はx座標とy座標で表されます。点の順序が長方形の境界に沿っている場合にのみ正しく判定され、順序が異なる場合は否定されます。
- **ポイント**: 入力データのバリデーションが必要で、不正な入力にはエラーメッセージを出力します。

### 課題2: 異星文明からのメッセージ解析プログラム
- **概要**: 異星文明から受信したメッセージを解析し、それらが同期する時刻を計算するプログラムを作成します。メッセージは小文字の文字列で与えられ、各文字は異なる長さのパルスを表します。
- **ポイント**: メッセージの同期を計算し、同期しない場合は特定のメッセージを出力します。入力データのバリデーションも行います。

### 課題3: 平日計算関数の実装
- **概要**: 特定の日付が平日かどうかを判定する関数と、指定された日付範囲内の平日数をカウントする関数を実装します。
- **ポイント**: 年、月、日が有効な日付かを確認し、不正な日付の場合は特定のエラーメッセージを出力します。

### 課題4: 城の制圧領域を計算する関数の実装
- **概要**: 城が制圧する領域の面積を計算する関数を実装します。地図はn×nの正方形のタイルに分割され、それぞれのタイルの標高が知られています。城は任意のタイルに位置し、そのタイルを含む長方形の領域を制圧します。制圧する領域はできるだけ広く、かつ城がその領域内で最も高いタイルに位置している必要があります。
- **ポイント**: 各タイルに城が位置する場合に制圧できる最大の長方形領域の面積を計算し、結果を2D配列に格納します。

### 課題5: Eショップのレビュー統計データ解析プログラム
- **概要**: Eショップで収集されたユーザーレビューの統計データを解析し、指定された評価合計に最も近いレビューの区間を見つけるプログラムを作成します。
- **ポイント**: 評価合計が最も近い区間を見つけ、その区間の開始日と終了日を出力します。レビューが不正な形式の場合はエラーメッセージを出力します。

### 課題8: 疎行列を扱う関数の実装
- **概要**: 疎行列を扱うための一連の関数を実装します。疎行列は、多くの値が設定されていない（ゼロと見なされる）行列です。
- **ポイント**: 単一リンクリストを用いて、行列の効率的な表現を実装します。行と列は昇順で格納されます。

<img src="https://github.com/michiaka/teamlab_portfolio_2024/assets/155388365/e729deb8-6c3c-4e6e-b58e-90eef189c192" width="400">

