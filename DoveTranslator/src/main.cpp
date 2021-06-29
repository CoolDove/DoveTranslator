//--------------------------------------------
// Copyright (c) 2021 Dove All rights reserved.
//--------------------------------------------

#include <curl/curl.h>
#include <openssl/md5.h>

#include "json/json.h"
#include "platform.h"

#ifdef WIN
#pragma comment(lib, "libcurl.dll.lib")
#pragma comment(lib, "libcrypto.lib")
#pragma warning(disable : 4996) 
#endif

CURL* g_curl;
std::vector<std::string> g_args_query;

std::string g_appid		 = "";
std::string g_secret_key = "";
std::string g_from		 = "auto";
std::string g_to		 = "auto";
bool g_terminal_tag		 = false;
bool g_sentence_tag		 = false;

const std::string cg_languages[13] = {
	"auto",	 //自动
	"zh",	 //中文
	"en",	 //英语
	"jp",	 //日语
	"kor",	 //韩语
	"de",	 //德语
	"ru",	 //俄语
	"spa",	 //西班牙语
	"fra",	 //法语
	"fin",	 //芬兰语
	"it",	 //意大利语
	"wyw",	 //文言文
	"cht"	 //繁体中文
};

void fn_help() {
	using namespace std;
	printf("Dove Translator Help: \n");
	printf("language: \n");
	printf("  a\t-auto\t|");
	printf("z\t-zh\t|");
	printf("e\t-en\n");
	printf("  j\t-jp\t|");
	printf("k\t-kor\t|");
	printf("d\t-de\n");
	printf("  r\t-ru\t|");
	printf("s\t-spa\t|");
	printf("f\t-fra\n");
	printf("  n\t-fin\t|");
	printf("i\t-it\t|");
	printf("w\t-wyw\n");
	printf("  c\t-cht\n\n");
	printf("option: \n");
	printf("  -help  -h : help\n");
	printf("  -s        : sentence mode\n");
	printf("  -init     : initialize Dove Translator\n");
	exit(0);
}

int fn_get_language(const char& tag) {
	switch (tag)
	{
		case 'a':
			return 0;
		case 'z':
			return 1;
		case 'e':
			return 2;
		case 'j':
			return 3;
		case 'k':
			return 4;
		case 'd':
			return 5;
		case 'r':
			return 6;
		case 's':
			return 7;
		case 'f':
			return 8;
		case 'n':
			return 9;
		case 'i':
			return 10;
		case 'w':
			return 11;
		case 'c':
			return 12;
		default:
			return 13;
	}
}

void fn_set_language(const char& from, const char& to) {
	int i_from = fn_get_language(from);
	int i_to   = fn_get_language(to);

	if (i_from == 13 || i_to == 13) fn_help();

	g_from = cg_languages[i_from];
	g_to   = cg_languages[i_to];
}

std::string fn_build_query() {
	if (g_args_query.size() == 0) fn_help();

	std::string query;
	for (size_t i = 0; i < g_args_query.size(); i++)
	{
		if (i != 0 || i != g_args_query.size() - 1)
		{
			if (g_sentence_tag)
				query.append(" ");
			else
				query.append("\n");
		}

		query.append(g_args_query[i]);
	}

	return fn_conv_to_utf8(query);
}

void fn_init_account(std::string& _appid, std::string& _secret_key) {
	std::string path = get_config_path();

	std::cout << "Dove Translator initializing: " << std::endl;
	std::cout << "--Input your AppID: ";
	std::cin >> _appid;
	std::cout << "--Input your Secret Key: ";
	std::cin >> _secret_key;

	std::ofstream ofs(path);
	if (ofs)
	{
		ofs << _appid << std::endl;
		ofs << _secret_key << std::endl;
		std::cout << "√-Dove Translator initialized! " << std::endl;
		ofs.close();
	}
	exit(0);
}

