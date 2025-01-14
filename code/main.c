#include "hash_table.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 测试插入和搜索功能
void test_ht_insert_and_search()
{
    ht_hash_table *ht = ht_new();

    // 插入一个键值对并验证
    ht_insert(ht, "key1", "value1");
    assert(strcmp(ht_search(ht, "key1"), "value1") == 0);

    // 插入另一个键值对并验证
    ht_insert(ht, "key2", "value2");
    assert(strcmp(ht_search(ht, "key2"), "value2") == 0);

    // 更新已存在的键值对并验证
    ht_insert(ht, "key1", "new_value1");
    assert(strcmp(ht_search(ht, "key1"), "new_value1") == 0);

    ht_del_hash_table(ht);
}

// 测试删除功能
void test_ht_delete()
{
    ht_hash_table *ht = ht_new();

    // 插入一个键值对并验证
    ht_insert(ht, "key1", "value1");
    assert(strcmp(ht_search(ht, "key1"), "value1") == 0);

    // 删除键值对并验证
    ht_delete(ht, "key1");
    assert(ht_search(ht, "key1") == NULL);

    // 删除不存在的键值对并验证
    ht_delete(ht, "nonexistent_key");
    assert(ht_search(ht, "nonexistent_key") == NULL);

    ht_del_hash_table(ht);
}

// 主函数，运行所有测试
int main()
{
    test_ht_insert_and_search();
    test_ht_delete();
    printf("All tests passed!\n");
    return 0;
}
