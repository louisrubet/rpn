static void dump8(unsigned char* to_dump, unsigned long offset,
        unsigned long size)
{
    const int block_size = 1;
    const int block_per_line = 16;
    int max_line = size / block_size;
	unsigned char mychar;
	int i;
	int j;
    for (i = 0; i < max_line; i++)
	{
        if ((i % block_per_line) == 0)
		{
			if (i > 0)
			{
				printf(" ");
                for (j = i - block_per_line; j < i; j++)
				{
                    mychar = *(to_dump + j);
					if ((mychar < 32) || (mychar >= 127))
						mychar = '.';
					printf("%c", mychar);
				}
			}
            printf("\n%08lX:", offset + i * block_size);
		}
        printf(" %02hhX", *(to_dump + i));
	}
	if (i > 0)
	{
		printf(" ");
        for (j = (i >= block_per_line) ? (i - block_per_line) : 0; j < i; j++)
		{
            mychar = *(to_dump + j);
			if ((mychar < 32) || (mychar >= 127))
				mychar = '.';
			printf("%c", mychar);
		}
	}
	printf("\n");
}
