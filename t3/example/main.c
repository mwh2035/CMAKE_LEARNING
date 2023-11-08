#include <stdio.h>
#include <stdlib.h>
#include "bmi.h"
int main(int argc,char* argv[])
{
	float weight,high,personal_bmi;
	printf("共输入%d个参数\n",argc);
	if(argc != 4){
		printf("请输入4个参数\n");
	}
	else
	{	
		// 字符串转浮点
		high = atof(argv[2]);
		weight = atof(argv[3]);
		personal_bmi = mybmi(high,weight);
		printf("身高为：%fm,体重为：%fkg,BMI为：%f\n",high,weight,personal_bmi);
	}
	
}
