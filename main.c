#include <stdio.h>//printf
#include <sys/types.h>//open
#include <sys/stat.h>//open
#include <fcntl.h>//open
#include <unistd.h>//close
#include <sys/mman.h>//mmap、munmap
#include <pthread.h>
#include <strings.h>//bzero
#include <string.h> 
#include <linux/input.h>
#include <malloc.h>
#include <pthread.h>
#include "bmp.h"
#include "touch_screen.h"
#include "camera.h"

int showbmp(char *path,int fd_fb0,int startx,int starty);
int read_tsxy(int fd_event0,int *coordinate_x, int *coordinate_y);
int arm6818_color(int fd_fb0,int color, int coordinate_x, int coordinate_y, int wide, int height);
char *bmppath(int number);
char *bmppath1(int number);
int fd_fb0;
int pthread_flag=0;

void *pthread_task1 (void* arg)
{
	while(pthread_flag == 0)
	{
	arm6818_color(fd_fb0,0x00c9fea9,5,305,220,75);	
	time_t get_time;
	time(&get_time);
	char *p_time = NULL;
	p_time = ctime(&get_time);
    
	char *p_str_first = NULL;
	char *p_str_sec = NULL;

	//"Wed Jun 26 18:27:48 2019"
	p_str_first = strtok(p_time, " ");//星期 

	p_str_sec = strtok(NULL, " ");//月份 
	int month ;
	
	
	if(strcmp(p_str_sec,"Jan")==0)
	{
		month = 1;
	}
	else if(strcmp(p_str_sec,"Feb")==0)
	{
		month = 2;
	}
	else if(strcmp(p_str_sec,"Mar")==0)
	{
		month = 3;
	}
	else if(strcmp(p_str_sec,"Apr")==0)
	{
		month = 4;
	}
	else if(strcmp(p_str_sec,"May")==0)
	{
		month = 5;
	}
	else if(strcmp(p_str_sec,"Jun")==0)
	{
		month = 6;
	}
	else if(strcmp(p_str_sec,"Jul")==0)
	{
		month = 7;
	}
	else if(strcmp(p_str_sec,"Aug")==0)
	{
		month = 8;
	}
	else if(strcmp(p_str_sec,"Sep")==0)
	{
		month = 9;
	}
	else if(strcmp(p_str_sec,"Oct")==0)
	{
		month = 10;
	}
	else if(strcmp(p_str_sec,"Nov")==0)
	{
		month = 11;
	}
	else 
	{
		month = 12;
	}
//	printf("month= %d ",month);
	//"Wed Jun 26 18:27:48 2019"
	char *p_str_third = NULL;
	p_str_third = strtok(NULL, " ");//日 
	int day;
	day = atoi(p_str_third);
//	printf("day= %d ",day);
	
	char *p_str_forth = NULL;
	p_str_forth = strtok(NULL, ":");//时 
	int hour;
	hour = atoi(p_str_forth);
//	printf("hour= %d ",hour);
	
	char *p_str_fifth = NULL;
	p_str_fifth = strtok(NULL, ":");//分 
	int minute;
	minute = atoi(p_str_fifth);
//	printf("minute= %d ",minute);
	
	char *p_str_sixth = NULL;
	p_str_sixth = strtok(NULL, " ");//秒 
	int second;
	second = atoi(p_str_sixth);
//	printf("second= %d ",second);
	
	char *p_str_seventh = NULL;
	p_str_seventh = strtok(NULL, "\0");//年 
	int year;
	year = atoi(p_str_seventh);
	//printf("year= %d ",year);
	
	int start_timeyx=5,start_timeyy=350;
	int start_timehx=5,start_timehy=305;
	
	char *path=bmppath1(year/1000);
	showbmp(path,fd_fb0,start_timeyx,start_timeyy);
	
	path=bmppath1((year%1000)/100);
	showbmp(path,fd_fb0,start_timeyx+22,start_timeyy);
	
	path=bmppath1((year%100)/10);
	showbmp(path,fd_fb0,start_timeyx+44,start_timeyy);
	
	path=bmppath1(year%10);
	showbmp(path,fd_fb0,start_timeyx+66,start_timeyy);
	
	path=bmppath1(10);
	showbmp(path,fd_fb0,start_timeyx+88,start_timeyy);
	
	path=bmppath1(month/10);
	showbmp(path,fd_fb0,start_timeyx+110,start_timeyy);
	path=bmppath1(month%10);
	showbmp(path,fd_fb0,start_timeyx+132,start_timeyy);
	
	path=bmppath1(10);
	showbmp(path,fd_fb0,start_timeyx+154,start_timeyy);
	
	path=bmppath1(day/10);
	showbmp(path,fd_fb0,start_timeyx+176,start_timeyy);
	path=bmppath1(day%10);
	showbmp(path,fd_fb0,start_timeyx+198,start_timeyy);
	
	path=bmppath1(hour/10);
	showbmp(path,fd_fb0,start_timehx,start_timehy);
	path=bmppath1(hour%10);
	showbmp(path,fd_fb0,start_timehx+22,start_timehy);
	
	path=bmppath1(11);
	showbmp(path,fd_fb0,start_timehx+49,start_timehy);
	
	path=bmppath1(minute/10);
	showbmp(path,fd_fb0,start_timehx+76,start_timehy);
	path=bmppath1(minute%10);
	showbmp(path,fd_fb0,start_timehx+103,start_timehy);
	
	path=bmppath1(11);
	showbmp(path,fd_fb0,start_timehx+130,start_timehy);
	
	path=bmppath1(second/10);
	showbmp(path,fd_fb0,start_timehx+157,start_timehy);
	path=bmppath1(second%10);
	showbmp(path,fd_fb0,start_timehx+183,start_timehy);
	
	sleep(1);
   }
	pthread_exit(NULL);
}

