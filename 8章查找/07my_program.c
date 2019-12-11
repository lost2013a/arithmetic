#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUCKETCOUNT 16



struct hashEntry
{
    unsigned short key;
    char* value;
    struct hashEntry* next;
};

typedef struct hashEntry entry;

struct hashTable
{
    entry bucket[BUCKETCOUNT];  //先默认定义16个桶
};

typedef struct hashTable table;

//在堆上分配足以保存str的内存
//并拷贝str内容到新分配位置
char* strDup(const char* str)
{
    int len;
    char* ret;
    if (str == NULL)return NULL;

    len = strlen(str);
    ret = (char*)malloc(len + 1);
    if (ret != NULL) {
        memcpy(ret , str , len);
        ret[len] = '\0';
    }
    return ret;
}

//哈希散列方法函数
int keyToIndex(unsigned short key)
{
    return key%BUCKETCOUNT;
}


//初始化哈希表
void initHashTable(table* t)
{
    int i;
    if (t == NULL)return;

    for (i = 0; i < BUCKETCOUNT; ++i) {
        t->bucket[i].key = 0;
        t->bucket[i].value = NULL;
        t->bucket[i].next = NULL;
    }
}

//释放哈希表
void freeHashTable(table* t)
{
    int i;
    entry* e,*ep;
    if (t == NULL)return;
    for (i = 0; i<BUCKETCOUNT; ++i) {
        e = &(t->bucket[i]);
        while (e->next != NULL) {
            ep = e->next;
            e->next = ep->next;
            free(ep->value);
            free(ep);
        }
    }
}


//向哈希表中插入数据
int insertEntry(table* t , unsigned short key , const char* value)
{
    int index , vlen1 , vlen2;
    entry* e , *ep;

    if (t == NULL || key == 0 || value == NULL) {
        return -1;
    }

    index = keyToIndex(key);
    if (t->bucket[index].key == 0) {
        t->bucket[index].key = key;
        t->bucket[index].value = strDup(value);
    }
    else {
        e = ep = &(t->bucket[index]);
        while (e != NULL) { //先从已有的找
            if (strcmp(e->value , value) == 0) {
                //找到key所在，替换值
                vlen1 = strlen(value);
                vlen2 = strlen(e->value);
                if (vlen1 > vlen2) {
                    free(e->value);
                    e->value = (char*)malloc(vlen1 + 1);
                }
                memcpy(e->value , value , vlen1 + 1);
                return index;   //插入完成了
            }
            ep = e;
            e = e->next;
        } // end while(e...

        //没有在当前桶中找到
        //创建条目加入
        e = (entry*)malloc(sizeof (entry));
        e->key = key;
        e->value = strDup(value);
        e->next = NULL;
        ep->next = e;
    }
    return index;
}

//在哈希表中查找key对应的value
//找到了返回value的地址，没找到返回NULL
const char* findValueByKey(const table* t , unsigned short key)
{
    int index;
    const entry* e;
    if (t == NULL || key == 0) {
        return NULL;
    }
    index = keyToIndex(key);
    e = &(t->bucket[index]);
    if (e->key == 0) return NULL;//这个桶还没有元素
    while (e != NULL) {
        if (key == e->key) {
            return e->value;    //找到了，返回值
        }
        e = e->next;
    }
    return NULL;
}


//在哈希表中查找key对应的entry
//找到了返回entry，并将其从哈希表中移除
//没找到返回NULL
entry* removeEntry(table* t , unsigned short key)
{
    int index;
    entry* e,*ep;   //查找的时候，把ep作为返回值
    if (t == NULL || key == 0) {
        return NULL;
    }
    index = keyToIndex(key);
    e = &(t->bucket[index]);
    while (e != NULL) {
        if (key == e->key) {	/*找到元素*/
            if (e == &(t->bucket[index])) { //如果是桶的第一个
                ep = e->next;
                if (ep != NULL) {
					//如果这个桶有两个或以上元素 
               		//交换第一个和第二个，然后移除第二个
                    entry tmp = *e; //做浅拷贝交换
                    *e = *ep;//相当于链表的头节点已经移除
                    *ep = tmp;  //这就是移除下来的链表头节点
                    ep->next = NULL;
                }
                else {//这个桶只有第一个元素
                    ep = (entry*)malloc(sizeof(entry));
                    *ep = *e;
                    e->key = e->value = 0;
                    e->next = NULL;
                }
            }
            else {
                //如果不是桶的第一个元素
                //找到它的前一个(这是前面设计不佳导致的多余操作)
                ep = &(t->bucket[index]);
                while (ep->next != e)ep = ep->next;
                //将e从中拿出来
                ep->next = e->next;
                e->next = NULL;
                ep = e;
            }
            return ep;
        }// end if(strcmp...
        e = e->next;
    }
    return NULL;
}

void printTable(table* t)
{
    int i;
    entry* e;
    if (t == NULL)return;
    for (i = 0; i<BUCKETCOUNT; ++i) {
        printf("bucket[%d]:\n" , i);
        e = &(t->bucket[i]);
        while (e->key != 0) {
            printf("\t%d\t=\t%s\n" , e->key , e->value);
            if (e->next == NULL)break;
            e = e->next;
        }
    }
}

int main()
{
    table t;
    initHashTable(&t);

    insertEntry(&t , 0 , "0");
    insertEntry(&t , 6502 , "6502");
    insertEntry(&t , 6503 , "6503");
    insertEntry(&t , 6502 , "6502");
    insertEntry(&t , 6505 , "6505");
 

    entry* e = removeEntry(&t , "主板型号");
    if (e != NULL) {
        puts("找到后要释放");
		e->key=0;
        free(e->value);
        free(e);
        e = NULL;
    }

    printTable(&t);

	
    unsigned short keys[4] = { 6501 , 6502, 6509 , 0 };
    for (int i = 0; i < 4; ++i) {
        const char* value = findValueByKey(&t , keys[i]);
        if (value != NULL) {
            printf("find %d\t=\t%s\n" ,keys[i], value);
        }
        else {
            printf("not found %d\n",keys[i]);
        }
    }

	
    freeHashTable(&t);
    return 0;
}

