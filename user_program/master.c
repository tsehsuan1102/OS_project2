#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

#define PAGE_SIZE 4096
#define BUF_SIZE 512
#define MAP_SIZE 4096*128
size_t get_filesize(const char* filename);//get the size of the input file


int main (int argc, char* argv[])
{
	char buf[BUF_SIZE];
	int i=atoi(argv[1]), dev_fd, file_fd;// the fd for the device and the fd for the input file
	size_t ret, file_size, offset = 0, tmp;
	int sended, now_send;
	char file_name[50], method[20];
	char *kernel_address = NULL, *file_address = NULL;
	struct timeval start;
	struct timeval end;
	double trans_time; //calulate the time between the device is opened and it is closed

	strcpy(method,argv[2+i]);
	for(int j=0; j<i; j++){
		strcpy(file_name, argv[2+j]);
		if( (dev_fd = open("/dev/master_device", O_RDWR)) < 0)
		{
			perror("failed to open /dev/master_device\n");
			return 1;
		}
		gettimeofday(&start ,NULL);
		if( (file_fd = open (file_name, O_RDWR)) < 0 )
		{
			perror("failed to open input file\n");
			return 1;
		}

		if( (file_size = get_filesize(file_name)) < 0)
		{
			perror("failed to get filesize\n");
			return 1;
		}


		if(ioctl(dev_fd, 0x12345677) == -1) //0x12345677 : create socket and accept the connection from the slave
		{
			perror("ioclt server create socket error\n");
			return 1;
		}


		switch(method[0])
		{
			case 'f': //fcntl : read()/write()
				do
				{
					ret = read(file_fd, buf, sizeof(buf)); // read from the input file
					write(dev_fd, buf, ret);//write to the the device

				}while(ret > 0);
				break;
			case 'm': //fcntl : read()/write()
				sended=0;
				while(sended<file_size){
					now_send = (sended+MAP_SIZE<=file_size)?MAP_SIZE:file_size-sended;
					file_address = mmap (NULL, now_send, PROT_READ, MAP_SHARED, file_fd, sended);
					kernel_address = mmap (NULL, now_send, PROT_READ|PROT_WRITE, MAP_SHARED, dev_fd, sended);
					sended+=now_send;
					memcpy(kernel_address, file_address, now_send);
					ioctl(dev_fd, 0x12345678 ,now_send);
					munmap(file_address, now_send);
					munmap(kernel_address, now_send);

				}
				//flush
				ioctl(dev_fd, 0 ,now_send);


		}

		if(ioctl(dev_fd, 0x12345679) == -1) // end sending data, close the connection
		{
			perror("ioclt server exits error\n");
			return 1;
		}
		gettimeofday(&end, NULL);
		trans_time = (end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)*0.0001;
		printf("Transmission time: %lf ms, File size: %d bytes\n", trans_time, file_size / 8);

		close(file_fd);
		close(dev_fd);
	}


	return 0;
}

size_t get_filesize(const char* filename)
{
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}
