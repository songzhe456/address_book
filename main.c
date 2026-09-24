#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024

struct Person {
    char name[40];
    int phone_number;
    struct Person* next;
};

struct Person* pool = NULL;
int count;

void inputInfo(struct Person* person) {
    printf("请输入要插入的联系人的姓名：");
    scanf("%s",person -> name);
    printf("请输入要插入的联系人的号码：");
    scanf("%d",&person -> phone_number);
}

void addPerson(struct Person **head) {
    struct Person* person;
    static struct Person* tail;
    //如果内存池池不为空则从中获取空间
    if (pool != NULL) {
        person = pool;
        pool = pool -> next;
        count --;
    }
    //如果内存池为空，则调用malloc申请新的空间
    else {
        person = (struct Person*)malloc(sizeof(struct Person));
        if (person == NULL) {
            printf("内存分配失败！");
            exit(1);
        }
    }
    inputInfo(person);
    if (*head != NULL) {
        tail -> next = person;
        person -> next = NULL;
    }
    else {
        *head = person;
        person -> next = NULL;
    }
    tail = person;
}

struct Person* findPerson(struct Person* head,char target[]) {
    struct Person* person = head;
    while (person != NULL) {
        if (!strcmp(person -> name,target)) {
            break;
        }
        person = person -> next;
    }
    return person;
}

void changePerson(struct Person *head) {
    char target[40];
    int mode;
    struct Person* person;
    printf("你要修改谁的数据：");
    scanf("%s",target);
    person = findPerson(head,target);
    if (person == NULL) {
        printf("该联系人不存在\n");
        return;
    }
    printf("你要修改什么数据（1:姓名/2:号码）：");
    scanf("%d",&mode);
    switch (mode) {
        case 1: {
            printf("要修改成什么：");
            scanf("%s",person -> name);
            break;
        }
        case 2: {
            printf("要修改成什么：");
            scanf("%d",&person -> phone_number);
            break;
        }
        default:printf("无效输入");
    }
}

void delPerson(struct Person** head,char name[]) {
    struct Person* previous;
    struct Person* current;
    struct Person* temp;

    current = *head;
    previous = NULL;
    while (current != NULL && strcmp(current -> name,name)) {
        previous = current;
        current = current -> next;
    }
    if (current == NULL) {
        printf("找不到匹配的节点！\n");
        return;
    }
    else {
        if (previous == NULL) {
            *head = current -> next;
        }
        else {
            previous -> next = current -> next;
        }

        //判断内存池是否有空位
        if (count < MAX) {
            if (pool != NULL) {
                temp = pool;
                pool = current;
                current -> next = temp;
            }
            else {
                pool = current;
                current -> next = NULL;
            }
        }
        else {
            free(current);
        }
    }
}

void displayContacts(struct Person** head) {
    struct Person* person = *head;
    while (1) {
        if (person != NULL) {
            printf("姓名：%s\n",person -> name);
            printf("号码：%d\n",person -> phone_number);
            person = person -> next;
        }
        else {
            break;
        }
    }
}

void release(struct Person** head) {
    struct Person* person = *head;
    while (1) {
        if (person != NULL) {
            struct Person* next = person -> next;
            free(person);
            person = next;
        }
        else {
            break;
        }
    }
}

void releasePool(void) {
    struct Person* temp;

    while (pool != NULL) {
        temp = pool;
        pool = pool -> next;
        free(temp);
    }
}
/*
 *全部选项可选
 */
int main(void) {
    struct Person* head = NULL;
    int func_num;
    char target_name[40];
    struct Person* person;
    char delete_name[40];

    while (1) {
        printf("请输入操作数（1：插入 2：查找 3：更改 4：删除 5：显示 6：退出）：");
        scanf("%d",&func_num);
        switch (func_num) {
            case 1: {
                addPerson(&head);
                break;
            }
            case 2: {
                printf("请输入要查找的联系人的姓名：");
                scanf("%s",target_name);
                person = findPerson(head,target_name);
                if (person == NULL) {
                    printf("无法检索到目标！\n");
                }
                else {
                    do {
                        printf("找到了符合要求的目标\n");
                        printf("%s:%d\n",person -> name,person -> phone_number);
                    }while ((person = findPerson(person -> next,target_name)) != NULL);
                }
                break;
            }
            case 3: {
                changePerson(head);
                break;
            }
            case 4: {
                printf("请输入你要删除的联系人的姓名：");
                scanf("%s",delete_name);
                delPerson(&head,delete_name);
                break;
            }
            case 5: {
                displayContacts(&head);
                break;
            }
            case 6: {
                printf("已退出程序\n");
                release(&head);
                releasePool();
                return 0;
            }
            default:printf("无效的操作数：%d\n",func_num);
        }
    }
    return 0;
}
