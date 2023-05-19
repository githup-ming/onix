#include <onix/bitmap.h>
#include <onix/string.h>
#include <onix/assert.h>




// 构造位图
void bitmap_make(bitmap_t *map, int8 *bits, u32 length, u32 offset)
{
    map->bits = bits;
    map->length = length;
    map->offset = offset;
}

// 初始化位图
void bitmap_init(bitmap_t *map, int8 *bits, u32 length, u32 start)
{
    memset(bits, 0, length);
    bitmap_make(map, bits, length, start);
}

// 测试位图某一位是否为1
bool bitmap_test(bitmap_t *map, u32 index)
{
    assert(index >= map->offset);
    
    // 得到位图的索引
    idx_t idx = index - map->offset;

    // 位图数组中的字节
    u32 bytes = idx / 8;

    // 该字节中的哪一位
    u8 bits = idx % 8;

    assert(bytes < map->length);

    // 返回哪一位是否等于1
    return (map->bits[bytes] & (1 << bits));
}

// 设置位图某一位的值
void bitmap_set(bitmap_t *map, u32 index, bool value)
{
    // value 必须是二值
    assert(value == 0 || value == 1);

    assert(index >= map->offset);

    // 得到位图的索引
    idx_t idx = index - map->offset;

    // 位图数组的字节数
    u32 bytes = idx / 8;

    // 该字节中的哪一位
    u8 bits = idx % 8;

    if (value) {
        // 置位 1
        map->bits[bytes] |= (1 << bits);
    } else {
        //置位0
        map->bits[bytes] &= ~(1 << bits);
    }
    
}

// 从位图中得到连续的count 位
int bitmap_scan(bitmap_t *map, u32 count)
{
    int32 start = EOF; // 标记开始的位置
    u32 bits_left = map->length * 8; //剩余的位数
    u32 next_bit = 0; // 下一位
    u32 counter = 0;   // 计数器

    // 从头开始找
    while (bits_left-- > 0)
    {
        if (!bitmap_test(map, map->offset + next_bit)) {
            // 如果下一位没有占用，则计数器加1
            counter++;
        } else {
            // 否则计数器置为0， 继续寻找
            counter = 0;
        }
        //下一位，位置加1
        next_bit++;

        //找到数量一致，则设置开始的位置，结束
        if (counter == count) {
            start = next_bit - count;
            break;
        }
    }
    if (start == EOF) {
        return EOF;
    }
    //否则将找到的位，全部置为1
    bits_left = count;
    next_bit = start;
    while (bits_left--) {
        bitmap_set(map, map->offset + next_bit, true);
        next_bit++;
    }
    // 返回索引
    return start + map->offset;
}

#include <onix/debug.h>
#define LEN 2
u8 buf[LEN];
bitmap_t map;

void bitmap_tests()
{
    bitmap_init(&map, buf, LEN, 0);
    buf[0] = 0x49;
    buf[1] = 0x92;
    for (size_t i = 0; i < 33; i++)
    {
        idx_t idx = bitmap_scan(&map, 2);
        if (idx ==EOF)
        {
            LOGK("test finish\n");
            break;
        }
        LOGK("%d\n",idx);
        
    }
    
}
