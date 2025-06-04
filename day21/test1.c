#include "my_header.h"
#define ONE_COPY_MAX_SIZE (4096 *1000)
int main(int argc,char *argv[]){

    int src_fd=open(argv[1],O_RDONLY);
    ERROR_CHECK(src_fd,-1,"OPEN SRC FAILED");

    int dest_fd=open(argv[2],O_RDWR|O_CREAT|O_TRUNC);
    ERROR_CHECK(dest_fd,-1,"OPEN DESR FAILED");

    struct stat sb;
    fstat(src_fd,&sb);

    //off_t  is long
    off_t src_size=sb.st_size;
    off_t src_copied_size=0;

    //先把要复制的大小空间预定
    ftruncate(dest_fd,src_size);

    while (src_copied_size<src_size)
    {
       off_t current_copy_size =(src_size-src_copied_size) > 
       ONE_COPY_MAX_SIZE ? ONE_COPY_MAX_SIZE : 
       (src_size-src_copied_size);
        //length 指定要映射的内存块大小
        //offset 指定开始映射的位置
        char *src_p=(char *)mmap(NULL,current_copy_size,
        PROT_READ,MAP_SHARED,src_fd,src_copied_size);
        char *dest_p = (char *)mmap(NULL, current_copy_size,
             PROT_READ | PROT_WRITE, MAP_SHARED, dest_fd, 
             src_copied_size);
        ERROR_CHECK(src_p, MAP_FAILED, "mmap failed");
        ERROR_CHECK(dest_p, MAP_FAILED, "mmap failed");
        
        for(off_t i=0;i<current_copy_size;i++){
            dest_p[i]=tolower(src_p[i]);
        }
        
        memcpy(dest_p,src_p,current_copy_size);
        munmap(src_p,current_copy_size);
        munmap(dest_p,current_copy_size);

        src_copied_size+=current_copy_size;
       
    }
    close(src_fd);
    close(dest_fd);
    
}