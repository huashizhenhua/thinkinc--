#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define IPC_KEY 0x33


int shm_write(char* content) {
    key_t key = ftok("/tmp", IPC_KEY);
    char* str; 
 
    if (key == -1) {
        perror("ftok error");
        return -1;
    }
    
    int shm_id = shmget(key, 4096, IPC_CREAT|IPC_EXCL|0600);
    if (shm_id == -1) {
        perror("shmget error");
        return -1;
    }

    printf("shm_id=%d\n", shm_id);

    str = (char*)shmat(shm_id, NULL, 0);
    strcpy(str, content);
    shmdt(str);
  

}

char* shm_read(){
    key_t key = ftok("/tmp", IPC_KEY);

    int shm_id = shmget(key, 0, 0);
    if (shm_id == -1) {
        perror("shmget error");
        return NULL;
    }

    printf("shm_id=%d\n", shm_id);

    
    char* shm_content= (char*)shmat(shm_id, NULL, 0);

    return shm_content;
}


int main()
{
	int segment_id;
	char* shared_memory;
	struct shmid_ds shmbuffer;
	int segment_size;
	const int shared_segment_size = 0x6400;
        
        // 分配一个共享内存块(shmget) 
	segment_id = shmget(IPC_PRIVATE, shared_segment_size, IPC_CREAT|IPC_EXCL|S_IRUSR|S_IWUSR ); 
    
        // 绑定到共享内存块(shmat)
        printf("segment_id=%d" , segment_id);
	shared_memory = (char*)shmat(segment_id, 0, 0);
	printf("shared memory attached at address %p\n", shared_memory); 

        // 确定共享内存的大小(shmctl)
	shmctl(segment_id, IPC_STAT, &shmbuffer); // 
	segment_size = shmbuffer.shm_segsz;
	printf("segment size: %d\n", segment_size);
	sprintf(shared_memory, "Hello, world."); // 在共享内存中写入一个字符串 
	shmdt(shared_memory); // 脱离该共享内存块 
	shared_memory = (char*)shmat(segment_id, (void*) 0x500000, 0);// 重新绑定该内存块 
	printf("shared memory reattached at address %p\n", shared_memory);
	printf("%s\n", shared_memory); // 输出共享内存中的字符串 
	shmdt(shared_memory); // 脱离该共享内存块 
	shmctl(segment_id, IPC_RMID, 0);// 释放这个共享内存块


        pid_t pid = getpid();
        printf("pid->%d", pid);
        char cmd[30]; 
        while(1) {
            scanf("%s", cmd);
            //printf("input = %s" , cmd);
            if (strcmp(cmd, "write") == 0) {
                shm_write("shm write test");      
            }
            else if (strcmp(cmd, "read") == 0) {
                char* readShmStr = (char*)shm_read();
                printf("result = %s", readShmStr);              
            }
        } 
	return 0;
}
