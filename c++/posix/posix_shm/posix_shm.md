# 2.posix shm 函数
## 2.1 shm_open函数
### 功能：
用来创建或打开一个共享内存对象
### 原型：
<pre><code>int shm_open(const char* name, int oflag, mode_t mode);
</code></pre>

### 参数：
- name: 共享内存对象的名字
- oflag: 与open函数类似，可以是O_RDNONLY、O_RDWR，还可以按位或上O_CREAT、O_EXCL、O_TRUNC
- mode：此参数总是需要设置，如果oflag没有指定了O_CREAT，可以指定为0

### 返回值：
- 成功： 返回非负整数文件描述符
- 失败： -1

## 2.2 ftruncate函数
### 功能：
修改共享内存对象大小,也可以用于文件
### 原型：
<pre><code>int ftruncate(int fd, off_t length);
</code></pre>

### 参数：
- fd: 文件描述符
- length:长度

### 返回值：
- 成功： 0
- 失败： -1

## 2.3 fstat函数
### 功能：
获取共享内存对象信息
### 原型：
<pre><code>int fstat(int fd, struct stat *buf);
</code></pre>

### 参数：
- fd: 文件描述符
- buf:返回共享内存的状态

### 返回值：
- 成功： 0
- 失败： -1

### 附：
文件状态信息：
<pre><code> struct stat {
               dev_t     st_dev;         /* ID of device containing file */
               ino_t     st_ino;         /* Inode number */
               mode_t    st_mode;        /* File type and mode */
               nlink_t   st_nlink;       /* Number of hard links */
               uid_t     st_uid;         /* User ID of owner */
               gid_t     st_gid;         /* Group ID of owner */
               dev_t     st_rdev;        /* Device ID (if special file) */
               off_t     st_size;        /* Total size, in bytes */
               blksize_t st_blksize;     /* Block size for filesystem I/O */
               blkcnt_t  st_blocks;      /* Number of 512B blocks allocated */

               /* Since Linux 2.6, the kernel supports nanosecond
                  precision for the following timestamp fields.
                  For the details before Linux 2.6, see NOTES. */

               struct timespec st_atim;  /* Time of last access */
               struct timespec st_mtim;  /* Time of last modification */
               struct timespec st_ctim;  /* Time of last status change */

           #define st_atime st_atim.tv_sec      /* Backward compatibility */
           #define st_mtime st_mtim.tv_sec
           #define st_ctime st_ctim.tv_sec
           };
</code></pre>

## 2.4 shm_unlink函数
### 功能：
删除共享内存
### 原型：
<pre><code>int shm_unlink(const char* name);</code></pre>

### 参数：
- name: 共享内存的名字

### 返回值：
- 成功： 0
- 失败： -1

## 2.5 共享内存对象的映射
### 功能：
将共享内存对象映射到进程地址空间
### 原型：
<pre><code>void* mmap(void* addr, size_t len, int prot, int flags, int fd,off_t offset)</code></pre>

### 参数：
- addr: 要映射的起始地址，通常指定为NULL，让内核自动选择
- len：映射到进程地址空间的字节数
- prot：映射区保护方式
- flags：标志
- fd：文件描述符
- offset: 从文件头开始的偏移量

### 返回值：
- 成功： 成功返回映射到的内存区起始地址
- 失败： -1
