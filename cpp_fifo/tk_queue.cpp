#include "tk_queue.h"
#include <string.h>
/**
 * @brief 静态初始化队列
 * @param queuepool 队列缓存区
 * @param pool_size 缓存区大小(单位字节)
 * @param queue_size 队列元素大小(单位字节)
 * @param keep_fresh 是否为保持最新模式,true：保持最新 false：默认(存满不能再存)
 * @return true 初始化成功
 * @return false 初始化失败
 */
bool tk_queue::tk_queue_init(void *queuepool, long pool_size, long queue_size, bool keep_fresh)
{
    if (queuepool == NULL)
        return false;
    this->keep_fresh = keep_fresh;
    this->queue_pool = queuepool;
    this->queue_size = queue_size;
    this->max_queues = pool_size / this->queue_size;
    this->front = 0;
    this->rear = 0;
    this->len = 0;
    return true;
}
/**
 * @brief 清空队列
 * @return true 清除成功
 * @return false 清除失败
 */
bool tk_queue::tk_queue_clean(void)
{
    this->front = 0;
    this->rear = 0;
    this->len = 0;
    return true;
}
/**
 * @brief 查询队列当前数据长度
 * @return long 队列数据当前长度(元素个数)
 */
long tk_queue::tk_queue_curr_len(void)
{
    return (this->len);
}
/**
 * @brief 判断队列是否为空
 * @return true 空
 * @return false 不为空
 */
bool tk_queue::tk_queue_empty(void)
{
    if (this->len == 0)
        return true;
    return false;
}
/**
 * @brief 判断队列是否已满
 * @return true 满
 * @return false 不为满
 */
bool tk_queue::tk_queue_full(void)
{
    if (this->len >= this->max_queues)
        return true;
    return false;
}
/**
 * @brief 向队列压入(入队)1个元素数据
 * @param val 压入值
 * @return true 成功
 * @return false 失败
 */
bool tk_queue::tk_queue_push(void *val)
{
    if (this->len >= this->max_queues)
    {
        if (this->keep_fresh == true)
        {
            memcpy((uint8_t *)this->queue_pool + (this->rear * this->queue_size),
                   (uint8_t *)val,
                   this->queue_size);
            this->rear = (this->rear + 1) % this->max_queues;
            this->front = (this->front + 1) % this->max_queues;
            this->len = this->max_queues;
            return true;
        }
        return false;
    }
    else
    {
        memcpy((uint8_t *)this->queue_pool + (this->rear * this->queue_size),
               (uint8_t *)val,
               this->queue_size);
        this->rear = (this->rear + 1) % this->max_queues;
        this->len++;
    }
    return true;
}
/**
 * @brief 从队列弹出(出队)1个元素数据
 * @param pval 弹出值
 * @return true 成功
 * @return false 失败
 */
bool tk_queue::tk_queue_pop(void *pval)
{
    if (this->len == 0)
    {
        return false;
    }
    else
    {
        memcpy((uint8_t *)pval,
               (uint8_t *)this->queue_pool + (this->front * this->queue_size),
               this->queue_size);
        this->front = (this->front + 1) % this->max_queues;
        this->len--;
    }
    return true;
}
/**
 * @brief 向队列压入(入队)多个元素数据
 * @param pval 压入元素首地址
 * @param len 压入元素个数
 * @return long 实际压入个数
 */
long tk_queue::tk_queue_push_multi(void *pval, long len)
{
    if (this->len + len > this->max_queues)
    {
        if (this->keep_fresh == true)
        {
            // 先把剩余空间填充完
            long push_len1 = this->max_queues - this->len;
            memcpy((uint8_t *)this->queue_pool + (this->rear * this->queue_size),
                   (uint8_t *)pval,
                   this->queue_size * push_len1);
            // 这里正常来说 this->rear 应该为0了
            this->rear = (this->rear + push_len1) % this->max_queues;
            // 在从头开始填充
            long push_len2 = len - push_len1;
            memcpy((uint8_t *)this->queue_pool + (this->rear * this->queue_size),
                   (uint8_t *)pval + (push_len1 * this->queue_size),
                   this->queue_size * push_len2);
            this->rear = (this->rear + push_len2) % this->max_queues;
            this->front = (this->front + push_len2) % this->max_queues;
            this->len = this->max_queues;
            return len;
        }
        return 0;
    }
    else
    {
        memcpy((uint8_t *)this->queue_pool + (this->rear * this->queue_size),
               (uint8_t *)pval,
               this->queue_size * len);
        this->rear = (this->rear + len) % this->max_queues;
        this->len += len;
        return len;
    }
    // uint8_t *u8pval = (uint8_t *)pval;
    // long push_len = 0;
    // while (len-- && tk_queue_push(u8pval) == true)
    // {
    // push_len++;
    // u8pval += this->queue_size;
    // }
    // return push_len;
}
/**
 * @brief 从队列弹出(出队)多个元素数据
 * @param pval 存放弹出元素的首地址
 * @param len 希望弹出的元素个数
 * @return long 实际弹出个数
 */
long tk_queue::tk_queue_pop_multi(void *pval, long len)
{
    long pop_len = 0;
    if (this->len == 0)
    {
        return pop_len;
    }
    else if (len > this->len) // 全部取出
    {
        pop_len = this->len;
    }
    else
    {
        pop_len = len;
    }
    if (this->front + pop_len > this->max_queues)
    {
        // 先把尾部取出
        long pop_len1 = this->max_queues - this->front;
        memcpy((uint8_t *)pval,
               (uint8_t *)this->queue_pool + (this->front * this->queue_size),
               this->queue_size * pop_len1);
        // 这里正常来说 this->front 应该为0了
        this->front = (this->front + pop_len1) % this->max_queues;
        this->len -= pop_len1;
        // 在取开头部分
        long pop_len2 = pop_len - pop_len1;
        memcpy((uint8_t *)pval + (pop_len1 * this->queue_size),
               (uint8_t *)this->queue_pool + (this->front * this->queue_size),
               this->queue_size * pop_len2);

        this->front = (this->front + pop_len2) % this->max_queues;
        this->len -= pop_len2;
    }
    else
    {
        memcpy((uint8_t *)pval, 
               (uint8_t *)this->queue_pool + (this->front * this->queue_size), 
               this->queue_size * pop_len);
        this->front = (this->front + pop_len) % this->max_queues;
        this->len -= pop_len;
    }
    return pop_len;
    // long pop_len = 0;
    // uint8_t *u8pval = (uint8_t *)pval;
    // if (this->len == 0)
    // return false;
    // while (len-- && tk_queue_pop(u8pval) == true)
    // { // pop_len++;
    // u8pval += this->queue_size;
    // }
    // return pop_len;
}
/**
 * @brief 从队列中读取一个元素(不从队列中删除)
 * @param pval 读取值地址
 * @return true 读取成功
 * @return false 读取失败
 */
bool tk_queue::tk_queue_peep(void *pval)
{
    if (this->len == 0)
    {
        return false;
    }
    else
    {
        memcpy((uint8_t *)pval, 
               (uint8_t *)this->queue_pool + (this->front * this->queue_size), 
               this->queue_size);
    }
    return true;
}
/**
 * @brief 移除一个元素
 * @return true 移除成功
 * @return false 移除失败
 */
bool tk_queue::tk_queue_remove(void)
{
    if (this->len == 0)
    {
        return true;
    }
    this->front = (this->front + 1) % this->max_queues;
    this->len--;
    return true;
}
