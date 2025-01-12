# 方法

我们的哈希函数将实现以下API：

```c
// hash_table.h
void ht_insert(ht_hash_table* ht, const char* key, const char* value);
char* ht_search(ht_hash_table* ht, const char* key);
void ht_delete(ht_hash_table* h, const char* key);
```

## 插入

为了插入一个新的键值对，我们会遍历索引直到找到一个空桶。接着将项插入该桶，并增加哈希表的 `count` 属性，表示添加了新项。当我们在下一部分中探讨[调整大小](/06-resizing)时，哈希表的 `count` 属性将变得非常有用。

```c
// hash_table.c
void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
    ht_item* item = ht_new_item(key, value);
    int index = ht_get_hash(item->key, ht->size, 0);
    ht_item* cur_item = ht->items[index];
    int i = 1;
    while (cur_item != NULL) {
        index = ht_get_hash(item->key, ht->size, i);
        cur_item = ht->items[index];
        i++;
    } 
    ht->items[index] = item;
    ht->count++;
}
```

## 查找

查找类似于插入，但在 `while` 循环的每次迭代中，我们检查项的键是否与正在查找的键匹配。如果匹配，我们返回该项的值。如果循环遇到 `NULL` 桶，我们返回 `NULL`，表示未找到该值。

```c
// hash_table.c
char* ht_search(ht_hash_table* ht, const char* key) {
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;
    while (item != NULL) {
        if (strcmp(item->key, key) == 0) {
            return item->value;
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    } 
    return NULL;
}
```

## 删除

从开放地址哈希表中删除项比插入或查找更为复杂。我们希望删除的项可能是冲突链的一部分。将其从表中移除会破坏该链，导致无法在链尾部找到其余项。为解决此问题，我们不删除项，而是简单地将其标记为已删除。

我们通过将项替换为指向全局标记为删除的项（哨兵项）的指针来标记项为已删除。

```c
// hash_table.c
static ht_item HT_DELETED_ITEM = {NULL, NULL};

void ht_delete(ht_hash_table* ht, const char* key) {
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                ht_del_item(item);
                ht->items[index] = &HT_DELETED_ITEM;
            }
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    } 
    ht->count--;
}
```

删除后，我们减少哈希表的 `count` 属性。

我们还需要修改 `ht_insert` 和 `ht_search` 函数，以考虑已删除的节点。

在查找时，我们忽略并“跳过”已删除的节点。在插入时，如果遇到已删除的节点，我们可以将新节点插入已删除的位置。

```c
// hash_table.c
void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
    // ...
    while (cur_item != NULL && cur_item != &HT_DELETED_ITEM) {
        // ...
    }
    // ...
}

char* ht_search(ht_hash_table* ht, const char* key) {
    // ...
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM) { 
            if (strcmp(item->key, key) == 0) {
                return item->value;
            }
        }
        // ...
    }
    // ...
}
```

## 更新

我们当前的哈希表尚不支持更新键的值。如果插入两个具有相同键的项，键将发生冲突，第二个项将被插入到下一个可用桶中。当查找该键时，总是会发现原始键，而我们无法访问到第二个项。

我们可以通过修改 `ht_insert`，在插入新项之前删除旧项，并在其位置插入新项来解决这个问题。

```c
// hash_table.c
void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
    // ...
    while (cur_item != NULL) {
        if (cur_item != &HT_DELETED_ITEM) {
            if (strcmp(cur_item->key, key) == 0) {
                ht_del_item(cur_item);
                ht->items[index] = item;
                return;
            }
        }
        // ...
    } 
    // ...
}
```

下一节内容：[调整表大小](../06-resizing/README_ZH.md)