int main()
{
	char login_account[7] ="123456";//起始账号 
	char login_password[7] ="123456";//起始密码 
	char input_account[7];//输入账号 
	char input_password[7];//输入密码 
	
	char *path=bmppath(11);//背景图片路径 
	fd_fb0 = open("/dev/fb0",O_RDWR);//打开屏幕设备 
	int fd_event0 = open("/dev/input/event0",O_RDWR);//打开触控屏设备 
	
	struct input_event coordinate;//触控屏结构体
	int ts_x=0, ts_y=0;//点击屏幕的x和y坐标 
	int input_account_number=0;//输入的账号数 
	int input_password_number=0;//输入的密码数 
	int input_flag=-1;//输入状态标识符 
	char* input_number_path=NULL;//输入数字标识符 
	int show_accountx=210,show_accounty=70;//显示的坐标 
	int show_passwordx=210,show_passwordy=150;//显示的坐标
	int input_fault=0,input_fault_number=0;
	
	showbmp(path,fd_fb0,0,0);//显示背景图片
	
	system("date -s 2019-06-27");
	system("date -s 12:00:00");
	pthread_t t_task1=0;
	pthread_create(&t_task1,NULL,pthread_task1,NULL);
	
	
	while(1)
	{
		if(input_fault==1)
		{
		path=bmppath(11);
		showbmp(path,fd_fb0,0,0);//显示背景图片
		show_accountx=210;show_accounty=70;
		show_passwordx=210,show_passwordy=150;
		input_account_number=0;input_password_number=0;
		input_fault = 0;
		input_flag=-1;
		}
		
		read_tsxy(fd_event0,&ts_x, &ts_y);
		if(ts_x>=140 && ts_x<=640 && ts_y >=60 && ts_y<=135)  //输入账号 
		{
			input_flag = 0;
		}
		
		if(ts_x>=140 && ts_x<=640 && ts_y >=136 && ts_y<=220)  //输入密码 
		{	
			input_flag = 1;
		}
		
		if(ts_x>=290 && ts_x<=350 && ts_y >=230 && ts_y<=270)//数字1
		{
			if(input_flag==0)
			{
			    input_number_path = bmppath(1);
				
				if(input_account_number<=6)
				{
				input_account[input_account_number++] = '1';
				showbmp(input_number_path,fd_fb0,show_accountx,show_accounty);
				show_accountx+=60;
				}
			}
			if(input_flag==1)
			{
				input_number_path = bmppath(10);
				
				if(input_password_number<=6)
				{
				input_password[input_password_number++] = '1';
				showbmp(input_number_path,fd_fb0,show_passwordx,show_passwordy);
				show_passwordx+=60;
			    }
			}	
		}
		
		if(ts_x>=370 && ts_x<=430 && ts_y >=230 && ts_y<=270)//数字2
		{
			if(input_flag==0)
			{
				input_number_path = bmppath(2);
				
				if(input_account_number<=6)
				{
				input_account[input_account_number++] = '2';
				showbmp(input_number_path,fd_fb0,show_accountx,show_accounty);
				show_accountx+=60;
			    }
			}
			if(input_flag==1)
			{
				input_number_path = bmppath(10);
				
				if(input_password_number<=6)
				{
				input_password[input_password_number++] = '2';
				showbmp(input_number_path,fd_fb0,show_passwordx,show_passwordy);
				show_passwordx+=60;
			    }
			}
		}
		
		if(ts_x>=450 && ts_x<=510 && ts_y >=230 && ts_y<=270)//数字3
		{	
			if(input_flag==0)
			{
				input_number_path = bmppath(3);
				
				if(input_account_number<=6)
				{
				input_account[input_account_number++] = '3';
				showbmp(input_number_path,fd_fb0,show_accountx,show_accounty);
				show_accountx+=60;
			    }
			}
			if(input_flag==1)
			{
				input_number_path = bmppath(10);
				
				if(input_password_number<=6)
				{
				input_password[input_password_number++] = '3';
				showbmp(input_number_path,fd_fb0,show_passwordx,show_passwordy);
				show_passwordx+=60;
			    }
			}
		}
		
		if(ts_x>=290 && ts_x<=350 && ts_y >=290 && ts_y<=330)//数字4
		{
			if(input_flag==0)
			{   
			    input_number_path = bmppath(4);
				
				if(input_account_number<=6)
				{
				input_account[input_account_number++] = '4';
				showbmp(input_number_path,fd_fb0,show_accountx,show_accounty);
				show_accountx+=60;
			    }
			}
			if(input_flag==1)
			{
				input_number_path = bmppath(10);
				
				if(input_password_number<=6)
				{
				input_password[input_password_number++] = '4';
				showbmp(input_number_path,fd_fb0,show_passwordx,show_passwordy);
				show_passwordx+=60;
			    }
			}
		}
		
		if(ts_x>=370 && ts_x<=430 && ts_y >=290 && ts_y<=330)//数字5
		{	
			if(input_flag==0)
			{
				input_number_path = bmppath(5);
				
				if(input_account_number<=6)
				{
				input_account[input_account_number++] = '5';
				showbmp(input_number_path,fd_fb0,show_accountx,show_accounty);
				show_accountx+=60;
			    }
			}
			if(input_flag==1)
			{
				input_number_path = bmppath(10);
				
				if(input_password_number<=6)
				{
				input_password[input_password_number++] = '5';
				showbmp(input_number_path,fd_fb0,show_passwordx,show_passwordy);
				show_passwordx+=60;
			   }
			}
		}
		
		if(ts_x>=450 && ts_x<=510 && ts_y >=290 && ts_y<=330)//数字6
		{
			if(input_flag==0)
			{
				input_number_path = bmppath(6);
				
				if(input_account_number<=6)
				{
				input_account[input_account_number++] = '6';
				showbmp(input_number_path,fd_fb0,show_accountx,show_accounty);
				show_accountx+=60;
			    }
			}
			if(input_flag==1)
			{
				input_number_path = bmppath(10);
				
				if(input_password_number<=6)
				{
				input_password[input_password_number++] = '6';
				showbmp(input_number_path,fd_fb0,show_passwordx,show_passwordy);
				show_passwordx+=60;
			   }
			}
		}
		
		if(ts_x>=290 && ts_x<=350 && ts_y >=370 && ts_y<=410)//数字7
		{
			if(input_flag==0)
			{
				input_number_path = bmppath(7);
				
				if(input_account_number<=6)
				{
				input_account[input_account_number++] = '7';
				showbmp(input_number_path,fd_fb0,show_accountx,show_accounty);
				show_accountx+=60;
			   }
			}
			if(input_flag==1)
			{
				input_number_path = bmppath(10);
				
				if(input_password_number<=6)
				{
				input_password[input_password_number++] = '7';
				showbmp(input_number_path,fd_fb0,show_passwordx,show_passwordy);
				show_passwordx+=60;
			    }
			}
		}
		if(ts_x>=370 && ts_x<=410 && ts_y >=370 && ts_y<=410)//数字8
		{
			if(input_flag==0)
			{
				input_number_path = bmppath(8);
				
				if(input_account_number<=6)
				{
				input_account[input_account_number++] = '8';
				showbmp(input_number_path,fd_fb0,show_accountx,show_accounty);
				show_accountx+=60;
			    }
			}
			if(input_flag==1)
			{
				input_number_path = bmppath(10);
				
				if(input_password_number<=6)
				{
				input_password[input_password_number++] = '8';
				showbmp(input_number_path,fd_fb0,show_passwordx,show_passwordy);
				show_passwordx+=60;
			    }
			}
		}
		if(ts_x>=450 && ts_x<=510 && ts_y >=370 && ts_y<=410)//数字9
		{
			if(input_flag==0)
			{
			    input_number_path = bmppath(9);
				
				if(input_account_number<=6)
				{
				input_account[input_account_number++] = '9';
				showbmp(input_number_path,fd_fb0,show_accountx,show_accounty);
				show_accountx+=60;
			    }
			}
			if(input_flag==1)
			{
				input_number_path = bmppath(10);
				
				if(input_password_number<=6)
				{
				input_password[input_password_number++] = '9';
				showbmp(input_number_path,fd_fb0,show_passwordx,show_passwordy);
				show_passwordx+=60;
			    }
			}
		}
		
		if(ts_x>=370 && ts_x<=430 && ts_y >=430 && ts_y<=470)//数字0 
		{
			if(input_flag==0)
			{
			    input_number_path = bmppath(0);
				
				if(input_account_number<=6)
				{
				input_account[input_account_number++] = '0';
				showbmp(input_number_path,fd_fb0,show_accountx,show_accounty);
				show_accountx+=60;
			    }
			}
			if(input_flag==1)
			{
				input_number_path = bmppath(10);
				
				if(input_password_number<=6)
				{
				input_password[input_password_number++] = '0';
				showbmp(input_number_path,fd_fb0,show_passwordx,show_passwordy);
				show_passwordx+=60;
			    }
			}
		}
		if(ts_x>=290 && ts_x<=350 && ts_y >=430 && ts_y<=470)//删除 
		{
			if(input_flag==0)
			{
				if(show_accountx>210)
				{
			    show_accountx-=60;
				input_account[--input_account_number] = '\0';
				arm6818_color(fd_fb0,0x00ffffff,show_accountx,show_accounty,60,40);
				}
			}
			
			if(input_flag==1)
			{
				if(show_passwordx>210)
				{
				show_passwordx-=60;
				input_password[--input_password_number] = '\0';
				arm6818_color(fd_fb0,0x00ffffff,show_passwordx,show_passwordy,60,40);
				} 
				
			}
			
		}
		if(ts_x>=450 && ts_x<=510 && ts_y >=430 && ts_y<=470)//确认 
		{
			if(input_account_number == 6 && input_password_number == 6 && strncmp(login_account,input_account,input_account_number-1)==0 && strncmp(login_password,input_password,input_password_number-1)==0)//匹配成功 
			{
				arm6818_color(fd_fb0,0x0000ff00,0,0,800,480);//全屏显示绿色
				input_fault_number=0;
				pthread_flag = 1;
			    pthread_join(t_task1,NULL);
				sleep(1); 
				break;
			}
			if(input_account_number !=6 || input_password_number != 6|| strncmp(login_account,input_account,input_account_number-1)!=0 || strncmp(login_password,input_password,input_password_number-1)!=0)//匹配失败 
			{
			    input_fault_number++;
			   
			    if(input_fault_number==3)
				{
					arm6818_color(fd_fb0,0x00000000,0,0,800,480);//全屏显示黑色 
					return -1;
				} 
			else 
			{
			    arm6818_color(fd_fb0,0x00ff0000,0,0,800,480);//全屏显示红色
				sleep(1); 
			}
			input_fault=1;
		    }
	    } 
	   // printf("input_account_number=%d input_password_number=%d\n",input_account_number,input_password_number);
    }
    int witch_flag=0;
    showbmp("/root/project/images/groundback.bmp",fd_fb0,0,0);
    pthread_flag=0;
    pthread_create(&t_task1,NULL,pthread_task1,NULL);
    while(1)
	{
		read_tsxy(fd_event0,&ts_x, &ts_y);
		if(ts_x>=200 && ts_x<=300 && ts_y>=200 && ts_y<=280)//监控 
		{
			pthread_flag = 1;
			pthread_join(t_task1,NULL);
			
			open_ts();
	    	show_bmp800_480("/root/project/images/camera.bmp");
	        my_camera();
            close_ts();
            showbmp("/root/project/images/groundback.bmp",fd_fb0,0,0);
            pthread_flag=0;
            pthread_create(&t_task1,NULL,pthread_task1,NULL);
           
		}
		if(ts_x>=500 && ts_x<=560 && ts_y>=200 && ts_y<=275)//退出 
		{
			pthread_flag = 1;
			pthread_join(t_task1,NULL);
			break;
		}
	}
	arm6818_color(fd_fb0,0x00000000,0,0,800,480);//全屏显示黑色 
	close(fd_fb0);
	close(fd_event0);
	
	return 0;
 } 
 
