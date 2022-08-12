#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

struct threadpool_t
{
    pthread_mutex_t lock;   //  用于锁住本结构体
    pthread_mutex_t thread_counter; //  记录忙线程数

    pthread_cond_t queue_not_full;  //  当任务队列满时，添加任务的线程阻塞等待此条件
    pthread_cond_t queue_not_empty; //  当任务队列不为空时，通知等待任务的线程

    pthread_t* threads; //  存放线程池中每个线程的tid
    pthread_t adjust_tid;   //  存放管理线程tid
    threadpool_task_t* task_queue;  //  任务队列

    int min_thr_num;    //  线程池最小线程数
    int max_thr_num;    //  线程池最大线程数
    int live_thr_num;   //  当前存活线程数
    int busy_thr_num;   //  忙线程数
    int wait_exit_thr_num;  //  待销毁线程数

    int queue_from; //  队列头
    int queue_rear; //  队列尾
    int queue_size; //  队列中实际任务数
    int queue_max_size; //  队列可容纳上限

    int shutdown;   //  标志位：线程池使用状态
};

typedef struct  //  各个子线程的任务结构体
{
    void* (*func)(void*);   //  回调函数
    void* arg;  //  回调函数参数
}threadpool_task_t;



//  向线程池中添加一个任务
int threadpool_add(struct threadpool_t* pool, void* (*func)(void*), void* arg)
{
    pthread_mutex_lock(&(pool->lock));

    while((pool->queue_size == pool->queue_max_size) && (!pool->shutdown))  
    {
        pthread_cond_wait(&(pool->queue_not_full), &(pool->lock));
    }

    if(pool->shutdown)
    {
        pthread_cond_broadcast(&(pool->queue_not_empty));
        pthread_mutex_unlock(&(pool->lock));
        return 0;
    }

    //  清空工作线程调用的回调函数的参数
    if(pool->task_queue[pool->queue_rear].arg != NULL)
        pool->task_queue[pool->queue_rear].arg = NULL;

    //  添加任务到任务队列里
    pool->task_queue[pool->queue_rear].func = func;
    pool->task_queue[pool->queue_rear].arg = arg;
    pool->queue_rear = (pool->queue_rear + 1) % pool->queue_max_size;
    pool->queue_size++;

    //  添加完任务后，队列不为空，唤醒线程池中等待任务的线程
    pthread_cond_signal(&(pool->queue_not_empty));
    pthread_mutex_unlock(&(pool->lock));

    return 0;
}

//  线程池中各个工作线程
void* threadpool_thread(void* threadpool)
{
    struct threadpool_t* pool = (struct threadpool*)threadpool;
    threadpool_task_t task;

    while(1)
    {
        pthread_mutex_lock(&(pool->lock));

        while((pool->queue_size == 0) && (!pool->shutdown))
        {
            printf("thread 0x%x is waiting\n", (unsigned int)pthread_self());
            pthread_cond_wait(&(pool->queue_not_empty), &(pool->lock));

            //  清除指定数目的空闲线程
            if(pool->wait_exit_thr_num > 0)
            {
                pool->wait_exit_thr_num--;

                if(pool->live_thr_num > pool->min_thr_num)
                {
                    printf("thread 0x%x is exiting\n", (unsigned int)pthread_self());
                    pool->live_thr_num--;
                    pthread_mutex_unlock(&(pool->lock));

                    pthread_exit(NULL);
                }
            }
        }

        //  如果指定了true，要关闭线程池里的每个线程，自行退出处理--销毁线程池
        if(pool->shutdown)
        {
            pthread_mutex_unlock(&(pool->lock));
            printf("thread 0x%x is exiting\n", (unsigned int)pthread_self());
            pthread_detach(pthread_self());
            pthread_exit(NULL);
        }

        //  从任务队列中获取任务，出队操作
        task.func = pool->task_queue[pool->queue_from].func;
        task.arg = pool->task_queue[pool->queue_from].arg;

        pool->queue_from = (pool->queue_from + 1) % pool->queue_max_size;
        pool->queue_size--;

        //  通知允许有新的任务添加进来
        pthread_cond_broadcast(&(pool->queue_not_full));

        //  任务取出后，立即释放线程池锁
        pthread_mutex_unlock(&(pool->lock));

        //  执行任务
        printf("thread 0x%x start working\n", (unsigned int)pthread_self());
        pthread_mutex_lock(&(pool->thread_counter));
        pool->busy_thr_num++;
        pthread_mutex_unlock(&(pool->thread_counter));

        (*(task.func))(task.arg);

        //  任务结束处理
        printf("thread 0x%x end working\n", (unsetenv int)pthread_self());
        pthread_mutex_lock(&(pool->thread_counter));
        pool->busy_thr_num--;
        pthread_mutex_unlock(&(pool->thread_counter));
    }   

    return NULL;
}

