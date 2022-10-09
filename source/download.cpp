// 通过 curl 获取远端数据到内存缓冲区，再一次性写入文件。

#include <iostream>
#include <string.h>
#include <curl/curl.h>

#define FILE_URL "http://cn.bing.com/th?id=OHR.GrandsCausses_EN-CN3335882379_800x480.jpg"
#define LOCAL_FILE "/tmp/curlTestDst.jpg"

//内存块结构体
struct MemoryStruct {
    char *memory;
    size_t size;

    MemoryStruct()
    {
        memory = (char *)malloc(1);
        size = 0;
    }

    ~MemoryStruct()
    {
        free(memory);
        memory = NULL;
    }
};

size_t HttpPostWriteBack(void *contents, size_t size, size_t nmemb, void *userp);
int progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);

int main()
{
    const char *netlink = FILE_URL;
    const char *output = LOCAL_FILE;

    //初始化全局资源
    curl_global_init(CURL_GLOBAL_ALL);
    //初始化句柄
    CURL *curl = curl_easy_init();

    //需要的话，可以设置代理
    //curl_easy_setopt(curl, CURLOPT_PROXY, "127.0.0.1:7890");

    // 访问网址
    curl_easy_setopt(curl, CURLOPT_URL, netlink);
    // 设置用户代理
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/72.0.3626.121 Safari/537.36");

    // 获取数据
    MemoryStruct chunk;
    // CURLOPT_WRITEDATA 设置 HttpPostWriteBack 的 userp 参数
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpPostWriteBack);

    // 实现下载进度
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_callback);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, nullptr);

    // 同步执行
    curl_easy_perform(curl);
    // 释放句柄
    curl_easy_cleanup(curl);
    // 释放全局资源
    curl_global_cleanup();

    //写出数据
    FILE *fp = fopen(output, "wb");
    if (fp == nullptr) {
        fprintf(stderr, "open file %s failed\n", output);
        return -1;
    }
    fwrite(chunk.memory, chunk.size, 1, fp);
    fclose(fp);
    return 0;
}

// 回调函数实现：一次请求可能多次调回调函数
size_t HttpPostWriteBack(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb; //一次回调返回的数据量
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = (char *)realloc(mem->memory, mem->size + realsize);
    if (ptr == NULL) {
        fprintf(stderr, "not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    return realsize; //必须返回真实的数据
}

int progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
    fprintf(stdout, "dltotal = %d, dlnow = %d, ultotal = %d, ulnow = %d\n", dltotal, dlnow, ultotal, ulnow);
    return 0;
}