int showbmp(char *path,int fd_fb0,int startx,int starty)
{
	int bmp_bp0=0,width=0,high=0;
	char* bmp_buf=NULL;
	int* lcd_buf=NULL;
	int i=0,j=0,swap=0,A=0x00,len=800*480*4;
	int* p_mmap=NULL;
	int* p,*q;
	
	p=&width;q=&high;
	
	if(fd_fb0<=0)
	{
		printf("open lcd error\n");
		return -1;
	}
	
	bmp_bp0 = open(path,O_RDWR);
	if(bmp_bp0<=0)
	{
		printf("open bmp error\n");
		return -1;
	}
	lseek(bmp_bp0,18,SEEK_SET);
	read(bmp_bp0,p,4);
	read(bmp_bp0,q,4);
	bmp_buf = (char *)malloc(width*high*3*sizeof(char));
	lcd_buf = (int *)malloc(width*high*sizeof(int));

	if(lseek(bmp_bp0,54,SEEK_SET)==-1)
	{
		printf("lseek error\n");
		return -1;
	}
	
	if(read(bmp_bp0,bmp_buf,width*high*3)==-1)
	{
		
		printf("read bmp error\n");
		return -1;
	}
	
	for(i=0;i<width*high*3;i+=3)
	{
	    lcd_buf[j++]=bmp_buf[i+0]<<0 | bmp_buf[i+1]<<8 | bmp_buf[i+2]<<16 | A<<24;
	}
	
	p_mmap = mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED,fd_fb0,0);
	if(p_mmap==MAP_FAILED) 
	{
		printf("mmap error\n");
		return -1; 
	} 
	
	int *p_user = p_mmap;
	
	p_user = p_user + 800*starty + startx;
	
	
	
	for(i=0; i<high && i+starty<480; i++)
	{
		for(j=0; j<width && j+startx<800; j++)
		{
			*(p_user+i*800+j)=lcd_buf[(high-1-i)*width+j];
			
		}
	}
    free(bmp_buf);
    free(lcd_buf);
	munmap(p_mmap,len);
	close(bmp_bp0);
}

