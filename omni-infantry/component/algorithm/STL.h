//
// Created by 86198 on 2024/1/20.
//

#ifndef OMNI_INFANTRY_STL_H
#define OMNI_INFANTRY_STL_H
//队列实现
typedef float QDateType;   //目前声明队列保存浮点数

typedef struct QueueNode
{
    QDateType val;
    struct QueueNode* next;
}QueueNode;

typedef	struct Queue
{
    QueueNode* head;
    QueueNode* tail;
}Queue;
void QueueInti(Queue* pq);
// 队列初始化
void QueueDestory(Queue* pq);
// 队列的销毁
void QueuePush(Queue* pq, QDateType x);
// 入队
void QueuePop(Queue* pq);
// 出队
QDateType QueueFront(Queue* pq);
// 取出队首元素
int QueueSize(Queue* pq,QDateType *avg);
// 求队列的长度并求均值
bool QueueEmpty(Queue* pq);
// 判断队是否为空

#endif //OMNI_INFANTRY_STL_H
