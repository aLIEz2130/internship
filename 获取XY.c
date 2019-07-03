#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/input.h>
int main()
{
	struct input_event coordinate;
	int fb_event0 = 0;
	fb_event0 = open("/dev/input/event0", O_RDWR);
	if (fb_event0 == -1)
	{
		printf("open error");
		return -1;
	}
	while (1)
	{
		read(fb_event0, &coordinate, sizeof(struct input_event));
			if (coordinate.type == 3)
			{
				if (coordinate.code == 0)
				{
					printf("X = %d \n", coordinate.value);
				}
				else if (coordinate.code == 1)
				{
					printf("Y = %d \n", coordinate.value);
				}
			}
	}
	close(fb_event0);
	return 0;
}