#pragma once
#include <stdbool.h>
#include <stdint.h>
class tk_queue
{
public:
    bool keep_fresh;  // 是否为保持最新模式,true：保持最新 false：默认(存满不能再存)
    void *queue_pool; // 队列池
    long queue_size;  // 队列池大小
    long max_queues;  // 最大队列数
    long front;       // 队首指针
    long rear;        // 队尾指针
    long len;         // 队列长度
    bool tk_queue_init(void *queuepool, long pool_size, long queue_size, bool keep_fresh);
    bool tk_queue_clean(void);
    long tk_queue_curr_len(void);
    bool tk_queue_empty(void);
    bool tk_queue_full(void);
    bool tk_queue_push(void *val);
    bool tk_queue_pop(void *pval);
    long tk_queue_push_multi(void *pval, long len);
    long tk_queue_pop_multi(void *pval, long len);
    bool tk_queue_peep(void *pval);
    bool tk_queue_remove(void);
};
