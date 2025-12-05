#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1000
#define MAX_ROWS 100
#define MAX_TEXT_LEN 100
#define MAX_NUM_LEN 20


int main()
{
    FILE *dataset=fopen("employ-data.csv","r+");  //打开数据集文件
    if(dataset==NULL)
    {
        printf("Can not open the dataset file.");
        return 1;
    }

    char main_data_set[][2]={},line[MAX_LINE],list1[]={};
    int count=0,state=0,i,j,k,tmp,list2[]={};  //count为数据集的行数，由读取时的计数器得到

    while(fgets()!=NULL)  //读取数据集文件
    {
        if(line[i]=' ')  //读取到空格，什么都不做
        {

        }

        else if()  //读取到中文文本，将其写入main_data_set的第一列
        {
            main_data_set[count][0]=line[i];
        }

        else if()  //读取到数字，将其写入main_data_set的第二列
        {
            main_data_set[count][1]=line[i];
        }

        count++;
    }

    for(i=0;i<count;i++)
    {
        list1[i]=main_data_set[i][0];
        list2[i]=(int)main_data_set[i][1];
    }

    for(i=0;i<count-1;i++)
    {
        k=i;

        for(j=i+1;j<count;j++)
        {
            if(list2[j]<list2[k])
            {
                k=j;
            }
        }

        if(k!=i)
        {
            tmp=list2[k];
            list2[k]=list2[i];
            list2[i]=tmp;

            tmp=list1[k];
            list1[k]=list1[i];
            list1[i]=tmp;
        }
    }

    for(i=0;i<count;i++)
    {
        printf("%c %d",list1[i],list2[i]);
    }


    return 0;
}