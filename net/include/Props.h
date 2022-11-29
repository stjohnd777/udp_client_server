
#pragma once

#include <string>
#include <sstream>

namespace lm {

    namespace utils {

        class Props {

        public:
            static std::string getEnvironmentVarAsString(const std::string name);

            static int getEnvironmentVarAsInt(const std::string name);

        public:
            Props();

            Props(std::string propsFile) {

            };

            virtual ~Props();
        };
    }
}

