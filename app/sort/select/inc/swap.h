/* 交换方法一：借助第三个变量 
 * */
static inline void swap(int *a, int *b)
{
    int tmp = 0;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

/*
 * 交换方法二：整数相加法
 * */
static inline void swap2(int *a, int *b)
{
    *a = *a + *b;
    *b = *a - *b;
    *a = *a - *b;
}

/*
 * 交接方法三：位运算--异或
 * */
static inline void swap3(int *a, int *b)
{
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}
