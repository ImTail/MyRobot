

/*
	功能：去除掉字符串前面的\0，并且去除掉所有的换行，回车和空格符号。
	输入：字符串指针
	输出：原字符串指针
*/
char* MyStringTool_Remove_Emp(char* str)
{
	unsigned char copySrc = 0;
	unsigned char copyDst = 0;
	
	unsigned char findChar = 0;//标记是否找到了第一个不是null的字符
	
	while (1)
	{
		if (findChar==0)
		{
			if (*(str+copySrc)!='\0')
			{
				findChar = 1;
				continue;
			}
			copySrc++;
		}
		else
		{
			if (*(str+copySrc) == '\0')
			{
				break;
			}
			if (*(str+copySrc) == '\r' || *(str+copySrc) == '\n' || *(str+copySrc) == ' ' || *(str+copySrc) == ' ')
			{
				copySrc++;
				continue;
			}
			*(str+copyDst)=*(str+copySrc);
			copyDst++;
			copySrc++;
		}
		
	}
	*(str+copyDst)='\0';
	
	return str;
}
