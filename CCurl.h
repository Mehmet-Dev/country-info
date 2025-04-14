#ifndef CCURL_H
#define CCURL_H

#include <curl/curl.h>
#include <iostream>
#include <string>
#include "json.hpp"

class CCurl
{
    public:
    std::string ReadBuffer;
    CCurl();
    std::string SimpleInfo(std::string country);
    std::string DetailedInfo(std::string country);

    private:
    CURL* curl;
    CURLcode res;
    nlohmann::json GetResponse(std::string country);
};

#endif