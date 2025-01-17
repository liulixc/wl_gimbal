//
// Created by 86198 on 2024/1/20.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "STL.h"

void QueueInti(Queue* pq)
{
    assert(pq);
    pq->head = pq->tail = NULL;
}

void QueueDestory(Queue* pq)
{
    assert(pq);
    QueueNode* cur = pq->head;
    while (cur)
    {
        QueueNode* next = cur->next;
        free(cur);
        cur = next;
    }
    pq->tail = pq->head = NULL;
}

void QueuePush(Queue* pq, QDateType x)
{
    assert(pq);

    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (NULL == newNode)
    {
        printf("malloc error\n");
        exit(-1);
    }
    newNode->val = x;
    newNode->next = NULL;

    if (pq->tail == NULL)
    {
        assert(pq->head == NULL);
        pq->head = pq->tail = newNode;
    }
    else
    {
        pq->tail->next = newNode;
        pq->tail = newNode;
    }

}

void QueuePop(Queue* pq)
{
    assert(pq);
    assert(pq->head && pq->tail);
    if (pq->head->next == NULL)
    {
        free(pq->head);
        pq->head = pq->tail = NULL;
    }
    else
    {
        QueueNode* next = pq->head->next;
        free(pq->head);
        pq->head = next;
    }
}

bool QueueEmpty(Queue* pq)
{
    assert(pq);

    return pq->head == NULL;
}

QDateType QueueFront(Queue* pq)
{
    assert(pq);
    assert(pq->head);

    return pq->head->val;
}

int QueueSize(Queue* pq,QDateType *avg)
{
    assert(pq);
    QDateType sum=0;
    QueueNode* cur = pq->head;
    int count = 0;
    while (cur)
    {
        sum+=cur->val;
        cur = cur->next;
        count++;
    }
    (*avg)=sum/count;
    return count;
}


