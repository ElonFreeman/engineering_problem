#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int input_a_num, rand_num;            // 定义输入的数和生成的随机数
    int count, remaining, guess_max = 10; // 定义计数器，剩余次数和最大猜测次数

    printf("you have %d times chances totally.\n", guess_max);
    srand(time(NULL));                          // 初始化种子，否则会一直生成一样的随机数
    rand_num = rand() % 100 + 1;                // 生成1~100的随机数
    //printf("correct answer is:%d\n", rand_num); // 测试语句

    for (count = 0; count <= guess_max; count++) // main recycle
    {
        printf("Input a num in 1~100:");
        scanf("%d", &input_a_num);

        if (input_a_num <= 1 && input_a_num >= 100) // 输入合法性验证
        {
            printf("Illegal input!\n");
            continue;
        }

        if (input_a_num > rand_num) // 比较大小，主功能块
        {
            printf("bigger!\n");
        }
        else if (input_a_num < rand_num)
        {
            printf("smaller!\n");
        }
        else
        {
            printf("correct!\n");
            printf("you used %d times.\n", count);

            if ( count >= 3) // 根据次数做出评价
            {
                printf("you such a foolish!\n");
            }
            else
            {
                printf("goodjob!\n");
            }
            break;
        }

        remaining = guess_max - count; // 计算剩余次数
        printf("you have %d times chances.\n", remaining);
    }

    if (count >= guess_max && input_a_num != rand_num) // 超限处理
    {
        printf("Too many times!\n");
        printf("correct answer is:%d\n", rand_num);
    }

    return 0;
}