/*
 * Props.cpp
 *
 *  Created on: Jun 12, 2020
 *      Author: local-admin
 */

#include "Props.h"

namespace lm {
    namespace utils {

        Props::Props() {
            // TODO Auto-generated constructor stub
        }

        Props::~Props() {
            // TODO Auto-generated destructor stub
        }

        std::string Props::getEnvironmentVarAsString(const std::string name) {
            std::string value;
            try {
                std::string value = std::getenv(name.c_str());
            } catch (...) {
                value = nullptr;
            }
            return value;
        }

        int Props::getEnvironmentVarAsInt(const std::string name) {
            int value = -1;
            try {
                std::string svalue = std::getenv(name.c_str());
                std::istringstream(svalue) >> value;
            } catch (...) {

            }
            return value;
        }

    }
}
