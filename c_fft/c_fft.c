#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

 /* 复数结构体 */
typedef struct 
{
    double real;
    double imag;
}Complex;

/* 复数加法 */
void Complex_ADD(Complex a, Complex b, Complex *c)
{
	c->real = a.real + b.real; 
	c->imag = a.imag + b.imag;
}
/* 复数减法 */
void Complex_SUB(Complex a, Complex b, Complex *c)
{
	c->real = a.real - b.real;
	c->imag = a.imag - b.imag;
}
/* 复数乘法 */
void Complex_MUL(Complex a, Complex b, Complex *c)
{
	c->real = a.real * b.real - a.imag * b.imag;
	c->imag = a.real * b.imag + a.imag * b.real;
}
/* 复数绝对值 */
double Complex_ABS(Complex *a)
{
	double b;
	b = sqrt((a->real)*(a->real)+(a->imag)*(a->imag));
	return b;
}

static double PI=4.0*atan(1); //定义π 因为tan(π/4)=1 所以arctan（1）*4=π，增加π的精度

/**
 * @description: FFT算法（非递归）
 * @param {cs_uint32*} signal_in: 需要fft信号输入
 * @param {cs_uint32} signal_len: 输入信号长度
 * @param {Complex*} fft_out: fft完成后输出结果
 * @param {cs_uint32} fft_point:  采样点数
 * @return {*} 
 */
uint32_t FFT(uint32_t* signal_in, uint32_t signal_len, Complex* fft_out, uint32_t fft_point)
{
	Complex *W;//旋转因子Wn
	uint32_t i,j,k,m;
	Complex temp1,temp2,temp3;//用于交换中间量
	double series;//序列级数
	if(signal_len < fft_point)//采样点数与信号数据对比 
	{
		for(i=0;i < signal_len;i++)
		{
			fft_out[i].real = signal_in[i];
			fft_out[i].imag = 0;
		}
		for(i=signal_len;i<fft_point;i++)//补0
		{
			fft_out[i].real = 0;
			fft_out[i].imag = 0;
		}
	}else if(signal_len == fft_point)
	{
		for(i=0;i < signal_len;i++)
		{
			fft_out[i].real = signal_in[i];
			fft_out[i].imag = 0;
		}
	}
	else
	{
		for(i=0;i < fft_point;i++)
		{
			fft_out[i].real = signal_in[i];
			fft_out[i].imag = 0;
		}
	}
	if(fft_point%2 != 0)
	{
		return 1;
	}
	
	W = (Complex *)malloc(sizeof(Complex) * fft_point);
	if (W == NULL)
	{
		return 1;
	}
	
	for (i = 0; i < fft_point; i++)
	{
		W[i].real = cos(2*PI/fft_point*i);	 //欧拉表示的实部
		W[i].imag = -1*sin(2*PI/fft_point*i);//欧拉表示的虚部
	}
	
	for(i = 0;i < fft_point;i++)
	{
		k = i;
		j = 0;
		series = log(fft_point)/log(2); //算出序列的级数
		while((series--) > 0)//利用按位与以及循环实现码位颠倒
		{
			j = j << 1;
			j|=(k & 1);
			k = k >> 1;
		}
		if(j > i) //将x(n)的码位互换
		{
			temp1 = fft_out[i];
			fft_out[i] = fft_out[j];
			fft_out[j] = temp1;
		}
	}
	series = log(fft_point)/log(2); //蝶形运算的级数
	
	for(i = 0; i<series;i++)
	{
		m = 1<<i; //移位 每次都是2的指数的形式增加，其实也可以用m=2^i代替
		for(j = 0;j<fft_point;j+=2*m) //一组蝶形运算，每一组的蝶形因子乘数不同
		{
			for(k=0;k<m;k++)//蝶形结点的距离  一个蝶形运算 每个组内的蝶形运算
			{
				Complex_MUL(fft_out[k+j+m],W[fft_point*k/2/m],&temp1);
				Complex_ADD(fft_out[j+k],temp1,&temp2);
				Complex_SUB(fft_out[j+k],temp1,&temp3);
				fft_out[j+k] = temp2;
				fft_out[j+k+m] = temp3;
			}
		}
	}
	free(W);
	return 0;
}



static double PI=4.0*atan(1); //定义π 因为tan(π/4)=1 所以arctan（1）*4=π，增加π的精度

