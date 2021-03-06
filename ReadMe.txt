﻿=================================
 Apache Log Analysis Program
 Ver 1.0
 (c)2017 Seito Miyazaki
=================================
最終更新：2017/11/26



・概要

このプログラムはApache HTTP サーバのアクセスログを解析し、いくつかの観点からの情報を表示します。



・インストール方法

同梱のApache.exeを実行してください。



・使用方法

1.  同梱のApache.exeを実行します。

2.  期間の開始日の有無を指定します。(Y：指定有、N:指定無)

	2-1. Yを入力した場合には開始日を 年/月/日 のフォーマットで指定します。(例:2017年1月4日の場合→2017/1/4)

	2-2. 開始日が正しければYを入力します。間違っていればNを入力し、再度開始日を指定します。

3.  期間の終了日の有無を指定します。(Y：指定有、N:指定無)

	3-1. Yを入力した場合には終了日を 年/月/日 のフォーマットで指定します。(例:2017年2月10日の場合→2017/2/10)

	3-2. 終了日が正しければYを入力します。間違っていればNを入力し、再度終了日を指定します。

4.  取り込みたいログファイルの数を指定します。

5.  ログファイル名を指定します(同時に1つまで入力できます)。

6.  自動的にログ解析を開始します。

7.  ファイル数を複数指定した場合には5・6をファイル数分だけ繰り返します。

8.  全てのファイルの解析が終わるとメニューが表示されます。

	・ Tを入力すると時間帯別のアクセス件数を表示します。
	・ Rを入力するとリモートアドレス別のアクセス件数を表示し、アドレス順にソートします。
	・ Fを入力するとリモートアドレス別のアクセス件数を表示し、アクセス件数順にソートします。

9.  それぞれの解析結果を表示後、何かキーを押下することでメニューへ戻ります。

10. メニューでEを入力するとプログラムを終了します。



・アンインストール方法

レジストリなどは使用していないので、フォルダごと削除してください。



・開発環境

使用言語: C++
コンパイラ: Visual Studio Express 2015


(c) 2017 - Seito Miyazaki, All Rights Reserved.

======================================================================================= 

・各設問について

(問1) アクセス件数の集計
Apache のアクセスログファイルを読み込み、以下に示すような観点でアクセス件数を集計したい。
- 各時間帯毎のアクセス件数を知りたい
- リモートホスト別のアクセス件数:
  アクセスの多いリモートホストの順にアクセス件数の一覧を表示する。
上記の要件を満たすプログラムを作成せよ。

第一項目についてはメニューのTで、第二項目についてはメニューのFにて実装しています。


(問2) 複数ファイルの対応
解析したいログファイルが複数に分割して保存されていることが判明した。
問1で作成したプログラムを改良して、入力として複数のログファイルを読み込めるようにせよ。

途中でファイル数を指定すれば、複数読み込むことが可能になっています。


(問3) 期間の指定
入力として与えられるファイルに含まれる全てのログの中から、
ある指定した期間(例えば、2017年4月1日～4月30日までなど)に絞って、
同様の集計を行いたい。問2で作成したプログラムを改良して、
そのためのオプションを追加せよ。

使用方法2、3の選択をYにすることで指定ができます。


(問4) 大規模データへの対応
解析したいアクセスログファイルのサイズが総計で10GB程度になることが判明した。
実行するマシンのメモリが2GBしかない場合でも、メモリ不足にならずに
動作するように問3で作成したプログラムを改良せよ。

具体的な対策として、
・同時に開くファイルは1つに限定
・1行ずつ処理
・集計に不必要な項目はメモリ上に保持しない
・vectorを用いた動的メモリ確保
これらによって、コンピュータ上のメモリの使用を必要最低限に抑えています。


