#include <unistd.h>
#include <stdio.h>

#define Log(fmt,arg...) printf("[APP]=[%s:%d]="fmt"\n",__func__,__LINE__,##arg)
#define LogPath(fmt,arg...) printf("[APP]=[%s:%s:%d]"fmt"\n",__FILE__,__func__,__LINE__,##arg)
struct demo_operations 
{
	void (*first_fun)(void);
	void (*second_fun)(void);
	void (*third_fun)(void);

	const struct demo_operations* inherits;
};

void demo_finalize_operation(struct demo_operations* ops)
{
	void **begin = (void **)ops;
	void **end	 = (void **)&ops->inherits;
	void **pp;
	const struct demo_operations* cur;
	int index = 0;
	for (cur = ops->inherits; cur; cur = cur->inherits)
	{
		void **inherit = (void **)cur;
		for (pp = begin, index = 0; pp < end; pp++, inherit++)
		{
			if (!*pp)
			{
			//	Log("index = %d, *pp=0x%p *inherit=%p", index++,*pp,*inherit);
				*pp = *inherit;
			}
		}
	}
	ops->inherits = NULL;
	/*
	for (pp = begin; pp < end; pp++)
		if (IS_ERR(*pp))
			*pp = NULL;
			*/
	Log("aaaaaa");
}

void parent_first_fun(void)
{
	Log("parent_first_fun=0x%p",parent_first_fun);
}
void parent_second_fun(void)
{
	Log("parent_second_fun=0x%p",parent_second_fun);
}
void parent_third_fun(void)
{
	Log("parent_third_fun=0x%p",parent_third_fun);
}
void child_first_fun(void)
{
	Log("child_first_fun=0x%p",child_first_fun);
}
void child_second_fun(void)
{
	Log("child_second_fun=0x%p",child_second_fun);
}
void child_third_fun(void)
{
	Log("child_third_fun=0x%p",child_third_fun);
}

const struct demo_operations g_basic = {
	.first_fun	= parent_first_fun,
	.second_fun = parent_second_fun,
	.third_fun = parent_third_fun,
};

const struct demo_operations g_child = {
	.inherits = &g_basic,
	.first_fun = child_first_fun,
	.second_fun = child_second_fun,
};

int main(int argc, char** argv)
{
	demo_finalize_operation(&g_child);
	g_child.first_fun();
	g_child.second_fun();
	g_child.third_fun();
	return 0;
}

