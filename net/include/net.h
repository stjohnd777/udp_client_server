//
// Created by Daniel St. John on 11/16/22.
//

#pragma once


/**
 * Creates get<funName>/set<funName> for instance varable of
 * type <varType> and name <varName> with initial value v
 *
 */
#define ATTR(varType, varName, funName, v)\
    protected: varType varName = v ;\
    public: virtual varType get##funName(void) const {\
        return varName;\
    }\
    public: virtual void set##funName(varType var){\
        varName = var;\
    }\

#define ATT(varType, varName, funName)\
    protected: varType varName ;\
    public: virtual varType get##funName(void) const {\
        return varName;\
    }\
    public: virtual void set##funName(varType var){\
        varName = var;\
    }

#include "utils.h"
#include "udp/udp_utils.h"
 

typedef struct example {
    uint8_t _u8;
    uint16_t _u16;
    uint32_t _u32;
    char matrix[100][200];
} Test;