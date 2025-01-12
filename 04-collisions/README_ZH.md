## 处理冲突

哈希函数将无限多的输入映射到有限数量的输出上。不同的输入键可能会映射到相同的数组索引，导致存储桶发生冲突。哈希表必须实现某种方法来应对这种冲突。

我们的哈希表将采用一种称为双重哈希开放定址的技术来处理冲突。双重哈希利用两个哈希函数来计算在发生`i`次冲突后，一个项目应存储的索引位置。

关于其他类型的冲突解决方法概览，请参阅[附录](../07-appendix/README_ZH.md)。

## 双重哈希

在`i`次冲突后应使用的索引由以下公式给出：

```
index = hash_a(string) + i * hash_b(string) % num_buckets
```

我们看到，如果没有发生冲突，`i = 0`，所以索引仅仅是字符串的`hash_a`结果。如果发生冲突，索引将通过`hash_b`进行调整。

有可能`hash_b`会返回0，从而使第二项变为0。这将导致哈希表反复尝试将项目插入同一个存储桶中。我们可以通过在第二个哈希结果上加1来缓解这种情况，以确保结果永远不会是0。

```
index = (hash_a(string) + i * (hash_b(string) + 1)) % num_buckets
```

## 实现

```c
// hash_table.c
static int ht_get_hash(
    const char* s, const int num_buckets, const int attempt
) {
    const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}
```

下一节: [哈希表方法](../05-methods/README_ZH.md)  
