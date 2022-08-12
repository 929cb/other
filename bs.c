// #include <stdio.h>

// // void swap(int cups[], int cup1, int cup2)
// // {
// //     cups[cup1] += cups[cup2];
// //     cups[cup2] = cups[cup1] - cups[cup2];
// //     cups[cup1] = cups[cup1] - cups[cup2];
// // }

// // void* my_memset(void* s, int c, int n)
// // {
// //     void* ret = s;

// //     while(*(char*)s = c && --n);

// //     return ret;
// // }

// // //  给N个杯子倒扣在桌面上，在第X个杯子中有一个球，移动杯子里面的球会跟着移动，经过K次移动，求最后那个球在第几个杯子中？
// // int main()
// // {
// //     int N, X, K;    //  N个杯子，第X个杯子中有球，移动K次
    
// //     scanf("%d %d", &N, &X);
// //     scanf("%d", &K);

// //     int cups[N + 1];
// //     my_memset(cups, 0, N + 1);
// //     cups[X] = 1;

// //     int k = K;
// //     while(k--)
// //     {
// //         int cup1, cup2;
// //         scanf("%d %d", &cup1, &cup2);
// //         swap(cups, cup1, cup2);
// //     }

// //     for(int i = 1; i <= K; i++)
// //     {
// //         if(cups[i] == 1) printf("%d\n", i);
// //     }

// //     return 0;
// // }



// int n, k;

// void my_sort(int jz[], int len)
// {
//     int i, j, min;

//     for(i = 0; i < len - 1; i++)
//     {
//         min = i;
//         for(j = i + 1; j < len; j++)
//         {
//             if(min > jz[j])
//             {
//                 min = j;
//             }
//         }
//         if(i != min)
//         {
//             jz[i] += jz[min];
//             jz[min] = jz[i] - jz[min];
//             jz[i] -= jz[min];
//         }
//     }
// }

// void maxzy(int jz[], int len)
// {
//     my_sort(jz, len);

//     int max = 0, index, nn = n;

//     for(int i = 1; i < len; i++)
//     {
//         if(jz[i] - jz[i - 1] > max)
//         {
//             max = jz[i] - jz[i - 1];
//             index = i;
//         }
//     }

//     jz[nn++] = (jz[index] - jz[index - 1]) / 2 + 1;

// }

// //  有n个基站，我们还可以多设置k个基站，问我们怎么设置基站能够使噪音最小，返回最小噪音。
// int main()
// {
//     //int n, k;

//     scanf("%d %d", &n, &k);

//     int jz[n + k];
//     for(int i = 0; i < n + k; i++) jz[i] = 0;

//     for(int i = 0; i < n; i++)
//     {
//         scanf("%d", jz + i);
//     }

//     int max = 0;

//     for(int i = 0; i < k; i++)
//     {
//         maxzy(jz, n + i);
//     }

//     my_sort(jz, n + k);

//     for(int i = 1; i < n + k; i++)
//     {
//         if(jz[i] - jz[i - 1] > max)
//         {
//             max = jz[i] - jz[i - 1];
//         }
//     }

//     printf("%d\n", max);

//     return 0;
// }
































// #include <stdio.h>

// int main()
// {
//     double d = 3.5;
//     int i = (int)d;

//     if(d - i >= 0.5) i++;

//     printf("%d\n", i);

//     return 0;
// }














#include <stdio.h>
#include <math.h>

#define PI (3.1415927)


int N;

int HanWin(short int nn, short int n)
{
    
    double param = 2 * PI * n / N;
    double han = 0.5 * (1 - cos(param)) * nn;

    short int num = (short int)han;


    if(han - num >= 0.5) num++;
    else if(han - num <= -0.5) num--;
    

    return num;
}

int main()
{    
    scanf("%d", &N);
    
    short int arr[N];
    for(int i = 0; i < N; i++)
    {
        scanf("%hd", (arr + i));
        printf("%d ", HanWin(arr[i], i));
    }
    
    return 0;
}








/* #include <stdio.h> */

/* int remove2(int* nums, int len, int* ans) */
/* { */
/*     int fast, clow = 0; */
    
/*     for(fast = len - 1; fast >= 0; fast--) */
/*     { */
/*         ans[clow++] = nums[fast]; */
        
/*         while(nums[fast - 1] == nums[fast]) */
/*             fast--; */
/*     } */
    
/*     return clow; */
/* } */

/* unsigned long long remove3(unsigned long long num) */
/* { */
/*     unsigned long long ans = 0; */
/*     unsigned long long temp = 0; */
/*     int i = 1; */

/*     while(temp = num / i % 10) */
/*     { */
/*         ans += temp; */
        

/*         while(num / (10*i) % 10 && num / (10*i) % 10 == num / i % 10) */
/*             i *= 10; */
        
/*         if(num / (10*i) % 10 == 0) */
/*             break; */

/*         ans *= 10; */
/*     } */

/*     return ans; */
/* } */

/* int main()  //  33456799 */
/* { */
/*     unsigned long long num, ans; */

/*     scanf("%lld", &num); */
    
/*     ans = remove3(num); */
    
    
/*     printf("%lld", ans); */
    
/*     return 0; */
/* } */







#include <stdio.h>
// #include <stddef.h>

// struct s1
// {
// 	char c1;
// 	int i;
// 	char c2;
// };

// #pragma pack(1)	//	设置默认对齐字节数 1
// struct
// {
// 	double d;
// 	//char c;
// 	struct s1 s;
// 	char i;
// }st;
// #pragma pack()	//	取消设置

// struct
// {
// 	char a:3;
// 	char b:4;
// 	char c:5;
// 	char d:4;
// }wd;

// int main()
// {
// 	printf("%ld\n", sizeof(st));

// 	printf("%lu\n", offsetof(struct s1, c1));
// 	printf("%lu\n", offsetof(struct s1, i));
// 	printf("%lu\n", offsetof(struct s1, c2));

// 	printf("%ld\n", sizeof(wd));

// 	return 0;
// }















#include <stdio.h>

// void finddog(int a[], int sz, int *num)
// {
//     int i = 0;
//     int pos = 0;
//     int ret = 0;
//     //遍历数组，结果为两个不同数的异或。
//     for (i = 0; i < sz; i++)
//     {
//         ret ^= a[i];
//     }
//     /*寻找这两个不同数异或结果的一个位为 1 的位 【 关键 】*/
//     for (pos = 0; pos < 32; pos++)
//     {
//         if (((ret >> pos) & 1) == 1) //整型 32 位，从低位向高位依次遍历
//         {
//             break; // pos记录二进制位为 1 的数
//         }
//     }

//     for (i = 0; i < sz; i++)
//     {
//         //找到数组中pos位为1的数，并进行异或
//         if (((a[i] >> pos) & 1) == 1)
//         {
//             num[0] ^= a[i];
//         }
//         //找到数组中pos位为0的数，并进行异或
//         else
//         {
//             num[1] ^= a[i];
//         }
//     }
// }
// int main()
// {
//     int arr[] = {1, 1, 2, 4};
//     int num[2] = {0};
//     int sz = sizeof(arr) / sizeof(arr[0]);
//     finddog(arr, sz, num);
//     printf("%d %d\n", num[0], num[1]);
//     return 0;
// }











#include <stdio.h>
// #include <unistd.h>

// int main()
// {
// 	unsigned int sec;

// 	sec = alarm(5);
// 	printf("%u\n", sec);

// 	sleep(3);

// 	sec = alarm(6);
// 	printf("%d\n", sec);

// 	while(1)
// 	{
// 		printf("hai\n");

// 		sleep(1);
// 	}

// 	return 0;
// }