/**
 * @description: FFT算法（非递归）
 * @param {uint32_t*} signal_in: 需要fft信号输入
 * @param {uint32_t} signal_len: 输入信号长度
 * @param {Complex*} fft_out: fft完成后输出结果
 * @param {uint32_t} fft_point:  采样点数
 * @return {*} 
 */
uint32_t FFT(uint32_t* signal_in, uint32_t signal_len, Complex* fft_out, uint32_t fft_point)
{
	Complex *W;//旋转因子Wn
	uint32_t i,j,k,m;
	Complex temp1,temp2,temp3;//用于交换中间量
	double series;//序列级数
	if(signal_len < fft_point)//采样点数与信号数据对比 
	{
		for(i=0;i < signal_len;i++)
		{
			fft_out[i].real = signal_in[i];
			fft_out[i].imag = 0;
		}
		for(i=signal_len;i<fft_point;i++)//补0
		{
			fft_out[i].real = 0;
			fft_out[i].imag = 0;
		}
	}else if(signal_len == fft_point)
	{
		for(i=0;i < signal_len;i++)
		{
			fft_out[i].real = signal_in[i];
			fft_out[i].imag = 0;
		}
	}
	else
	{
		for(i=0;i < fft_point;i++)
		{
			fft_out[i].real = signal_in[i];
			fft_out[i].imag = 0;
		}
	}
	if(fft_point%2 != 0)
	{
		return 1;
	}
	
	W = (Complex *)malloc(sizeof(Complex) * fft_point);
	if (W == NULL)
	{
		return 1;
	}
	
	for (i = 0; i < fft_point; i++)
	{
		W[i].real = cos(2*PI/fft_point*i);	 //欧拉表示的实部
		W[i].imag = -1*sin(2*PI/fft_point*i);//欧拉表示的虚部
	}
	
	for(i = 0;i < fft_point;i++)
	{
		k = i;
		j = 0;
		series = log(fft_point)/log(2); //算出序列的级数
		while((series--) > 0)//利用按位与以及循环实现码位颠倒
		{
			j = j << 1;
			j|=(k & 1);
			k = k >> 1;
		}
		if(j > i) //将x(n)的码位互换
		{
			temp1 = fft_out[i];
			fft_out[i] = fft_out[j];
			fft_out[j] = temp1;
		}
	}
	series = log(fft_point)/log(2); //蝶形运算的级数
	
	for(i = 0; i<series;i++)
	{
		m = 1<<i; //移位 每次都是2的指数的形式增加，其实也可以用m=2^i代替
		for(j = 0;j<fft_point;j+=2*m) //一组蝶形运算，每一组的蝶形因子乘数不同
		{
			for(k=0;k<m;k++)//蝶形结点的距离  一个蝶形运算 每个组内的蝶形运算
			{
				Complex_MUL(fft_out[k+j+m],W[fft_point*k/2/m],&temp1);
				Complex_ADD(fft_out[j+k],temp1,&temp2);
				Complex_SUB(fft_out[j+k],temp1,&temp3);
				fft_out[j+k] = temp2;
				fft_out[j+k+m] = temp3;
			}
		}
	}
	free(W);
	return 0;
}

#define SIGNAL 320
#define N 2048
uint32_t signal_in[SIGNAL]= {0};
Complex fft_out[N];

int main()
{
	int i;
	FFT(signal_in, SIGNAL, fft_out, N);
	printf("输出FFT后的结果\n");
	for (i = 0;i < N;i++)
	{
		printf("%d,%lf,%lf\n", i, Complex_ABS(&fft_out[i]),fft_output[i]);
		// printf("%.4f",fft_out[i].real); //输出复数的实部
		// if(fft_out[i].imag>=0.0001)
		// {
		// 	printf("+%.4fj\n",fft_out[i].imag);  //当复数的虚补大于0.0001时，输出+ 虚部 j的形式
		// }
		// else if(fabs(fft_out[i].imag)<0.0001)
		// {
		// 	printf("\n");//当虚部小于0.001时，跳过虚部，不输出
		// }
		// else
		// {
		// 	printf("%.4fj\n",fft_out[i].imag);//上述两个条件除外的形式，输出 虚部 j的形式
		// }
	}
	return 0;
}
