//主要归纳文件操作函数，不包括文件内容的操作


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*函数简要说明
*
*
*            函数名                                             用法                                备注
*
*1. int open(const char *pathname, int flags);    open and possibly create a file or device      flags 必须包含O_RDONLY, O_WRONLY, or O_RDWR中的任何一个
*
*2. int open(const char *pathname, int flags, mode_t mode);     UP                              mode只是在flags中包含O_CREAT时才有效
*
*3. int fsync(int fd);                            synchronize a file's in-core state with storage device  写完数据close前必须先同步，以防意外
*
*4. off_t lseek(int fd, off_t offset, int whence);  定位文件位置                                第三个参数可以为SEEK_SET SEEK_CUR SEEK_END
*
*5. ssize_t read(int fildes, void *buf, size_t nbyte); UP                                       文件位置会随读取的字节数移动
*
*6. ssize_t write(int fildes, const void *buf, size_t nbyte);   UP                              UP
*
*7. int close(int fd);                                          UP                              UP
*
*8.  void *mmap(void *addr, size_t length, int prot, int flags, 内存映射                        先用fstat得到文件大小，然后使用该函数将文件内容映射到内存中，然后就可以

                  int fd, off_t offset);                                                        直接调用字符串函数操作。
*
*9. int munmap(void *addr, size_t length);                      释放内存                        UP
*
*10. int ftruncate(int fd, off_t length);            truncate a file to a specified length      在新建文件时，如果用到内存映射，那么就需要先调用该函数给文件预置一个大小，否则会出错
*
*11. int fstat(int fd, struct stat *buf);           获得文件的基本信息                          具体参数见man，可以查到文件的大小、修改日期等
*
*/



/*标准C函数
*               函数名                                      用法                                        备注
*
*1. FILE *fopen(const char *path, const char *mode);    NT                                      mode可以为r,r+,w,w+,a,a+,rw等
*
*2. int fseek(FILE *stream, long offset, int whence);   NT                                      定位文件位置
*
*3. long ftell(FILE *stream);                           当前文件位置                            NT
*
*4. void rewind(FILE *stream);                          移动文件位置到文件头                    NT
*
*5. size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);   NT                      NT
*
*6. size_t fwrite(const void *ptr, size_t size, size_t nmemb,FILE *stream); NT                  NT
*
*7. int feof(FILE *stream);                             判断文件是否已经读完                    NT
*
*8. int fscanf(FILE *stream, const char *format, ...);  按格式读取文件流                        还有scanf家族的其他函数，比如sscanf可以读取字节流
*
*9. int fprintf(FILE *stream, const char *format, ...); 按格式将内容写入到文件中                还有sprintf,vprintf等等
*
*10. int vfprintf(FILE *stream, const char *format, va_list ap);    直接将不定参数写入到文件    相对应的还有读取函
*
*11. int fflush(FILE *stream);                          NT                                      NT
*
*12. char *fgets(char *s, int size, FILE *stream);       NT                                      读到换行符或EOF时停止
*
*13. int fileno(FILE *stream);                          返回整型文件描述符                      这是和GLIBC相通的地方
*
*14. int fputs(const char *s, FILE *stream);            NT                                      不写入结束符\0
*
*/


/*文件权限控制类函数汇总
*
*           函数名                                          用法                                    备注
*
*1. int remove(const char *pathname);                       NT                                      删除指定路径的文件
*
*2. int rename(const char *old, const char *new);           NT                                      重命名
*
*3. int utime(const char *path, const struct utimbuf *times);   NT                                  修改文件的存取时间
*
*4. int stat(const char *restrict path, struct stat *restrict buf); NT                              可以进一步获得文件的详细信息
*
*5. int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result);
                                                            NT                                      读取文件夹内容
*
*6. DIR *opendir(const char *name);                         NT                                      打开文件夹，这是第一步
*
*7. char *getcwd(char *buf, size_t size);                   NT                                      获得当前的绝对路径
*
*8. int chmod(const char *path, mode_t mode);               NT                                      改变文件权限
*
*9. int closedir(DIR *dirp);                                NT                                      打开文件夹后必须要关闭
*
*10. int chdir(const char *path);                           NT                                      改变当前的工作目录
*
*/











/*处理不定参需要用到的函数
*
*1. void va_start(va_list ap, last);                    必须首先被调用
*
*2. type va_arg(va_list ap, type);                      紧接着，根据%s %d %c得到实际的值
*
*3. void va_end(va_list ap);                            得到需要的值后，就调用end。
*
*