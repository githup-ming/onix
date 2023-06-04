#ifndef __ONIX_LIST_H__
#define __ONIX_LIST_H__

#include <onix/types.h>

#define element_offset(type, member) (u32)(&((type *)0)->member)
#define element_entry(type, member, ptr) (type *)((u32)ptr - element_offset(type, member))


// 链表节点
typedef struct list_node
{
    struct list_node *prev;// 前一个节点
    struct list_node *next;// 下一个节点
} list_node_t;

// 链表
typedef struct list
{
    list_node_t head;// 头节点
    list_node_t tail;// 尾节点
} list_t;

// 初始化链表
void list_init(list_t *list);

// 在anchor 节点前面插入节点node
void list_insert_before(list_node_t *anchor, list_node_t *node);

// 在anchor 节点后面插入节点node
void list_insert_after(list_node_t *anchor, list_node_t *node);

// 插入到头结点后
void list_push(list_t *list, list_node_t *node);

// 移除头结点后的节点
list_node_t *list_pop(list_t *list);

// 插入到尾节点前
void list_pushback(list_t *list, list_node_t *node);

// 移除尾节点前的节点
list_node_t *list_popback(list_t *list);

// 查找链表中节点是否存在
bool list_search(list_t *list, list_node_t *node);

// 从链表中删除节点
void list_remove(list_node_t *node);

// 判断链表是否为空
bool list_empty(list_t *list);

// 获取链表长度
u32 list_size(list_t *list);




void list_test();

#endif