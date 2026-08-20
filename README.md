# Mac M3 で implot を動かす

## ソースをダウンロド

https://github.com/ocornut/imgui
今回はzipファイルを入手

## Metal バージョン確認

バックエンドとして色んなものがつかえるらしいが、Metal というopenGL的なやつのMac版があるので今回はそれを使う。
バージョンを確認すると、Metal 4 であることがわかる

```shell-session
$ system_profiler SPDisplaysDataType
Graphics/Displays:

    Apple M3:

      Chipset Model: Apple M3
      Type: GPU
      Bus: Built-In
      Total Number of Cores: 10
      Vendor: Apple (0x106b)
      Metal Support: Metal 4
      ...
```

## サンプルを実行

解凍したディレクトリに `examples/example_apple_metal4` があるので実行してみる
警告がでてきたがウィンドウが現れたので大丈夫そう

```shell-session
$ make run
...
../..//backends/imgui_impl_metal4.mm:138:34: warning: comparison of integers of different signs: 'int' and 'NSUInteger' (aka 'unsigned long') [-Wsign-compare]
...

```

![image.png](https://qiita-image-store.s3.ap-northeast-1.amazonaws.com/0/3859059/95fd1ab4-98bb-402d-b9ef-820fc021c7bc.png)

# 参考

https://qiita.com/zacky1972/items/2609711bec7fd806128b

https://shizenkarasuzon.hatenablog.com/entry/2020/03/21/000515

https://dexall.co.jp/articles/?p=2157#i-8
