#include <66func.h>
typedef struct node_s {
    int num;
    struct node_s * pNext;
} node_t;
typedef struct list_s {
    node_t * pHead;
    node_t * pTail;
    int size;
} list_t;
void tailInsert(list_t *plist, int num){
    node_t * pNew = (node_t *)calloc(1,sizeof(node_t));
    pNew->num = num;

    if(plist->size == 0){
        plist->pHead = pNew;
        plist->pTail = pNew;
    }
    else{
        plist->pTail->pNext = pNew;
        plist->pTail = pNew;
    }
    ++plist->size;
}
void headDelete(list_t *plist){
    node_t * pCur = plist->pHead; //待删除的节点
    plist->pHead = pCur->pNext; // 修改pHead的指向
    if(plist->size == 1){
        plist->pTail = NULL;
    }
    free(pCur);
    --plist->size;
}
void printList(list_t *plist){
    node_t * pCur = plist->pHead;
    while(pCur){
        printf("%d ", pCur->num);
        pCur = pCur->pNext;
    }
    printf("\n");
}
void *threadFunc(void *arg){
    list_t * plist = (list_t *)arg;
    for(int i = 0; i < 5; ++i){
        tailInsert(plist,i);
        printList(plist);
    }
    return NULL;
}
int main(int argc, char *argv[])
{
    list_t list;
    memset(&list,0,sizeof(list_t));//初始化得到一个空链表
    pthread_t tid;
    pthread_create(&tid,NULL,threadFunc,&list);
    pthread_join(tid,NULL);
    for(int i = 0; i < 5; ++i){
        if(list.size > 0){
            headDelete(&list);
            printList(&list);
        }
    }
    return 0;
}

