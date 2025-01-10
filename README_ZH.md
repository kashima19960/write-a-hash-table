> 译者留：本文由[木人舟](https://github.com/kashima19960)翻译，[原文地址](https://github.com/kashima19960/write-a-hash-table)。由于译者本身的翻译水平有限，如有错误请多多包涵并指出

# 用C语言编写一个哈希表

[哈希表](https://en.wikipedia.org/wiki/Hash_table)是最有用的数据结构之一。其快速的插入、查找和删除操作使其在众多计算机科学问题中广泛适用。

在本教程中，我们将使用C语言实现一个[开放寻址](https://en.wikipedia.org/wiki/Open_addressing)、[双重哈希](https://en.wikipedia.org/wiki/Double_hashing)的哈希表。通过学习本教程，你将获得：

- 理解基础数据结构内部运行的原理
- 深入了解何时使用哈希表、何时不使用，以及它们可能失败的原因
- 接触新的C语言代码

C语言是编写哈希表的理想选择，因为：

- 该语言本身没有内置哈希表
- C语言作为低级语言，能够深入理解在机器层面的运行机制

本教程假设读者具备一定的编程知识和C语言语法基础。代码本身相对简单，大部分问题可以通过网络搜索解决。

完整实现代码大约200行，学习过程预计需要一到两个小时。

## 目录

1. [简介](./01-introduction/README_ZH.md)
2. [哈希表结构](./02-hash-table/README_ZH.md)
3. [哈希函数](./03-hashing/README_ZH.md)
4. [处理冲突](./04-collisions/README_ZH.md)
5. [哈希表操作方法](./05-methods/README_ZH.md)
6. [调整哈希表大小](./06-resizing/README_ZH.md)
7. [附录：替代冲突处理方法](./07-appendix/README_ZH.md)

## 致谢

本教程由[James Routley](https://twitter.com/james_routley)编写，他的博客地址是[routley.io](https://routley.io)。