int read_tsxy(int fd_event0,int *coordinate_x, int *coordinate_y)
{
	struct input_event coordinate;
	while(1)
	{		
		read(fd_event0, &coordinate,sizeof(struct input_event));
		
		if(coordinate.type==3 && coordinate.code==0 && coordinate.value>=0 && coordinate.value<800)//X
		{
			*coordinate_x = coordinate.value;
		}
		if(coordinate.type==3 && coordinate.code==1 && coordinate.value>=0 && coordinate.value<480)//Y
		{
			*coordinate_y = coordinate.value;
		}		
		if(coordinate.type==1 && coordinate.code==330 && coordinate.value==0)
		{
			break;
		}	
	}	
	
	return 0;
}

int arm6818_color(int fd_fb0,int color, int coordinate_x, int coordinate_y, int wide, int height)
{
	int *p_mmap = NULL;
	p_mmap = mmap(NULL, 800*480*4, PROT_READ|PROT_WRITE , MAP_SHARED, fd_fb0, 0);
	if(p_mmap == MAP_FAILED)
	{
		printf("mmap faile!\n");
		return -1;		
	}
	
	int *p_user = p_mmap;
	
	p_user = p_user + 800*coordinate_y + coordinate_x;

	int x, y;
	for(y=0; y<height; y++)
	{
		for(x=0; x<wide; x++)
		{
			*(p_user + 800*y + x) = color;
		}
	}
	
	return 0;
}

