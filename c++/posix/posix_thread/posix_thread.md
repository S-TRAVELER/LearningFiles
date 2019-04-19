# 3.posix thread 函数
## 3.1 pthread_create 函数
### 功能：
创建一个新的线程
### 原型：
<pre><code>int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*),void* arg);
</code></pre>

### 参数：
- thread：返回线程ID
- attr: 设置线程的属性，attr为NULL表示使用默认属性
- start_routine：是个函数地址，线程启动后要执行的函数
- arg：传给线程启动函数的参数

### 返回值：
- 成功： 0
- 失败： 错误码

### 注意：
- 传统的一些函数是，成功返回0，失败返回-1,并且对全局变量errno赋值指示错误。

- pthreads函数出错时不会设置全局变量errno（而大部分其他POSIX函数会这样做）。而是将错误代码通过返回值返回

- pthreads同样也提供了线程内的errno变量，以支持它使用errno的代码。对于pthreads函数的错误，建议通过返回值进行判断，因为读取返回值要比读取线程内的errno变量的开销更小

## 3.2 pthread_exit 函数
### 功能：
结束本线程（相当于exit）
### 原型：
<pre><code>void pthread_exit(void* retval);
</code></pre>

### 参数：
- retval：返回值

### 返回值：
 无


## 3.3 pthread_join 函数
### 功能：
等待线程结束（相当于waitpid）
### 原型：
<pre><code>int pthread_join(pthread_t thread, void **value_ptr);
</code></pre>

### 参数：
- thread：返回线程ID
- value_ptr: 它指向一个指针，后者指向线程的返回值

### 返回值：
- 成功： 0
- 失败： 错误码

## 3.4 pthread_self 函数
### 功能：
返回线程ID
### 原型：
<pre><code>pthread_t pthread_self(void);
</code></pre>

### 参数：
无
### 返回值：
- 成功： 0
- 失败： 错误码

## 3.5 pthread_cancel 函数
### 功能:
取消一个执行中的线程（相当于kill）
### 原型：
<pre><code>int pthread_cancel(pthread_t thread);
</code></pre>

### 参数：
- thread：返回线程ID

### 返回值：
- 成功： 0
- 失败： 错误码


## 3.6 pthread_detach 函数
### 功能:
拆分线程
### 原型：
<pre><code>int pthread_detach(pthread_t thread);
</code></pre>

### 参数：
- thread：返回线程ID

### 返回值：
- 成功： 0
- 失败： 错误码

## 3.7 线程状态函数
### 功能:
设置或获取线程状态
### 原型：
<pre><code>pthread_attr_t *attr, int detachstate) pthread_attr_setdetachstate (pthread_attr_t *attr, int detachstate)
pthread_attr_t *attr, int detachstate) pthread_attr_getdetachstate (pthread_attr_t *attr, int detachstate)
</code></pre>

### 参数：
- attr：线程状态储存变量
- detachstate：状态

### 返回值：
- 成功： 0
- 失败： 错误码

## 3.7 线程栈管理函数
### 功能:
设置或获取线程栈的大小
### 原型：
<pre><code>int pthread_attr_getstacksize (pthread_attr_t *attr, int stacksize)  

int pthread_attr_setstacksize (pthread_attr_t *attr, int stacksize)  

int pthread_attr_getstackaddr (pthread_attr_t *attr, void **stackaddr)  

int pthread_attr_setstackaddr pthread_attr_t *attr, void **stackaddr)
</code></pre>

### 参数：
- attr：线程状态储存变量
- stacksize: 栈的大小
- stackaddr: 栈的地址

### 返回值：
- 成功： 0
- 失败： 错误码

## 3.8 条件变量函数
### 功能:
创建和销毁条件变量
### 原型：
<pre><code>int pthread_cond_init (pthread_mutex_t *condition,pthread_attr_t *attr)  

int pthread_cond_destroy (pthread_mutex_t *condition)  

int pthread_condattr_init (pthread_attr_t *attr)  

int pthread_condattr_destroy (pthread_attr_t *attr)  
</code></pre>

### 参数：
- condition：条件变量
- attr：线程状态储存变量

### 返回值：
- 成功： 0
- 失败： 错误码
