// tk_tools_test.cpp : 定义控制台应用程序的入口点。
// #include "stdafx.h"
#include <iostream>
#include <iomanip>
// 用于格式化输出
#include "tk_queue.h"

tk_queue g_tk_queue;

#define USBBUFFERMAXSIZE 90
uint8_t *usb_buffer;
uint8_t push_data[100] = {0};
uint8_t pop_data[50] = {0};
/* print queue */
void printf_queue(char *str, tk_queue &q)
{
    int i, j;
    long queue_front = q.front;
    std::cout << str << " " << q.len << std::endl;
    for (i = 0; i < q.len; i++)
    {
        for (j = 0; j < q.queue_size; j++)
        {
            std::cout << " " << (int)*((uint8_t *)(q.queue_pool) + queue_front * q.queue_size + j);
        }
        std::cout << ",";
        queue_front = (queue_front + 1) % q.max_queues;
    }
    std::cout << std::endl;
}

int main()
{
    setbuf(stdout,NULL);//debug调试 打印数据
    usb_buffer = new uint8_t[USBBUFFERMAXSIZE]; // 创建一个队列
    g_tk_queue.tk_queue_init(usb_buffer, sizeof(uint8_t) * USBBUFFERMAXSIZE, sizeof(uint8_t), true);
    // 初始化完
    printf_queue("建栈", g_tk_queue);
    // 建立 100 个数据
    for (int i = 0; i < 50; i++)
        push_data[i] = i;
    // 入栈测试
    g_tk_queue.tk_queue_push_multi(push_data, 50);
    printf_queue("入栈", g_tk_queue);
    // 入栈测试
    g_tk_queue.tk_queue_push_multi(push_data, 50);
    printf_queue("入栈", g_tk_queue);
    // 出栈测试
    g_tk_queue.tk_queue_pop_multi(pop_data, 50);
    printf_queue("出栈", g_tk_queue);
    delete[] usb_buffer;
    return 0;
}
