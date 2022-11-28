#include "RandomString.h"

#include <string>
#include <cstdlib>


namespace lm {

    namespace utils {

        static const std::string base10 = "0123456789";

        static const std::string dictonary = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

        static const std::string dictonary2 = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()?.,><:;~|}]{[";

        std::string random_string(std::string::size_type length) {
            auto dictonary_length = dictonary.length() - 1;
            std::string random_string;
            random_string.reserve(length);
            for (unsigned int i = 0; i < length; i++) {
                int aRandomInt = rand();
                unsigned int random_index = aRandomInt % dictonary_length;
                random_string += dictonary.at(random_index);
            }
            return random_string;
        }


        std::string random_int(std::string::size_type length) {
            auto dictonary_length = base10.length() - 1;
            std::string random_string;
            random_string.reserve(length);
            for (unsigned int i = 0; i < length; i++) {
                int aRandomInt = rand();
                unsigned int random_index = aRandomInt % dictonary_length;
                random_string += dictonary.at(random_index);
            }
            return random_string;
        }

        std::string random_string_ext(std::string::size_type length) {
            auto dictonary_length = dictonary2.length() - 1;
            std::string random_string;
            random_string.reserve(length);
            for (unsigned int i = 0; i < length; i++) {
                int aRandomInt = rand();
                unsigned int random_index = aRandomInt % dictonary_length;
                random_string += dictonary2.at(random_index);
            }
            return random_string;
        }

    }
}

