#include <stdio.h>
int main(void)
{
    int number=0;
    int* pnumber=NULL;

    number=10;
    printf("%p\n",&number);  //number的地址
    printf("%d\n",number);  //number的值

    pnumber=&number;
    printf("%p\n",(void*)&pnumber);  //指针的地址
    printf("%zu\n",sizeof(pnumber));  //指针的长度
    printf("%p\n",pnumber);  //指针的值，即number的地址
    printf("%d\n",*pnumber);  //指针所指向地址指向的值  .

    return 0;
}