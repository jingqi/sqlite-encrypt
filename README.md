
sqlite-encrypt
===

SQLite3 官方的加密扩展 SEE(SQLite Encryption Extension) 是收费的。免费的加密功能需要自己实现。

sqlite-encrypt 的实现参考了另一个 SQLite 的加密实现 [botansqlite3](https://github.com/OlivierJG/botansqlite3)。
其基本思路是打开 SQLite3 的 `SQLITE_HAS_CODEC` 编译选项，然后自己实现 SEE 预留的接口函数，从而提供加密功能。涉及到的接口很少，实现和编译都很简单。

原来的 `botansqlite3` 的实现需要依赖 `botan` 库来提供基础加密功能，这里换用了其他加密方式，减少依赖包的大小以及编译难度。

编译依赖
---
* [nut](https://github.com/jingqi/nut) 辅助库，提供 MD5、AES、AES-CBC-PKCS5 等算法支持。
* [sqlite-amalgamation](http://www.sqlite.org/amalgamation.html) 从 SQLite3 官方下载即可，是 SQLite 源码的一种压缩发布方式，
目前已知支持的版本是 `sqlite-amalgamation-3150100`。

注意
---
`sqlite-amalgamation` 中的 `sqlite.c` 文件已经被 include 到 `sqlite-encrypt` 的其他文件中了，如果自己写编译脚本则**不要**将
这个文件加入到编译源文件列表中。参看 `sqlite-encrypt` 的工程文件。
