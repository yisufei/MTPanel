#ifndef _CONFIG_BUSINESS_H
#define _CONFIG_BUSINESS_H

#include "ConfigWithLua.h"

class ConfigBusiness{

public:
    ConfigBusiness();
    ~ConfigBusiness();

public:
    bool getFileName(char* value, int size);
    bool getMethod(char* value, int size);

public:
    bool setFuncid(const char *psFuncid); 
    char* getFuncid();


private:
    char            m_psFuncid[9];
    ConfigWithLua * cfg;
};

#endif