void fn_read_config(std::string& _appid, std::string& _secret_key) {
	std::string path = get_config_path();

	std::ifstream ifs(path);
	if (ifs)
	{
		std::getline(ifs, _appid);
		std::getline(ifs, _secret_key);
	}
	else
	{
		fn_init_account(_appid, _secret_key);
	}
	ifs.close();
}

void fn_read_args(int argc, char* argv[]) {
	for (size_t i = 1; i < argc; i++)
	{
		std::string arg = argv[i]; 
		if (arg == "-init") fn_init_account(g_appid, g_secret_key);
		if (arg == "-help" || arg == "-h")
			fn_help();
		else if (arg == "-s")
			g_sentence_tag = true;
		else if (arg[0] == '-')
			fn_set_language(arg[1], arg[2]);
		else
			g_args_query.push_back(arg);
	}
}

Json::Value fn_get_root_from_string(const std::string& source) {
	Json::CharReaderBuilder reader_builder;
	Json::CharReader* json_reader;
	json_reader = reader_builder.newCharReader();

	Json::Value json_root;
	Json::String err;

	json_reader->parse(&source[0], &source[source.length() - 1], &json_root,
					   &err);

	delete json_reader;
	return json_root;
}

size_t fn_on_write_data(char* ptr, size_t size, size_t nmemb, void* stream) {
	std::string json_content = ptr;
	Json::Value root		 = fn_get_root_from_string(json_content);

	if (!root["error_code"].isNull())
	{
		std::cout << "Error: " << root["error_code"].asString() << "-" << root["error_msg"].asString() << std::endl;
		return size * nmemb;
	}

	if (!root["trans_result"].isNull() && root["trans_result"].size() <= 1)
		std::cout << root["trans_result"][0]["dst"].asString() << std::endl;
	else
	{
		for (int i = 0; i < root["trans_result"].size(); i++)
			std::cout << root["trans_result"][i]["src"].asString() << "\t\t>>\t" << root["trans_result"][i]["dst"].asString() << std::endl;
	}

	return size * nmemb;
}

int main(int argc, char* argv[]) {
	fn_read_config(g_appid, g_secret_key);

	if (argc < 2) fn_help();

	fn_read_args(argc, argv);

	CURLcode res;

	g_curl = curl_easy_init();
	if (g_curl)
	{
		std::string myurl = "http://api.fanyi.baidu.com/api/trans/vip/translate?";

		std::string q		= fn_build_query();
		char* query_encoded = curl_easy_escape(g_curl, q.c_str(), q.length());

		char salt[60];
		srand(time(NULL));
		int a = rand();
		sprintf(salt, "%d", a);

		std::string sign = "";
		sign.append(g_appid.c_str());
		sign.append(q.c_str());
		sign.append(salt);
		sign.append(g_secret_key.c_str());

		if (g_terminal_tag) return 0;

		// MD5
		unsigned char md[16];
		int i;
		char tmp[3] = {'\0'}, buf[33] = {'\0'};
		MD5((unsigned char*)sign.c_str(), strlen((char*)sign.c_str()), md);
		for (i = 0; i < 16; i++)
		{
			sprintf(tmp, "%2.2x", md[i]);
			strcat(buf, tmp);
		}

		//---
		myurl.append("appid=");
		myurl.append(g_appid);
		myurl.append("&q=");
		myurl.append(query_encoded);
		myurl.append("&from=");
		myurl.append(g_from);
		myurl.append("&to=");
		myurl.append(g_to);
		myurl.append("&salt=");
		myurl.append(salt);
		myurl.append("&sign=");
		myurl.append(buf);

		// set curlopt
		curl_easy_setopt(g_curl, CURLOPT_URL, myurl.c_str());
		curl_easy_setopt(g_curl, CURLOPT_WRITEFUNCTION, fn_on_write_data);

		res = curl_easy_perform(g_curl);

		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

		/* always cleanup */
		curl_easy_cleanup(g_curl);
	}
	return 0;
}
