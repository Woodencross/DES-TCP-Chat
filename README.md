# DES-TCP-Chat

基于DES加密的TCP命令行多线程通信

https://github.com/Woodencross/DES-TCP-Chat.git

> 环境Windows, VS2015

- Tips：
  
  - **重要**：bitset类型访问时低位在字符串的高位，高位在字符串的低位，如：
  
    ```c++
    bitset<4> a("1000");//a[0]=0, a[1]=0, a[2]=0, a[3]=1
  ```
  
    所以给bitset类型循环赋值的时候都是下标从大到小进行遍历
  
  - 先运行Server再运行Client自动连接，Client退出可重连，Server退出不可重连，没有做quit命令，退出直接右上角:x:
  
  - socket的inet_addr()和inet_ntoa()应该是过时了现在会报错，用inet_pton()和inet_ntop()
  
  - thread类在C++ 11及以上，还挺方便的...
  
  - 有两个bind()，好像是命名空间问题，返回int的bind()需要写成::bind()，在server的main()里
  
  - server的main()里用了一个goto
  
- 目录

  ```
  |-thisRepo
  	|-DES_TCP_Client				//客户端
  		|-DES_TCP_Client			//客户端代码
  			|-des.h
  			|-des.cpp
  			|-main.cpp
  		|-Release
  			|-DES_TCP_Client.exe	//客户端可执行程序
  		|-DES_TCP_Client.sln
  	|-DES_TCP_Server				//服务器
  		|-DES_TCP_Server			//服务器代码
  			|-des.h
  			|-des.cpp
  			|-main.cpp
  		|-Release
  			|-DES_TCP_Server.exe	//服务器可执行程序
  		|-DES_TCP_Server.sln
  	|-实验一.pdf
  ```

- 参考：

  - [Windows C++ socket 编程1](https://blog.csdn.net/xiaoquantouer/article/details/58001960), [Windows C++ socket 编程2](https://blog.csdn.net/uupton/article/details/82714798)
  - [inet_pton() 与 inet_ntop()](https://blog.csdn.net/zyy617532750/article/details/58595700)
  - [DES加密的C++实现](https://blog.csdn.net/lisonglisonglisong/article/details/41777413)
  - [C++ thread类实现多线程](https://blog.csdn.net/dcrmg/article/details/53912941)