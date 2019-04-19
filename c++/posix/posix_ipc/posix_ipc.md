# 1. posix ipc 函数
## 1.1 mq_open函数
### 功能：
用来创建和访问一个消息队列
### 原型：
<pre><code>mqd_t mq_open(const char* name, int oflag);
mqd_t mq_open(const char* name,int oflag, mode_t mode, struct mq_attr *attr);
</code></pre>

### 参数：
- name: 某个消息队列的名字,必须以‘/’开头，并且不能有其它的‘/’，形如”/pueuename“，长度不能超过NAME_MAX
- oflag: 与open函数类似，可以是O_RDONLY、O_WRONLY、O_RDWR，还可以按位或上O_CREAT、O_EXCL、O_NONBLOCK等。
- mode: 如果oflag指定O_CRET，需设置mode。

### 返回值：
- 成功：队列文件描述符
- 失败：-1

**Example**
<pre><code>#include < unistd.h>
#include < sys/types.h>
#include < fcntl.h>
#include < sys/stat.h>
#include < mqueue.h>

#include < stdlib.h>
#include < stdio.h>
#include < errno.h>

#define ERR_EXIT(m) \
	do{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	}while(0)

int main(void){
	mqd_t mqid;
	mqid=mq_open("/abc",O_CREAT|O_RDWR,0666,NULL);
	if(mqid==(mqd_t)-1){
		printf("打开队列失败\n");
	}
	else{
		printf("打开队列成功\n");
	}

	return 0;
}</code></pre>

### 查看消息队列
- posix的实现并非用系统消息队列实现的，因此，无法用ipcs-命令查看。
<pre><code>man 7 mq_overview
cat /dev/mqueue/abc</code></pre>
- 可以看出posix的消息队列其实是个文件，因此删除它就很简单了：
<pre><code>rm -r abc</code></pre>

- 挂载:
<pre><code>mount -t mqueue none /dev/mqueue</code></pre>
- 卸载：
<pre><code>sudo umount mqueue/</code></pre></code></pre>
- 查看连接数：
<pre><code>ls -li /dev/mqueue/abc

## 1.2 mq_close函数
### 功能：
关闭消息队列
### 原型：
<pre><code>mqd_t mq_close(mqd_t mqdes);</code></pre>

### 参数：
- mqdes: 消息队列描述符

### 返回值：
- 成功： 0
- 失败： -1

## 1.3 mq_unlink函数
### 功能：
删除消息队列
### 原型：
<pre><code>mqd_t mq_unlink(const char* name);</code></pre>

### 参数：
- name: 消息队列的名字

### 返回值：
- 成功： 0
- 失败： -1

## 1.4 mq_getattr/mq_setattr函数
### 功能：
获取/设置消息队列属性
### 原型：
<pre><code>mqd_t mq_getattr(mqd_t mqdes,struct mq_attr *attr);
mqd_t mq_setattr(mqd_t mqdes,struct mq_attr *newattr, struct mq_attr *oldattr);</code></pre>

### 参数：
- mqdes: 消息队列描述符
- attr: 消息队列属性

### 返回值：
- 成功： 0
- 失败： -1

## 1.5 mq_send函数
### 功能：
发送消息
### 原型：
<pre><code>mqd_t mq_send(mqd_t mqdes,const char* msg_ptr, size_t msg_len, unsigned msgprio);
</code></pre>

### 参数：
- mqdes: 消息队列描述符
- msg_ptr: 指向消息的指针
- msg_len: 消息长度
- msg_prio: 消息优先级

### 返回值：
- 成功： 0
- 失败： -1

### 查看发送后的消息队列大小
<pre><code>cat /dev/mqueue/abc
</code></pre>

## 1.6 mq_receive函数
### 功能：
接收消息，注意： 返回的是指定消息队列中最高优先级的最早消息
### 原型：
<pre><code>mqd_t mq_send(mqd_t mqdes,const char* msg_ptr, size_t msg_len, unsigned* msgprio);
</code></pre>

### 参数：
- mqdes: 消息队列描述符
- msg_ptr: 返回可接收到的消息的指针
- msg_len: 消息长度
- msg_prio: 返回接收消息的优先级

### 返回值：
- 成功： 返回接收到的消息字节数
- 失败： -1

## 1.7 mq_notify函数
### 功能：
建立或者删除消息到达通知事件
### 原型：
<pre><code>mqd_t mq_notify(mqd_t mqdes,const struct sigevent *notification);
</code></pre>

### 参数：
- mqdes: 消息队列描述符
- notification:
 - 非空表示当消息到达且消息队列先前为空，那么将得到通知
 - NULL 表示撤销已经注册的通知

### 返回值：
- 成功： 0
- 失败： -1

### 通知方式：
- 产生一个信号
- 创建一个线程执行一个指定的函数

###注意：
- 任何时刻只能有一个进程可以被注册为接收某个给定队列的通知

- 当有一个消息到达某个先前为空的队列，而且已有一个进程被注册为接收该队列的通知时，只有没有任何线程阻塞在该队列的mq_receive调用的前提下，通知才会发出。

- 当通知被发送给它 的注册进程时，其注册被撤销。进程必须再次调用mq_notify以重新注册(如果需要的话）,重新注册要放在从消息队列读出消息之前而不是之后（防止receive阻塞，而无法接收其他消息）。

### 附
信号：
<pre><code> union sigval {          /* Data passed with notification */
           int     sival_int;         /* Integer value */
           void   *sival_ptr;         /* Pointer value */
       };</code></pre>

信号事件：
<pre><code> struct sigevent {
           int          sigev_notify; /* Notification method */
           int          sigev_signo;  /* Notification signal */
           union sigval sigev_value;  /* Data passed with
                                         notification */
           void       (*sigev_notify_function) (union sigval);
                            /* Function used for thread
                               notification (SIGEV_THREAD) */
           void        *sigev_notify_attributes;
                            /* Attributes for notification thread
                               (SIGEV_THREAD) */
           pid_t        sigev_notify_thread_id;
                            /* ID of thread to signal (SIGEV_THREAD_ID) */
       };
</code></pre>
