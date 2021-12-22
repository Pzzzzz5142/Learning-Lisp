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

