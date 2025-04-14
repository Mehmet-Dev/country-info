#include "CCurl.h"

using json = nlohmann::json;

const std::string defaultLink = "https://restcountries.com/v3.1/name/";

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

CCurl::CCurl()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
}

json CCurl::GetResponse(std::string country)
{
    ReadBuffer.clear();
    std::string url = defaultLink + country;

    // VERWIJDER LATER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ReadBuffer);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK)
    {
        std::cout << "curl perform failed: " << curl_easy_strerror(res);
    }
    
    long responseCode;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);

    if(responseCode == 404)
        return json::object();


    return json::parse(ReadBuffer);
}

std::string CCurl::SimpleInfo(std::string country)
{
    json fullJson = GetResponse(country);
    std::string full = "";

    if(fullJson.empty())
    {
        return "404 error: country not found";
    }

    for (const json& j : fullJson)
    {
        full += "Name: " + j["name"]["common"].get<std::string>();

        if (j.contains("capital") && !j["capital"].empty())
            full += "\nCapital: " + j["capital"][0].get<std::string>();

        if (j["name"].contains("nativeName"))
            full += "\nOfficial name: " + j["name"]["nativeName"].begin().value()["official"].get<std::string>();

        if (j.contains("currencies"))
        {
            auto currencyData = j["currencies"].begin().value();
            full += "\nCurrency: " + currencyData["symbol"].get<std::string>() + " " + currencyData["name"].get<std::string>();
        }

        if (j.contains("population"))
            full += "\nPopulation: " + std::to_string(j["population"].get<int>());

        full += "\n";
    }

    return full;
}

std::string CCurl::DetailedInfo(std::string country)
{
    std::string full = SimpleInfo(country);
    json fullJson = GetResponse(country);

    if(fullJson.empty())
    {
        return full;
    }

    for (const json& j : fullJson)
    {
        if (j.contains("region"))
            full += "\nRegion: " + j["region"].get<std::string>();

        if (j.contains("timezones"))
        {
            std::string entry = "\nTimezones: \n";
            for (const auto& tz : j["timezones"])
            {
                entry += "- " + tz.get<std::string>() + "\n";
            }
            full += entry;
        }
    }
    return full;
}