#include <iostream>
#include "curl/curl.h"

size_t writeFuncCallback(const char* buffer, size_t size, size_t nmemb, /*std::string* stream*/ FILE* file)
{
	//size_t len = size * nmemb;
	//stream->append(buffer, len);

	size_t len = fwrite(buffer, size, nmemb, file);

	return len;
}

int main(int argc, char* argv[])
{
	// 全局初始化CURL库
	if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK)
	{
		return -1;
	}

	// 输出CURL库的信息
	curl_version_info_data* info_data = curl_version_info(CURLVERSION_LAST);
	std::cout << "libcurl version: " << info_data->version << " host: " << info_data->host << std::endl;

	std::string strBodyContent;
	FILE* file = fopen("./test.html", "w+");

	// 初始化一个请求对象
	CURL* pCurlObj = curl_easy_init();
	// 设置请求选项
	curl_easy_setopt(pCurlObj, CURLOPT_URL, "http://www.baidu.com");
	curl_easy_setopt(pCurlObj, CURLOPT_COOKIEFILE, "./cookies.txt");
	curl_easy_setopt(pCurlObj, CURLOPT_COOKIEJAR, "./cookies.txt");
	curl_easy_setopt(pCurlObj, CURLOPT_WRITEFUNCTION, writeFuncCallback);
	//curl_easy_setopt(pCurlObj, CURLOPT_WRITEDATA, &strBodyContent);
	curl_easy_setopt(pCurlObj, CURLOPT_WRITEDATA, file);
	// 发送请求
	CURLcode res = curl_easy_perform(pCurlObj);
	if (res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		return -1;
	}
	//std::cout << strBodyContent << std::endl;

	curl_easy_cleanup(pCurlObj);

	return 0;
}