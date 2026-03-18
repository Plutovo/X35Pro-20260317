#ifndef _QUEUE_MANUL_H_
#define _QUEUE_MANUL_H_
//#include <stdbool.h>
#include "Static_Menu_Creat.h"
#ifndef NULL
#define NULL (0)
#endif

/*在此处插入Item的类型定义*/
/*例如：typedef int Item或者typedef struct {int i;int j;} Item;*/

#ifndef Int8U
#define Int8U unsigned char
#define Int16U unsigned short int
#endif

#ifdef SPO2_SWITCH

#ifndef TREND_PARA_NUM
#define TREND_PARA_NUM       23 //+8 //ylk add 20181121
#endif

#else

#ifndef TREND_PARA_NUM
#define TREND_PARA_NUM       21 //+8 //ylk add 20181121
#endif

#endif

typedef struct
{
    RTC_TIMER Time;
    Int16U Para[TREND_PARA_NUM]; 
}Item;
//定义队列长度
#define DEFAULT_MAXQUEUE 23 //ylk modify 20181121 15->23 修改队列长度

typedef struct node 
{
    Item item;
    struct node * prev;
    struct node * next;
}Node;

typedef struct queue
{ 
    Node front; /*队列头指针*/
    unsigned int  items;   /*队列中项目个数*/
    unsigned int  max_item;
    unsigned long addr;
}Queue;

/**************************************
*函数名：InitializeQueue
*参数：pq传入要操作的队列指针,addr队列创建的地址
*功能：将队列初始化为空队列
***************************************
*/
void InitializeQueue(Queue *pq,unsigned long addr);//Int32U

/**************************************
*函数名：InsertQueue
*参数：item插入尾部的项目，pq传入要操作的队列指针
*功能：如果队列未满，则将项目插入到队列尾，返回True；
       如果队列已满，则将头项目释放，然后插入尾项目,返回True；
       如果创建失败，返回False
*返回值：
***************************************
*/
int InsertQueue(Item item,Queue *pq);

/**************************************
*函数名：QueueIsFull
*参数：传入要操作的队列指针
*功能：检查队列是否已满
*返回值：True满：False非满
***************************************
*/
int QueueIsFull(const Queue * pq);

/**************************************
*函数名：QueueIsEmpty
*参数：传入要操作的队列指针
*功能：检查队列是否已空
*返回值：True空：False非空
***************************************
*/
int QueueIsEmpty(const Queue * pq);

/**************************************
*函数名：QueueItemCount
*参数：传入要操作的队列指针
*功能：查看队列中项目的个数
*返回值：项目个数
***************************************
*/
int QueueItemCount(const Queue * pq);

/**************************************
*函数名：EmptyQueue
*参数：pq传入要操作的队列指针
*功能：清空队列
*返回值：
***************************************
*/
void EmptyQueue(Queue *pq);

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

int InitItem(Item *item);

/**************************************
*函数名：DeleteQueue
*参数：pitem被删除的头项目，pq传入要操作的队列指针
*功能：如果队列非空，队列首项目被复制到*pitem，并被
       从队列中删除，返回True；如果这个操作使队列为空，
       把队列重置为空队列。
       如果这个队列一开始就为空，则返回False
*返回值：
***************************************
*/
int DeleteQueue(Item *pitem,Queue *pq);


#endif  //_QUEUE_H_
