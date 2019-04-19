# 信号与槽

信号与槽是由对象系统（meta object system）提供的，类似的可以应用于Qt 文本翻译上。使用信号与槽，有以下几点要求：
- 类必须直接或间接继承与QObject

- 必须加入Q_OBJECT宏，Q_OBJECT宏声明了每一个QObject子类中必须实现的一些内省函数：metaObject()、tr()、qt_metacall()，以及一些其他函数

- 信号类似成员函数，但只需声明不需要定义，其的返回值一般为void，但可以以参数方式向槽函数传递参数。

- 信号与槽连接时，签名式（即参数和返回值）必须相同

- connect连接的信号与槽可以通过取地址符直接取对应的地址，或者使用SIGNAL与SLOT进行包装，但后者更好用。

## 1. 一个信号连接多个槽
<pre><code>connect(slider,SIGNAL(valueChanged(int)),
  spindBox,SLOT(setValue()));
connect(slider,SIGNAL(valueChanged(int)),
  this,SLOT(updateStatusBarIndicator(int)));
</code></pre>

## 2. 多个信号连接同一个槽
<pre><code>connect(lcd,SIGNAL(overflow()),
  this,SLOT(handleMathError()));
connect(caculator,SIGNAL(divisionByZero()),
  this,SLOT(handleMathError()));
</code></pre>

## 3. 一个信号链接另一个信号
<pre><code>connect(lineEdit,SIGNAL(textChanged(const QString&)),
  this,SLOT(handleMathError()));
</code></pre>

## 4. 移除连接
<pre><code>disconnect(lcd,SIGNAL(overflow()),
  this,SLOT(handleMathError()));
</code></pre>
