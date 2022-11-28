//
// Created by Daniel St. John on 9/30/21.
//

#include <iostream>
#include <sstream>
#include <exception>
#include <thread>
#include <functional>
#include <thread>
#include <future>

#include "http/RestClient.h"

using namespace std;

namespace lm {

    /*
    curl https://reqbin.com/echo/get/json
    -H "X-Custom-Header: value"
    -H "Content-Type: application/json"
     */

    /*
     *
     curl https://reqbin.com/echo/get/json
      -H "Accept: application/json"

     * To get JSON with Curl, you need to make an
     * HTTP GET request and provide the
     * Accept: application/json request header.
     * The application/json request header is passed to the server
     * with the curl -H command-line option and tells the server that
     * the client is expecting JSON in response. If you do not provide
     * an Accept request header, the server may respond with a different MIME type than JSON.
     *
     * The server specifies the returned data type with the Content-Type response header.
     * In this Curl GET JSON example, we send an HTTP GET request to download the JSON
     * from the ReqBin echo URL. Click the Run button to execute the Curl GET JSON example online and see the results.
     */

    static string s_data;

    static size_t onDataCallback(char *buf, size_t size, size_t nmemb, void *up) {
        cout << s_data << endl;
        for (int c = 0; c < size * nmemb; c++) {
            s_data.push_back(buf[c]);
        }
        return size * nmemb;
    }

    RestClient::RestClient() {}

    std::string RestClient::get(std::string uri) {


        // When using libcurl's "easy" interface you init your session and get a handle
        // (often referred to as an "easy handle"), which you use as input to the easy
        // interface functions you use. Use curl_easy_init to get the handle.
        CURL *curl = curl_easy_init();
        validate(curl);

        // You continue by setting all the options you want in the upcoming transfer,
        // the most important among them is the URL itself (you can't transfer anything
        // without a specified URL as you may have figured out yourself).
        curl_easy_setopt(curl, CURLOPT_URL, uri.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        // callbacks as well that will be called from the library when data is
        // available etc. curl_easy_setopt is used for all this.
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &onDataCallback);

        std::string data;
        std::mutex m_mutex;
        {
            // do the nature of one static external variable
            // this call will allow on one thread in the critical section
            std::lock_guard<std::mutex> guard(m_mutex);
            // perform the transfer using curl_easy_perform.
            // It will then do the entire operation and won't
            // return until it is done (successfully or not).
            CURLcode res = curl_easy_perform(curl);
            validate(res);
            data = s_data;

            // After the transfer has been made, you can set new options and
            // make another transfer, or if you're done, cleanup the session by
            // calling curl_easy_cleanup. If you want persistent connections,
            // you don't cleanup immediately, but instead run ahead and perform other
            // transfers using the same easy handle.
            curl_easy_cleanup(curl);
            s_data.clear();
        }
        return data;
    }

    void RestClient::getAsync(std::string uri, function<void(string raw)> f) {

        auto wrapper = [this, uri, f]() -> void {
            CURL *curl = curl_easy_init();
            validate(curl);
            curl_easy_setopt(curl, CURLOPT_URL, uri.c_str());
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &onDataCallback);
            std::string data;
            std::mutex m_mutex;
            {
                std::lock_guard<std::mutex> guard(m_mutex);
                CURLcode res = curl_easy_perform(curl);
                validate(res);
                curl_easy_cleanup(curl);
                data = s_data;
                s_data.clear();
                m_mutex.unlock();
            }
            f(data);
        };

        thread t(wrapper);
        t.detach();
    }


    std::string RestClient::post(std::string uri, const char *payload) {


        CURL *curl = curl_easy_init();
        validate(curl);

        curl_easy_setopt(curl, CURLOPT_URL, uri.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long) strlen(payload));
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &onDataCallback);

        std::string data;
        std::mutex m_mutex;
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            CURLcode res = curl_easy_perform(curl);
            validate(res);
            curl_easy_cleanup(curl);
            data = s_data;
            s_data.clear();
        }
        return data;

    }

    void RestClient::postAsync(std::string uri, const char *payload, std::function<void(std::string)> f) {

        auto wrapper = [this, uri,payload, f]() -> void {
            CURL *curl = curl_easy_init();
            validate(curl);
            curl_easy_setopt(curl, CURLOPT_URL, uri.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long) strlen(payload));
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &onDataCallback);
            std::string data;
            std::mutex m_mutex;
            {
                std::lock_guard<std::mutex> guard(m_mutex);
                CURLcode res = curl_easy_perform(curl);
                validate(res);
                curl_easy_cleanup(curl);
                data = s_data;
                s_data.clear();
            }
            f(data);
        };
        thread t(wrapper);
        t.detach();
    }


    void RestClient::validate(CURL *curl) {
        if (!curl) {
            throw new std::runtime_error("curl_easy_init() failed");
        }
    }

    void RestClient::validate(CURLcode ret) {
        if (ret != CURLE_OK) {
            std::stringstream ss;
            ss << "curl_easy_perform() failed " << curl_easy_strerror(ret) << std::endl;
            std::cout << ss.str();
            throw new std::runtime_error(ss.str());
        }
    }


}