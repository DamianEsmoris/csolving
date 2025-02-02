#include <stdio.h>
#include <time.h>

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef enum {
    GRATER,
    EQ,
    LOWER
} CmpResult;

CmpResult cmp_dates(Date d1, Date d2)
{
    if (d1.year != d2.year)
        return d1.year > d2.year ? GRATER : LOWER;
    if (d1.month > d2.month)
        return GRATER;
    if (d2.month > d1.month)
        return LOWER;
    if (d1.day == d2.day)
        return EQ;
    return d1.day > d2.year ? GRATER : LOWER; 
}

int is_leap_year(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

int days_on_month(int month, int year)
{
    switch (month) {
        case 2:
            return is_leap_year(year) ? 29 : 28;
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
    }

}

int date_diff_on_days(Date d1, Date d2)
{
    if (cmp_dates(d1, d2) == EQ)
        return 0;
    if (cmp_dates(d1, d2) == GRATER)
        return cmp_dates(d2, d1) * -1;
    int year_diff = d2.year - d1.year;
    int diff = d2.day - d1.day;
    for (int y = 0; y < year_diff; y++) {
        for (int m = 1; m <= 12; m++)
            diff += days_on_month(m, d1.year);
        d1.year++;
    }
    for (int m = d1.month; m < d2.month; m++)
        diff += days_on_month(m, d1.year);
    return diff;
}

void print_date(Date d)
{
    printf("%d-%02d-%02d\n", d.day, d.month, d.year);
}

int main()
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    Date now = {
        .year = tm.tm_year,
        .month = tm.tm_mon + 1,
        .day = tm.tm_mday
    };
    Date chrismas = {
        .year = now.year,
        .month = 12,
        .day = 25
    }; 
    int days_to_chrismas = date_diff_on_days(now, chrismas);
    if (days_to_chrismas >= 0)
        printf("%d days left before chrismas!\n", days_to_chrismas);
    else
        printf("chrismas was %d days ago\n", days_to_chrismas*-1);

}
