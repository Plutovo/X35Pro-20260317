#include "queue_manul.h"


/**************************************
*函数名：QueueMaxItem
*参数：传入要操作的队列指针
*功能：设置队列的最大项目数
*返回值：True满：False非满
***************************************
*/
int QueueMaxItem( Queue * pq,int num)
{
    if(pq != NULL)
    {
        pq->max_item = num;
        return 1;  //1true，0false
    }
    else 
    {
        return 0;
    }
}


/**************************************
*函数名：QueueIsFull
*参数：传入要操作的队列指针
*功能：检查队列是否已满
*返回值：True满：False非满
***************************************
*/
int QueueIsFull(const Queue * pq)
{
    return (pq->items == pq->max_item)?1:0;  //1true，0false
}

/**************************************
*函数名：QueueIsEmpty
*参数：传入要操作的队列指针
*功能：检查队列是否已空
*返回值：True空：False非空
***************************************
*/
int QueueIsEmpty(const Queue * pq)
{
    return (pq->items == 0)?1:0;
}

/**************************************
*函数名：QueueItemCount
*参数：传入要操作的队列指针
*功能：查看队列中项目的个数
*返回值：项目个数
***************************************
*/
int QueueItemCount(const Queue * pq)
{
    return pq->items;
}

/**************************************
*函数名：InitializeQueue
*参数：pq传入要操作的队列指针,addr队列创建的地址
*功能：将队列初始化为空队列
***************************************
*/
void InitializeQueue(Queue *pq,unsigned long addr)//Int32U
{
    pq->front.next = pq->front.prev = &pq->front;
    //pq->front.item.Para = {0};
    //pq->front.item.Time = {0};
    pq->items = 0;
    pq->max_item = DEFAULT_MAXQUEUE;
    pq->addr = addr;
}

/**************************************
*函数名：DeleteQueue
*参数：pitem被删除的头项目，pq传入要操作的队列指针
*功能：如果队列非空，队列首项目结点被传到pnode中，并被
       从队列中删除，返回True；如果这个操作使队列为空，
       把队列重置为空队列。
       如果这个队列一开始就为空，则返回False
*返回值：
***************************************
*/
int DeleteQueue(Item * pitem,Queue *pq)
{
//    Item pt;
    if(QueueIsEmpty(pq))
    {
        return 0;//false
    }
    *pitem = pq->front.next->item;
    pq->front.next = pq->front.next->next;
    pq->front.next->prev = &pq->front;

    pq->items--;
    return 1;//True
}

/**************************************
*函数名：InsertQueue
*参数：item插入尾部的项目，pq传入要操作的队列指针
*功能：如果队列未满，则将项目插入到队列尾，返回True；
       如果队列已满，返回False；
       如果创建失败，返回False
*返回值：
***************************************
*/
int InsertQueue(Item item,Queue *pq)
{
    Node *pnew;
//    Item *pt;
    unsigned int max_addr;
    if(QueueIsFull(pq))
    {
        return 0;
    }
    if(QueueIsEmpty(pq))
    {
        pnew = (Node *)(pq->addr);
        pnew->item = item;
        pq->front.next = pq->front.prev = pnew;
        pnew->prev = pnew->next = &pq->front;        
    }
    else
    {
        max_addr = pq->addr + ((pq->max_item-1)*sizeof(Node));
                                
        if((unsigned long)(pq->front.next) + (pq->items*sizeof(Node)) > max_addr)//地址超出范围，则使地址循环(已经用到了地址的最大值)
        {
            pnew = (Node *)(pq->addr + ((unsigned long)(pq->front.next) + (pq->items*sizeof(Node)) - (max_addr + 1*sizeof(Node)))); //当前段的物理起始地址 +　超出最大地址的值
        }
        else
        {
            pnew = (Node *)(pq->addr + (pq->items*sizeof(Node)));
        }
        memcpy(&pnew->item,&item,sizeof(Item));
        pnew->next = &pq->front;
        pnew->prev = pq->front.prev;
        pq->front.prev = pnew->prev->next = pnew;
    }
    pq->items++;
    return 1;
}
/**************************************
*函数名：EmptyQueue
*参数：pq传入要操作的队列指针
*功能：清空队列
*返回值：
***************************************
*/
void EmptyQueue(Queue *pq)
{
    Item pt;
    while(!QueueIsEmpty(pq))
    {
        DeleteQueue(&pt,pq);
    }
    pq->items = 0;
}

/**************************************
*函数名：InitItem
*参数：item传入想要初始化的项目的地址
*功能：初始化一个项目节点
*返回值：
*备注：可以制作一个传入初始化函数的参数，
       因为Item的类型不确定，预留一个初始化
       函数的接口就能变成万能函数了
***************************************
*/

int InitItem(Item *item)
{
    int cnt;
    memset(item,0,sizeof(Item));

//    for(cnt = 0;cnt < TREND_PARA_NUM;cnt++)
//    {
//        item->Para[cnt] = 0;
//    }
    return 0;
}