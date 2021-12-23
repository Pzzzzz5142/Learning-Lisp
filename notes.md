# Chapter 4

## Preprocessor

用来跨平台的。有些时候在不同操作系统上的行为不同，因此需要使用 preprocessor 在编译阶段加以区分。

但因为我换 C++ 的 cin 了，所以暂时用不到这玩意。

## 课后题

1. What does the preprocessor command #ifndef do?

后接一个叫 MACRO 的东西，具体形式如下所示。

```
#ifdef MACRO

controlled text

#endif /* MACRO */
```

用来根据`MACRO`判断是否编译`controlled text`中的内容。

# Chapter 6

## ~~（自己给自己挖的坑）~~如何用 C++ 调用 C 的库？

用`extern`关键字。然后链接生成一个动态库。

# Chapter 7 

## 三目运算符不能确定重载。

平时写 python 写习惯了，动态类型真的很好用，但是 C++ 没有那么自由。三目运算符的返回值在编译阶段就已经确定了，而不是在运行时可以动态调整了，所以在给函数传递参数的时候不能通过三目运算符达到执行不同重载函数的作用。

# Chapter 8

## 采用 Exception 来完成 Error Handling。

因为我选择用 C++ 来实现，所以错误处理与教程中的方法有些许的不一样，具体做法参考的[这里](https://docs.microsoft.com/en-us/cpp/cpp/errors-and-exception-handling-modern-cpp?view=msvc-170)。