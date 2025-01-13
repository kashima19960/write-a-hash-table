# 哈希表结构

我们的键值对（items）将分别存储在一个 `struct` 中：

```c
// hash_table.h
typedef struct {
    char* key;
    char* value;
} ht_item;
```

哈希表则存储一个指向这些 items 的指针数组，以及一些与大小和占用情况相关的细节：

```c
// hash_table.h
typedef struct {
    int size;
    int count;
    ht_item** items;
} ht_hash_table;
```

## 初始化与删除

我们需要为 `ht_item` 定义一个初始化函数。该函数会为 `ht_item` 分配一块内存，并将字符串 `k` 和 `v` 的副本保存到新分配的内存中。我们将此函数标记为 `static`，因为它只会在哈希表内部调用。

```c
// hash_table.c
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

static ht_item* ht_new_item(const char* k, const char* v) {
    ht_item* i = malloc(sizeof(ht_item));
    i->key = strdup(k);
    i->value = strdup(v);
    return i;
}
```

`ht_new` 函数用于初始化一个新的哈希表。`size` 定义了我们可以存储多少个 items。目前我们将它固定为 53，稍后在[扩容部分](/06-resizing)会对其进行扩展。我们使用 `calloc` 初始化 items 数组，`calloc` 会将分配的内存填充为 `NULL` 字节。数组中的 `NULL` 条目表示该桶是空的。

```c
// hash_table.c
ht_hash_table* ht_new() {
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));

    ht->size = 53;
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
    return ht;
}
```

我们还需要用于删除 `ht_item` 和 `ht_hash_table` 的函数，这些函数会释放我们分配的内存，以免造成[内存泄漏](https://en.wikipedia.org/wiki/Memory_leak)。

```c
// hash_table.c
static void ht_del_item(ht_item* i) {
    free(i->key);
    free(i->value);
    free(i);
}

void ht_del_hash_table(ht_hash_table* ht) {
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];
        if (item != NULL) {
            ht_del_item(item);
        }
    }
    free(ht->items);
    free(ht);
}
```

至此，我们已经完成了哈希表的定义，并编写了创建和销毁哈希表的代码。尽管目前它还不能执行太多功能，但我们可以尝试运行一下。

```c
// main.c
#include "hash_table.h"

int main() {
    ht_hash_table* ht = ht_new();
    ht_del_hash_table(ht);
}
```

下一节：[哈希函数](/03-hashing) 