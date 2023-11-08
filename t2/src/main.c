#include <stdio.h>
#include <stdlib.h>
int main(int argc,char* argv[])
{
	float weight,high;
	printf("共输入%d个参数\n",argc);
	if(argc != 4){
		printf("请输入4个参数\n");
	}
	else
	{	
		// 字符串转浮点
		high = atof(argv[2]);
		weight = atof(argv[3]);
		printf("身高为：%fm,体重为：%fkg,BMI为：%f\n",high,weight,weight/high/high);
	}
	
}
