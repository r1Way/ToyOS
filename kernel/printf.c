#include <stdarg.h>
#include "types.h"
#include "sbi.h"


/**
 * 通过 SBI 调用输出单个字符
 */
void console_putc(char c)
{
    sbi_call(1, c, 0, 0);  // SBI 调用号 1 是 console_putchar
}

// 功能与console_putc 完全相同，只是为了sys_myHelloWorld 函数的实现,重新写了一遍
void sbi_console_putchar(char c)
{
    sbi_call(1, c, 0, 0);  // SBI 调用号 1 是 console_putchar
}

/**
 * 输出字符串
 */
void printstr(const char *str)
{
    if (!str) {
        printstr("(null)");
        return;
    }
    
    while (*str) {
        console_putc(*str);
        str++;
    }
}

/**
 * 字符串反转函数
 */
static void reverse(char *str, int length)
{
    int start = 0;
    int end = length - 1;
    
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

/**
 * 将整数转换为字符串
 * @param num 要转换的数字
 * @param str 存储结果的字符串缓冲区
 * @param base 进制（10 或 16）
 * @return 字符串长度
 */
int itoa(int num, char *str, int base)
{
    int i = 0;
    int is_negative = 0;
    
    // 处理 0 的特殊情况
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return 1;
    }
    
    // 处理负数（仅对十进制）
    if (num < 0 && base == 10) {
        is_negative = 1;
        num = -num;
    }
    
    // 逐位转换
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }
    
    // 添加负号
    if (is_negative) {
        str[i++] = '-';
    }
    
    str[i] = '\0';  // 添加字符串结束符
    
    // 反转字符串
    reverse(str, i);
    
    return i;
}

/**
 * 打印整数
 * @param num 要打印的数字
 * @param base 进制（10 为十进制，16 为十六进制）
 */
void printint(int num, int base)
{
    char buffer[32];  // 足够大的缓冲区
    
    // 对于十六进制，添加 "0x" 前缀
    if (base == 16) {
        printstr("0x");
    }
    
    itoa(num, buffer, base);
    printstr(buffer);
}

__attribute__((format(printf, 1, 2)))
void printf(const char *fmt, ...)
{
    va_list list;
    va_start(list, fmt);

    for (;;)
    {
        switch (*fmt)
        {
            // 结束 printf
            case '\0':
                va_end(list);
                return;

            // 变量
            case '%':
                fmt++;
                switch (*fmt)
                {
                    case '%':
                        console_putc('%');
                        break;
                    case 'd':
                        printint(va_arg(list, int), 10);
                        break;
                    case 'x':
                        printint(va_arg(list, int), 16);
                        break;
                    case 's':
                        printstr(va_arg(list, const char *));
                        break;
                    default:
                        console_putc(*fmt);
                        break;
                }
                break;

            default:
                console_putc(*fmt);
                break;
        }
        fmt++;
    }
    va_end(list);
}

uint64 sys_myHelloWorld()
{
    const char *str ="Hello, World!\n";
    for(int i=0;str[i]!='\0';i++){
        sbi_console_putchar(str[i]);  // 使用 SBI 调用输出字符
    }

    while(1) {}// 防止程序退出（裸机环境下需要）
}