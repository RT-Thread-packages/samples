# RT-Thread Sample
这里是一些文件系统的操作例程

## 例程介绍 ##

| 目录          | 例程                   |
| ------------- | ---------------------- |
| openfile      | 打开文件               |
| readwrite     | 文件读写               |
| rename        | 更改名称               |
| stat          | 取得状态               |
| mkdir         | 创建目录               |
| opendir       | 打开目录               |
| readdir       | 读取目录               |
| tell_seek_dir | 保存与设置读取目录位置 |

## API 列表 ##

| API                                                   | 功能                     |
| ----------------------------------------------------- | ------------------------ |
| int open(const char *pathname, int oflag, int mode);  | 打开文件                 |
| int close(int fd);                                    | 关闭文件                 |
| size_t read(int fd, void *buf, size_t count);         | 读取数据                 |
| size_t write(int fd, const void *buf, size_t count);  | 写入数据                 |
| int rename(const char *oldpath, const char *newpath); | 更改名称                 |
| int stat(const char *file_name, struct stat *buf);    | 取得状态                 |
| int mkdir(const char *path, mode_t mode);             | 创建目录                 |
| DIR* opendir(const char* name);                       | 打开目录                 |
| struct dirent* readdir(DIR *d);                       | 读取目录                 |
| off_t telldir(DIR *d);                                | 取得目录流的读取位置     |
| void seekdir(DIR *d, off_t offset);                   | 设置读取目录的位置       |
| void rewinddir(DIR *d);                               | 重设读取目录的位置到开头 |
| int closedir(DIR* d);                                 | 关闭目录                 |
| int rmdir(const char *pathname);                      | 删除目录                 |
| int mkfs(const char * fs_name, const char * device)   | 格式化文件系统           |

