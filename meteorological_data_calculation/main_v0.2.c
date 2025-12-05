#include <stdio.h>

//1.
void dayaver(float daytemp[][4])
{
    int count,day;
    float sum,aver;
    for(day=0;day<7;day++)
    {
        sum=0;
        
        for(count=0;count<4;count++)
        {
            sum+=daytemp[day][count];
        }

        aver=sum/4.0;
        printf("%d:%f\n",day+1,aver);
    }
}

//2.
float *daydiff(float daytemp[][4],float daily_diff[])
{
    int count,day;
    float max,min,diff;

    for(day=0;day<7;day++)
    {
        max=daytemp[day][0];
        for(count=0;count<4;count++) //max
        {
            if(daytemp[day][count]>max)
            {
                max=daytemp[day][count];
            }
        }

        min=daytemp[day][0];
        for(count=0;count<4;count++) //min
        {
            if(daytemp[day][count]<min)
            {
                min=daytemp[day][count];
            }
        }

        diff=max-min;
        printf("%d:%f\n",day+1,diff);
        daily_diff[day]=diff;
    }
    
    return daily_diff;
}

//3.
void week_maxdiff(float day_diffs[])  //the daily maximum and minimum of temperature
{
    float max=day_diffs[0],min=day_diffs[0];
    int day,contamax=0,contamin=0;

    for(day=0;day<7;day++)  //max value
    {
        if(day_diffs[day]>max)
        {
            max=day_diffs[day];
            contamax=day+1;
        }
    }
    printf("max day %d:%f\n",contamax,max);

    for(day=0;day<7;day++)  //min value
    {
        if(day_diffs[day]<min)
        {
            min=day_diffs[day];
            contamin=day+1;
        }
    }
    printf("min day %d:%f\n",contamin,min);
}


//main function
int main()
{
    float temp_data[7][4];
    float daily_diffs[7];

    for(int i=0;i<7;i++)
    {
        for(int j=0;j<7;j++)
        {
            scanf("%f",&temp_data[i][j]);
        }
    }

    printf("daily average:\n");
    dayaver(temp_data);

    printf("daily different:\n");
    daydiff(temp_data,daily_diffs);

    printf("weekly differents maximum and minimum:\n");
    week_maxdiff(daily_diffs);

    return 0;
}