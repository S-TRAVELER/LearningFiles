## 1. 使用流程
1. 创建一个事件处理框架
2. 创建一事件
3. 将事件添加至事件处理框架上
4. 开始事件循环监听
5. 释放资源

## 2. event_base相关函数
- 创建event_base
 - struct event_base* event_base_new(void);
 - 失败返回NULL
- 释放event_base
 - event_base_free(struct event_base *base);
- 监听base对应的事件
 - event_base_dispatch();
- 事件循环（dispatch的简化版）
 - int event_base_loop(struct event_base* base, int flags);
- 查看event_base封装的后端
 - const char** event_get_supported_methods(void);  
 一般为: <pre><code>select
 poll
 epoll</code></pre>
 - const char* event_get_supported_method(const event_base* base);  
 上面的方法三选一，一般为epoll

## 3. 事件
事件分为不带缓冲区事件和带缓冲区事件两种: event、buffevent

### 3.1 event

<pre><code>typedef void (*event_callback_fn)(evutil_socket_t, short, void*)

struct event* event_new(
  struct event_base *base,
  evutil_socket_t fd, //文件描述符，int
  short what,
  event_callback_fn cb,
  void *arg
  );</code></pre>
事件类型：
<pre><code>#define EV_TIMEOUT 0x01 //废弃
#define EV_READ 0x02
#define EV_WRITE 0x04
#define EV_SIGNAL 0x08
#define EV_PERSIST 0x10   //持续模式
#define EV_ET 0x20
</code></pre>

### 3.2 buffevent
头文件
<pre><code>event2/buffevent.h</code></pre>
buffevent有一个底层的传输端口（如套接字），一个读取缓冲区和一个写入缓冲区（一旦有数据就自动发送过去）组成。只有在读取或写入足够的数据才会触发回调。          
注意：在读回调函数中实现读操作，在写回调函数中实现提示功能。
从缓冲区读取
<pre><code>bufferevent_read();</code></pre>
向缓冲区写入
<pre><code>bufferevent_write();</code></pre>


## 4. 使用bufferevent

**创建bufferevent**
<pre><code>struct buffevent* bufferevent_socket_new{
    struct event_base *base,
    evutil_socket_t fd,     
    enum buffevent_options option //常用的option： BEV_OPT_CLOSE_ON_FREE
};</code></pre>

**设置回调函数**
<pre><code>typedef void (*bufferevent_data_cb) (struct bufferevent *bev,
  void *ctx
);
typedef void (*bufferevent_event_cb)(
  struct bufferevent *bev,
  short events,
  void* ctx
);

void bufferevent_setcb(
  struct bufferevent* bufev,
  bufferevent_data_cb readcb,
    --在读回调函数中读数据
    -- bufferevent_read()
  bufferevent_data_cb writecb,
  bufferevent_event_cb eventcb,
    --处理连接成功、断开连接、操作异常
  void *cbarg
);</code></pre>

**在bufferevent上启动连接（客户端）**
<pre><code>int bufferevent_connect(
  struct bufferevent* bev,
  struct sockaddr* address, //server ip和port
  int addrlen
);
  --address和addrlen参数跟标准调用connect()的参数相
  同。如果还没有为bufferevent设置套接字，调用函数将为
  其分配一个新的流套接字，并且设置为非阻塞。
  --如果已经为bufferevent设置套接字，调
  用bufferevent_socket_connect()将告知libevent套
  接字未连接，直到连接成功之前不应该对其进行读取或写入操
  作 </code></pre>

**启动监听（服务端）**
- 流程
  - 创建监听的套接字
  - 绑定
  - 监听
  - 接受连接请求

- 连接回调函数
<pre><code>typedef void (*evconnlistener_cb)(
    struct evconnlistener *listener,
    evutil_socket_t sock,   //通信描述符
    struct sockaddr *addr,  //客户端的IP和端口信息
    int socklen,
    void *ptr //外部传来的数据
);</code></pre>
- 创建监听器
<pre><code>struct evconnlistener *evconnlistener_new(
    struct event_base *base,
    evconnlistener_cb cb,
    void *ptr,
    unsigned flags,
    int backlog,
    evutil_socket_t fd  //已经绑定的socket描述符
);</code></pre>
- 绑定并创建监听
<pre><code>struct evconnlistener *evconnlistener_new_bind(
    struct event_base *base,
    evconnlistener_cb cb,
      --实现连接之后的操作
    void *ptr,
    unsigned flags,
      --参考手册P99-100
      --常用 LEV_OPT_CLOSE_ON_FREE、LEV_OPT_REUSEABLE
    int backlog,
      --一般使用-1,默认最大值
    const struct sockaddr *sa,
      --服务器端口信息
    int socklen
);
</code></pre>

**禁用、启用缓冲区**
<pre><code>void bufferevent_enable
  struct bufferevent *bufev,
  short events
);
void bufferevent_disable(
  struct bufferevent *bufev,
  short events
);
short bufferevent_get_enabled(  //查看其读写是否可用
  struct buffevent *bufev
);
</code></pre>
注意：
- EV_WRITE默认是enable，EV_READ默认是disenable。
- 可以通过以上的函数启用或禁用buffevent的EV_READ、EV_WRITE或EV_READ|EV_WRITE。
- 禁用之后，对应的回调函数就不会被调用
