

/*
	���ܣ�ȥ�����ַ���ǰ���\0������ȥ�������еĻ��У��س��Ϳո���š�
	���룺�ַ���ָ��
	�����ԭ�ַ���ָ��
*/
char* MyStringTool_Remove_Emp(char* str)
{
	unsigned char copySrc = 0;
	unsigned char copyDst = 0;
	
	unsigned char findChar = 0;//����Ƿ��ҵ��˵�һ������null���ַ�
	
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
