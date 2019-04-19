# Linux GCC
## 1. 接触GCC
&emsp;GCC（GUN Compiler Collection）是Linux下的编译工具。包含gcc、g++、ar、nm等工具。
&emsp;GCC工具支持的语言有：Objective-C、Pascal、Fortran、Java、Ada等。
<pre><code>Usage: gcc [options] file...</code></pre>

## 2. GCC编译步骤
C和C++编译器是集成的.他们都要用四个步骤中的一个或多个处理输入文件:

```
graph LR
预处理preprocessing-->编译compilation
编译compilation-->汇编assembly
汇编assembly-->连接linking
```

<pre><code>.c      C源程序;预处理,编译,汇编
.C      C++源程序;预处理,编译,汇编
.cc     C++源程序;预处理,编译,汇编
.cxx    C++源程序;预处理,编译,汇编
.m      Objective-C源程序;预处理,编译,汇编
.i     预处理后的C文件;编译,汇编
.ii    预处理后的C++文件;编译,汇编
.s     汇编语言源程序;汇编
.S     汇编语言源程序;预处理,汇编
.h     预处理器文件;通常不出现在命令行上
其他后缀名的文件被传递给连接器(linker).通常包括:
.o     目标文件(Object file)
.a     归档库文件(Archive file)
</code></pre>

&emsp;除非使用了-c, -S,或-E选项(或者编译错误阻止了完整的过程),否则连接总是 最后的步骤.在连接阶段中,所有对应于源程序的.o文件, -l库文件,无法识别的文件名(包括指定的 .o目标文件和.a库文件)按命令行中的顺序传递给连接器.

[了解更多gcc选项 | GUN GCC 手册](http://www.shanghai.ws/gnu/gcc_1.htm)

## 3. 单文件编译
假设现有一个C语言代码文件test.c，则编译命令：
<pre><code> gcc test.c
</code></pre>

gcc编译后默认生成一个名为a.out的执行文件，运行命令：
<pre><code>./a.out</code></pre>

-o: 可以通过-o选项指定生成文件的名称：
<pre><code>gcc -o test test.c
./test</code></pre>

-c: 将源文件生成目标文件（默认与源文件同名test.o），而不是执行文件：
<pre><code>gcc -c test.c
gcc -c -o test.o test.c</code></pre>

-S: 生成汇编文件

-E: 把将文件预处理的结果显示在计算机屏幕上

## 4. 多文件编译
多文件编译时时，需把所有的.c文件都作为gcc的file参数。头文件不需包含。
<pre><code>gcc -o test string.c main.c
</code></pre>
<pre><code>gcc -c add/add_int.c -o add/add_int.o
gcc -c add/add_float.c -o add/add_float.o
gcc -c sub/sub_int.c -o sub/sub_int.o
gcc -c sub/sub_float.c -o sub/sub_float.o
gcc -c main.c -o main.o
gcc -o cacu add/add_int.o add/add_float.o sub/sub_int.o sub_float.o main.o
</code></pre>

## 5. 静态库
1. 生成静态库  
以上面string.c为例，先生成一个目标文件：
<pre><code>gcc -c string.c</code></pre>
再把string.o 打包成libstr.a ：
<pre><code>ar -rcs libstr.a string.o</code></pre>

2. 使用静态库
  - 直接在源文件后面家已编译好的静态链接库libstr.a
<pre><code>gcc -o test main.c libstr.a</code></pre>
  - 使用命令 "-l库名"
<pre><code>gcc -o main.c -lstr</code></pre>
上面的命令会在系统默认的路径下查找str函数库，并把它链接到目标程序上。所以执行上面的命令可能会出错。这需要显示指定库函数的路径：
<pre><code>gcc -o test main.c -L./ -lstr</code></pre>

## 6. 动态库
1. 生成动态库  
使用-fPIC选项或者-fpic选项。
<pre><code>gcc -shared -Wl, -soname,libstr.so -o libstr.so.1 string.c</code></pre>
默认目录：
<pre><code>/lib
/usr/lib
/usr/loval/lib</code></pre>

2. 配置动态库
