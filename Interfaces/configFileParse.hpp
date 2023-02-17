#ifndef _PARSING_HPP_
#define _PARSING_HPP_
#include <set>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <utility>
#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <algorithm>
#include <fcntl.h>
#include <cstdio>
#include "locationBlockParse.hpp"
#define MISSING_SERVER_NAME "Provide server name"
#define INVALID_PORT_MSG "Port must contain numbers only."
#define LISTEN_ERROR_MSG "Error on listen keyword"
#define ERROR_PAGE_ERROR_MSG "Error on error_page keyword"
#define SERVER_KEYWORD "server"
#define LISTEN_KEYWORD "listen"
#define SERVER_NAME_KEYWORD "server_name"
#define ERROR_PAGE_KEYWORD "error_page"
#define MAX_CLIENT_BODY_SIZE_KEYWORD "max_client_body_size"
#define INVALID_KEYWORD " : Is An Invalid Keyword."
#define DEFAULT_CONFIG_FILE_NAME "parsing/file.conf"
#define CONFIG_FILE_COULDNT_OPEN_MSG "Couldn't Open Config File."
#define SERVER_KEYWORD_MSG "Error on server keyword"
#define ERROR_PAGE_FILE_NOT_FOUND "Error_Page file is not exist !!"
#define SOCKET  int
#define MAX_REQUEST_SIZE 2000


class configFileParse{
    public:
        configFileParse();
        std::string serverHost;
        std::string serverPort;
        unsigned int clientBodyLimit;
        bool isClosed;
        std::list<std::string> serverName;
        std::list<std::pair<int, std::string> > errorInfo;
        std::list<locationBlockParse> Locations;
        void fillingDataFirstPart(std::string &);
        void listenKeywordFound(std::vector<std::string> &);
        void serverNameKeywordFound(std::vector<std::string> &);
        void errorPageKeywordFound(std::vector<std::string> &);
        void clientBodySizeKeywordFound(std::vector<std::string> &);
        static void     startParsingFile(std::list<std::string> &, std::list<configFileParse> &);
        static void     readingDataFromFile(std::list<std::string> &, std::string &);
        static void     printingParsingData(std::list<configFileParse> &);
};


bool    isValidNumber(std::string &);
void    errorPrinting(const char *);
#endif