//  管理线程
void* adjust_thread(void* threadpool)
{
    int i;
    struct threadpool_t* pool = (struct threadpool_t*)threadpool;

    while(!pool->shutdown)
    {
        sleep(10);  //  不需要一直管理，定时

        pthread_mutex_lock(&(pool->lock));
        int queue_size = pool->queue_size;
        int live_thr_num = pool->live_thr_num;
        pthread_mutex_unlock(&(pool->lock));

        pthread_mutex_lock(&(pool->thread_counter));
        int busy_thr_num = pool->busy_thr_num;
        pthread_mutex_unlock(&(pool->thread_counter));

        //  创建新线程：任务数大于等于最小线程池个数，且存活的线程数小于最大线程个数
        if(queue_size >= pool->min_thr_num && live_thr_num < pool->max_thr_num)
        {
            pthread_mutex_lock(&(pool->lock));
            int add = 0;

            for(i = 0; i < pool->max_thr_num && add < 10 && pool->live_thr_num < pool->max_thr_num; i++)
            {
                if(pool->threads[i] == 0 || !is_thread_alive(pool->threads[i]))
                {
                    pthread_create(&(pool->threads[i]), NULL, threadpool_thread, (void*)pool);
                    add++;
                    pool->live_thr_num++;
                }
            }
            pthread_mutex_unlock(&(pool->lock));
        }

        if((busy_thr_num * 2) < live_thr_num && live_thr_num > pool->min_thr_num)
        {
            //  一次销毁10个线程，随机10个即可
            pthread_mutex_lock(&(pool->lock));
            pool->wait_exit_thr_num = 10;
            pthread_mutex_unlock(&(pool->lock));

            for(i = 0; i < 10; i++)
            {
                //  通知处在空闲状态的线程，它们会自行终止
                pthread_cond_signal(&(pool->queue_not_empty));
            }
        }
    }

    return NULL;
}

int threadpool_free(struct threadpool_t* pool)
{
    if(pool == NULL)
        return -1;

    if(pool->task_queue)
        free(pool->task_queue);

    if(pool->threads)
    {
        free(pool->threads);
        pthread_mutex_lock(&(pool->lock));
        pthread_mutex_destroy(&(pool->lock));
        pthread_mutexlock(&(pool->thread_counter));
        pthread_mutex_destroy()
    }
}

int threadpool_destroy(struct threadpool_t* pool)
{
    int i;
    if(pool == NULL)
        return -1;

    pool->shutdown = TRUE;

    //  先销毁管理线程
    pthread_join(pool->adjust_tid, NULL);

    for(i = 0; i < pool->live_thr_num; i++)
    {
        pthread_cond_broadcast(&(pool->queue_not_empty));
    }
    for(i = 0; i < pool->live_thr_num; i++)
    {
        pthread_join(pool->threads[i], NULL);
    }

    threadpool_free(pool);

    return 0;
}

struct threadpool_t* threadpool_create(int min_thr_num, int max_thr_num, int queue_max_size)
{
    int i;
    struct threadpool_t* pool = NULL;

    do
    {
        if((pool = (struct threadpool_t*)malloc(sizeof(struct threadpool_t))) == NULL)
        {
            perror("fail to malloc threadpool");
            break;
        }

        //  初始化
        pool->min_thr_num = min_thr_num;
        pool->max_thr_num = max_thr_num;
        pool->busy_thr_num = 0;
        pool->live_thr_num = min_thr_num;
        pool->wait_exit_thr_num = 0;
        pool->queue_from = 0;
        pool->queue_rear = 0;
        pool->queue_max_size = queue_max_size;
        pool->queue_size = 0;
        pool->shutdown = FALSE;

        //  根据最大线程上限数  分配空间
        pool->threads = (pthread_t*)calloc(max_thr_num, sizeof(pthread_t));
        if(pool->threads == NULL)
        {
            perror("fail to calloc threads");
            break;
        }        

        //  给任务队列分配空间
        pool->task_queue = (threadpool_task_t*)calloc(queue_max_size, sizeof(threadpool_task_t));
        if(pool->task_queue == NULL)
        {
            perror("fail to calloc task_queue");
            break;
        }

        //  初始化互斥锁、条件变量
        if(pthread_mutex_init(&(pool->lock), NULL) != 0 ||
            pthread_mutex_init(&(pool->thread_counter), NULL) != 0 ||
            pthread_cond_init(&(pool->queue_not_empty), NULL) != 0 ||
            pthread_cond_init(&(pool->queue_not_full), NULL) != 0)
        {
            perror("fail to lock or cond");
            break;
        }

        //  创建min_thr_num个work thread
        for(i = 0; i < min_thr_num; i++)
        {
            pthread_create(pool->threads + i, NULL, threadpool_thread, (void*)pool);
            printf("start thread 0x%x ...\n", (unsigned int)pool->threads[i]);
        }
        //  创建管理线程
        pthread_create(&(pool->adjust_tid), NULL, adjust_thread, (void*)pool);

        return pool;
    } while (0);

    threadpool_free(pool);
    
    return NULL;
}

/* 线程池中线程， 模拟处理业务 */
void* process(void* arg)
{
    printf("thread 0x%x working on task %d ...\n", (unsigned int)pthread_self(), (int)arg);
    sleep(1);
    printf("task %d is end\n", (int)arg);

    return NULL;
}

int main()
{
    //  创建线程池；最少3个线程，最大100个，队列容量上限100
    struct threadpool_t* thp = threadpool_create(3, 100, 100);

    int num[20], i;
    for(i = 0; i < 20; i++)
    {
        num[i] = i;
        printf("add task %d ...\n", i);

        //  向线程池中添加任务
        threadpool_add(thp, process, (void*)&num[i]);
    }

    sleep(10);
    threadpool_destroy(thp);

    return 0;
}