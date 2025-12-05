#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main()
{
int num, result;
srand(time(NULL));
result = rand() % 101;
printf("%d",result);
printf("请输入一个整数");

do
{
scanf_s("%d", &num);
if (num > result)
{
printf("输入的数大了\n");
printf(" \\^o^/ \n");
printf(" / \\ \n");
printf("/ O O \\ \n");
printf("| _ | \n");
printf(" \\ --- / \n");
printf(" \\___/ \n");
printf(" / \\ \n");
printf(" / \\ \n");
printf("/ \\ \n");
}
else if (num < result)
{
printf("输入的数小了\n");
printf(" \\^o^/ \n");
printf(" / \\ \n");
printf("/ O O \\ \n");
printf("| _ | \n");
printf(" \\ --- / \n");
printf(" \\___/ \n");
printf(" / \\ \n");
printf(" / \\ \n");
printf("/ \\ \n");
}
else
{
printf("恭喜你，答对了\n");
double x, y, a;
for (y = 1.5; y > -1.5; y -= 0.1)
{
for (x = -1.5; x < 1.5; x += 0.05)
{
a = x * x + y * y - 1;
putchar(a * a * a - x * x * y * y * y <= 0.0 ? '*' : ' ');
}
system("color 0c");
putchar('\n');
}
break;
}
} while (num != result);

return 0;
}
