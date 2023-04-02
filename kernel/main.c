#include <onix/onix.h>
#include <onix/io.h>
#include <onix/string.h>
#include <onix/console.h>
#include <onix/stdarg.h>

void test_args(int cnt, ...)  //参数传递从右向左压入栈中
{
    va_list args;

    va_start(args, cnt);//(args = (va_list)&cnt + sizeof(int8 *))

    va_list args1 = (va_list)&cnt + sizeof(int8 *);
    int a = sizeof(int8 *);

    int arg;
    while (cnt--)
    {
        arg = va_arg(args, int);//arg = (*(int *)((args += sizeof(int8 *)) - sizeof(int8 *)))
        int arg1 = (*(int *)((args += sizeof(int8 *)) - sizeof(int8 *)));
    }
    va_end(args);//(args = (va_list)NULL)
    


}
void kernel_init()
{

    console_init();
    test_args(6,4,5,6,7,8,9);

    return;

}