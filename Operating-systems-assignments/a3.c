#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#define RESP_PIPE "RESP_PIPE_36611"
#define REQ_PIPE "REQ_PIPE_36611" 
int sharedmem_fd=-1;
char* sharedmem_ptr=NULL;
unsigned int sharedmem_size=0;
char *mapped_file=NULL;
int mapped_size=0;
int file_fd=-1;


int main()
{

	//creeaza RESP_PIPE
	if(mkfifo(RESP_PIPE,0600)<0)
	{
		perror("ERROR\ncannot create the response pipe");
		return 1;
	}

	//deschide REQ_PIPE in citire
	int req_fd=open(REQ_PIPE,O_RDONLY);
	if(req_fd==-1)
	{
		perror("ERROR\ncannot open the request pipe");
		unlink(RESP_PIPE);
		return 1;
	}

	//deschide RESP_PIPE in scriere
	int resp_fd=open(RESP_PIPE,O_WRONLY);
	if(resp_fd==-1)
	{
		perror("ERROR\ncannot open the response pipe");
		close(req_fd);
		unlink(RESP_PIPE);
		return 1;

	}

	//scrie in RESP_PIPE "START"
	char start_msg[10]="START!";
	if(write(resp_fd,start_msg,strlen(start_msg))!=strlen(start_msg))
	{
		perror("ERROR\ncannot write in the response pipe");
		close(req_fd);
		close(resp_fd);
		unlink(RESP_PIPE);
		return 1;
	}

	printf("SUCCESS\n");

	char request_msg[251];
	int index=0;
	char caracter;
	while(read(req_fd,&caracter,1)==1 && caracter!='!' && index<sizeof(request_msg)-1)
	{
		request_msg[index]=caracter;
		index++;
	}
	request_msg[index]='\0';//punem null la finalul sirului cand gasim !

	if(strcmp(request_msg,"PING")==0)
	{
		int num=36611;
		write(resp_fd,"PING!",5);
		write(resp_fd,"PONG!",5);
		write(resp_fd,&num,sizeof(num));
	}
	else if(strcmp(request_msg,"CREATE_SHM")==0)
	{
		//int sharedmem_size;
		if(read(req_fd,&sharedmem_size,sizeof(sharedmem_size))!=sizeof(sharedmem_size))
		{
			write(resp_fd,"CREATE_SHM!",11);
			write(resp_fd,"ERROR!",6);
		}
		else
		{
			sharedmem_fd=shm_open("/RFfQRC",O_CREAT|O_RDWR,0664);
			if(sharedmem_fd<0)
			{
				write(resp_fd,"CREATE_SHM!",11);
				write(resp_fd,"ERROR!",6);
			}
			else if(ftruncate(sharedmem_fd,sharedmem_size)==-1)
			{
				write(resp_fd,"CREATE_SHM!",11);
				write(resp_fd,"ERROR!",6);
				close(sharedmem_fd);
				sharedmem_fd=-1;

			}
			else
			{
				sharedmem_ptr=mmap(NULL,sharedmem_size,PROT_READ|PROT_WRITE,MAP_SHARED,sharedmem_fd,0);
				if(sharedmem_ptr==(void*)-1)
				{
					write(resp_fd,"CREATE_SHM!",11);
					write(resp_fd,"ERROR!",6);
					close(sharedmem_fd);
					sharedmem_fd=-1;
		
				}
				
					write(resp_fd,"CREATE_SHM!",11);
					write(resp_fd,"SUCCESS!",8);

				
			}
		}
	}
	else if(strcmp(request_msg,"WRITE_TO_SHM")==0)
	{
		unsigned int offset;
		unsigned int value;
		if(read(req_fd,&offset,sizeof(offset))!=sizeof(offset)||read(req_fd,&value,sizeof(value))!=sizeof(value))
		{
			write(resp_fd,"WRITE_TO_SHM!",13);
			write(resp_fd,"ERROR!",6);
	
		}
		else if(sharedmem_ptr==(void*)-1 || sharedmem_ptr==NULL)
		{
			write(resp_fd,"WRITE_TO_SHM!",13);
			write(resp_fd,"ERROR!",6);
		
		}
		//else if(offset>=sharedmem_size|| offset+sizeof(unsigned int)>sharedmem_size)
		else if(offset>sharedmem_size-sizeof(unsigned int))
		{
			//offset in afara zonei de memoriw partajata
			write(resp_fd,"WRITE_TO_SHM!",13);
			write(resp_fd,"ERROR!",6);
		}
		else
		{
			//scriem val in memoria partajata la offsetul indicat
	

			*((unsigned int*)((char*)sharedmem_ptr+offset))=value;

			write(resp_fd,"WRITE_TO_SHM!",13);
			write(resp_fd,"SUCCESS!",8);


		}

	}
	else if(strcmp(request_msg,"MAP_FILE")==0)
	{
		char filename[251];
		int idx=0;
		char c;
		
			while(read(req_fd,&c,1)==1 && c!='!' && idx<sizeof(filename)-1)
			{
				filename[idx]=c;
				idx++;
			}
			filename[idx]='\0';

			int file_fd=open(filename,O_RDONLY);
			if(file_fd<0)
			{
				write(resp_fd,"MAP_FILE!",9);
				write(resp_fd,"ERROR!",6);
			}
				else
				{
					//obtinem dimens fisierului
					mapped_size=lseek(file_fd,0,SEEK_END);
					lseek(file_fd,0,SEEK_SET);

					mapped_file=mmap(NULL,mapped_size,PROT_READ,MAP_PRIVATE,file_fd,0);
					if(mapped_file==(void*)-1)
					{
						write(resp_fd,"MAP_FILE!",9);
			    	    write(resp_fd,"ERROR!",6);	
					}

					write(resp_fd,"MAP_FILE!",8);
					write(resp_fd,"SUCESS!",7);

				}
			
		
	}




	close(resp_fd);
	close(req_fd);
	unlink(RESP_PIPE);
	

	if(sharedmem_ptr!=NULL &&sharedmem_ptr!=(void*)-1)
		munmap(sharedmem_ptr,sharedmem_size);
	if(sharedmem_fd!=-1)
	{
		close(sharedmem_fd);
		shm_unlink("/RFfQRC");
	}
	return 0;
}