char *bmppath(int number)
{
	char *path = NULL;
	if(number==0) 
	{
		path = "/root/project/images/0.bmp";
	}
	else if(number==1)
	{
		path = "/root/project/images/1.bmp";
	}
	else if(number==2)
	{
		path = "/root/project/images/2.bmp";
	}
	else if(number==3)
	{
		path = "/root/project/images/3.bmp";
	}
	else if(number==4)
	{
		path = "/root/project/images/4.bmp";
	}
	else if(number==5)
	{
		path = "/root/project/images/5.bmp";
	}
	else if(number==6)
	{
		path = "/root/project/images/6.bmp";
	}
	else if(number==7)
	{
		path = "/root/project/images/7.bmp";
	}
	else if(number==8)
	{
		path = "/root/project/images/8.bmp";
	}
	else if(number==9)
	{
		path = "/root/project/images/9.bmp";
	}
	else if(number==10)
	{
		path = "/root/project/images/mistry.bmp";
	}
	else if(number==11)
	{
		path = "/root/project/images/main.bmp";
	}
	return path;
}

char *bmppath1(int number)
{
	char *path = NULL;
	if(number==0) 
	{
		path = "/root/project/images/00.bmp";
	}
	else if(number==1)
	{
		path = "/root/project/images/11.bmp";
	}
	else if(number==2)
	{
		path = "/root/project/images/22.bmp";
	}
	else if(number==3)
	{
		path = "/root/project/images/33.bmp";
	}
	else if(number==4)
	{
		path = "/root/project/images/44.bmp";
	}
	else if(number==5)
	{
		path = "/root/project/images/55.bmp";
	}
	else if(number==6)
	{
		path = "/root/project/images/66.bmp";
	}
	else if(number==7)
	{
		path = "/root/project/images/77.bmp";
	}
	else if(number==8)
	{
		path = "/root/project/images/88.bmp";
	}
	else if(number==9)
	{
		path = "/root/project/images/99.bmp";
	}
	else if(number==10)
	{
		path = "/root/project/images/g.bmp";
	}
	else if(number==11)
	{
		path = "/root/project/images/mh.bmp";
	}
	return path;
}
