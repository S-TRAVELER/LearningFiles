# 1. 编译
1. 生成编译时用的makefile,其中，--prefix用来指定libevent的安装目录。
<pre><code>./configure --prefix=/opt/libevent
</code></pre>
2. make 编译
3. make install

**检测是否已经完成安装**
1. 查看是否有相关的库
<pre><code> sudo find /usr/local -name "libevent.so"</code></pre>
2. 运行semple样例程序（推荐）
<pre><code>cd semple
./hello-world</code></pre>
打开新的终端（这里是ubuntu）
<pre><code>nc 127.0.0.1 9995</code></pre>

## 2. 使用libevent进行编译
<pre><code>gcc hello-world.c -o hello-world -levent
</code></pre>
