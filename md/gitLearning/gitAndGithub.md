# Git


## 指令清单
</br>
开始一个工作区（参见：git help tutorial）
-   clone      克隆一个仓库到一个新目录
-   init       创建一个空的 Git 仓库或重新初始化一个已存在的仓库

在当前变更上工作（参见：git help everyday）
-   add        添加文件内容至索引
-   mv         移动或重命名一个文件、目录或符号链接
-   reset      重置当前 HEAD 到指定状态
-   rm         从工作区和索引中删除文件

检查历史和状态（参见：git help revisions）
-   bisect     通过二分查找定位引入 bug 的提交
-   grep       输出和模式匹配的行
-   log        显示提交日志
-   show       显示各种类型的对象
-   status     显示工作区状态

扩展、标记和调校您的历史记录
-   branch     列出、创建或删除分支
-   checkout   切换分支或恢复工作区文件
-   commit     记录变更到仓库
-   diff       显示提交之间、提交和工作区之间等的差异
-   merge      合并两个或更多开发历史
-   rebase     在另一个分支上重新应用提交
-   tag        创建、列出、删除或校验一个 GPG 签名的标签对象


## 1. 使用Git
1. 安装
Ubuntu:
<pre><code>sudo install apt-get install git</code></pre>
源码安装经典三步：
<pre><code>/config
make .
sudo make install</code></pre>
2. 设置（注意--global参数代表所有仓库都用相同的Name和Email）
<pre><code>git config --global user.name "Your Name"
git config --global user.email "email@example.com"</code></pre>

## 2. git的基础使用
- 第一步：开创一个目录作为仓库：
<pre><code>mdir learngit</code></pre>
- 第二步：通过git inti命令把这个目录变为git可以管理的仓库：
<pre><code>git init</code></pre>
- 第三步：增加文件（假设新文件为readme.txt）
<pre><code>git add readme.txt
git add file1.txt file2.txt  //多文件的方式</code></pre>
- 第四步：把文件提交到仓库
<pre><code>git commit -m "wrote a readme file"</code></pre>
- 第五步：登陆github，创建一个新的仓库（假设创建了learngit仓库）
- 第六步：创建SSH Key
<pre><code>cd ~
ssh-keygen -t rsa -C "youremail@example.com"</code></pre>
  登陆GitHub，打开“Account settings”，“SSH Keys”页面。然后，点“Add SSH Key”，填上任意Title，在Key文本框里粘贴id_rsa.pub文件的内容.
  </br>
- 第七步：把github与本地仓库关联
<pre><code>git remote add origin git@github.com:michaelliao/learngit.git
//千万注意，把上面的michaelliao替换成你自己的GitHub账户名</code></pre>
- 第八步：把所有内容推送到远程仓库
&emsp; 如果远程库是空的，我们第一次推送master分支时，就加上-u参数，Git不但会把本地的master分支内容推送的远程新的master分支，还会把本地的master分支和远程的master分支关联起来，在以后的推送或者拉取时就可以简化命令。

  仓库为空：
<pre><code>git push -u origin master  </code></pre>
  仓库不为空：
<pre><code>git pull origin master //远程的master与本地的分支合并
git push origin master</code></pre>

## 3. 本地仓管理

- git status  查看仓库当前的状态

- git diff file  查看file文件修改了什么内容

- git log 查看commit历史记录

- git reset --hard HEAD^ 退回到上一个版本

- git reset --hard 1094a 推出commit

- git reflog 查看命令执行记录

- git checkout -- readme.txt 可以丢弃工作区的修改，实是用版本库里的版本替换工作区的版本，无论工作区是修改还是删除，都可以“一键还原”。

- git reset HEAD readme.txt 把暂存区的修改撤销掉（unstage），重新放回工作区

  注：当你不但改乱了工作区某个文件的内容，还添加到了暂存区时，想丢弃修改
  先执行这条命令，再执行上一条命令


- git rm test.txt 工作区删除文件，如果要彻底删除需要commit

- rm -r .git 删除当前目录的本地仓库


## 遇到的问题
1. 远程仓库包含本地不存在的提交
<pre><code>提示：更新被拒绝，因为远程仓库包含您本地尚不存在的提交。这通常是因为另外
提示：一个仓库已向该引用进行了推送。再次推送前，您可能需要先整合远程变更
提示：（如 'git pull ...'）。
提示：详见 'git push --help' 中的 'Note about fast-forwards' 小节。
</code></pre>
解决方法：<pre><code>git pull origin master</code></pre>

2. 拒绝合并无关历史
<pre><code>warning: 没有共同的提交
remote: Enumerating objects: 3, done.
remote: Counting objects: 100% (3/3), done.
remote: Total 3 (delta 0), reused 0 (delta 0), pack-reused 0
展开对象中: 100% (3/3), 完成.
来自 github.com:S-TRAVELER/LearningFiles
  × branch            master     -> FETCH_HEAD
  × [新分支]          master     -> origin/master
fatal: 拒绝合并无关的历史
</code></pre>
  解决方法：<pre><code>git pull origin master --allow-unrelated-histories </code></pre>
