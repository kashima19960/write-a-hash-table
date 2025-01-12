# 调整大小

目前，我们的哈希表具有固定数量的桶。随着插入的项目越来越多，表开始填满。这会导致两个问题：

1. 哈希表的性能会随着碰撞率的增加而下降。
2. 我们的哈希表只能存储固定数量的项目。如果我们尝试存储更多项目，插入函数将失败。

为了缓解这个问题，我们可以在数组填满时增加其大小。我们将哈希表中存储的项目数量存储在表的 `count` 属性中。在每次插入和删除时，我们计算表的“负载”，即已填充桶与总桶数的比率。如果负载高于或低于某个值，我们将相应地调整桶的大小。

我们将进行以下调整：

- 如果负载 > 0.7，则扩大桶的大小。
- 如果负载 < 0.1，则缩小桶的大小。

为了调整大小，我们创建一个新的哈希表，其大小约为当前表的一半或两倍，并将所有未删除的项目插入其中。

我们的新数组大小应该是一个质数，大约为当前大小的两倍或一半。找到新的数组大小并不简单。为此，我们存储一个基本大小，这是我们希望数组的大小，然后将实际大小定义为大于基本大小的第一个质数。为了扩大大小，我们将基本大小加倍，并找到下一个更大的质数；为了缩小大小，我们将基本大小减半，并找到下一个更大的质数。

我们的基本大小从 50 开始。我们使用暴力方法来找到下一个质数，通过检查每个连续的数字是否为质数。虽然暴力方法听起来有些令人担忧，但实际上我们需要检查的数值很少，而且所花费的时间远远少于重新哈希表中每个项目所花费的时间。

首先，我们定义一个函数来找到下一个质数。我们将在两个新文件 `prime.h` 和 `prime.c` 中实现这一点。

```c
// prime.h
int is_prime(const int x);
int next_prime(int x);
```

```c
// prime.c

#include <math.h>

#include "prime.h"


/*
 * 返回 x 是否为质数
 *
 * 返回值：
 *   1  - 质数
 *   0  - 非质数
 *   -1 - 未定义（即 x < 2）
 */
int is_prime(const int x) {
    if (x < 2) { return -1; }
    if (x < 4) { return 1; }
    if ((x % 2) == 0) { return 0; }
    for (int i = 3; i <= floor(sqrt((double) x)); i += 2) {
        if ((x % i) == 0) {
            return 0;
        }
    }
    return 1;
}


/*
 * 返回 x 之后的下一个质数，如果 x 是质数则返回 x
 */
int next_prime(int x) {
    while (is_prime(x) != 1) {
        x++;
    }
    return x;
}
```

接下来，我们需要更新 `ht_new` 函数以支持创建特定大小的哈希表。为此，我们将创建一个新函数 `ht_new_sized`。我们将 `ht_new` 改为调用 `ht_new_sized`，并传入默认的初始大小。

```c
// hash_table.c
static ht_hash_table* ht_new_sized(const int base_size) {
    ht_hash_table* ht = xmalloc(sizeof(ht_hash_table));
    ht->base_size = base_size;

    ht->size = next_prime(ht->base_size);

    ht->count = 0;
    ht->items = xcalloc((size_t)ht->size, sizeof(ht_item*));
    return ht;
}


ht_hash_table* ht_new() {
    return ht_new_sized(HT_INITIAL_BASE_SIZE);
}
```

现在我们已经拥有了编写调整大小函数所需的所有部分。

在我们的调整大小函数中，我们首先确保不会尝试将哈希表的大小缩小到最小大小以下。然后我们初始化一个具有所需大小的新哈希表。所有非 `NULL` 或未删除的项目都将插入到新哈希表中。然后我们交换新旧哈希表的属性，最后删除旧哈希表。

```c
// hash_table.c
static void ht_resize(ht_hash_table* ht, const int base_size) {
    if (base_size < HT_INITIAL_BASE_SIZE) {
        return;
    }
    ht_hash_table* new_ht = ht_new_sized(base_size);
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];
        if (item != NULL && item != &HT_DELETED_ITEM) {
            ht_insert(new_ht, item->key, item->value);
        }
    }

    ht->base_size = new_ht->base_size;
    ht->count = new_ht->count;

    // 为了删除 new_ht，我们将其的大小和项目与 `ht` 交换  
    const int tmp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->size = tmp_size;

    ht_item** tmp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp_items;

    ht_del_hash_table(new_ht);
}
```

为了简化调整大小的操作，我们定义了两个小函数来分别处理扩大和缩小哈希表。

```c
// hash_table.c
static void ht_resize_up(ht_hash_table* ht) {
    const int new_size = ht->base_size * 2;
    ht_resize(ht, new_size);
}


static void ht_resize_down(ht_hash_table* ht) {
    const int new_size = ht->base_size / 2;
    ht_resize(ht, new_size);
}
```

为了执行调整大小的操作，我们在插入和删除时检查哈希表的负载。如果负载高于或低于预定义的阈值（0.7 和 0.1），我们分别进行扩大或缩小操作。

为了避免进行浮点数运算，我们将 `count` 乘以 100，然后检查它是否高于或低于 70 或 10。

```c
// hash_table.c
void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
    const int load = ht->count * 100 / ht->size;
    if (load > 70) {
        ht_resize_up(ht);
    }
    // ...
}


void ht_delete(ht_hash_table* ht, const char* key) {
    const int load = ht->count * 100 / ht->size;
    if (load < 10) {
        ht_resize_down(ht);
    }
    // ...
}
```

下一节：[附录](../07-appendix/README_ZH.md)  