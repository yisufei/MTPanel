#include "ConfigBusiness.h"
#include <stdio.h>
#include <string.h>

ConfigBusiness::ConfigBusiness(){
    cfg = new ConfigWithLua();
    memset(m_psFuncid, 0x00, sizeof(m_psFuncid));
}

ConfigBusiness::~ConfigBusiness(){
    if (cfg != NULL){
        delete cfg;
        cfg = NULL;
    }
}

bool ConfigBusiness::getFileName(char* filename, int size){
    memset(filename, 0x00, size);
    if (!cfg->getValue(m_psFuncid, "file", filename, size)) return false;
    printf("m_psFuncid:[%s], filename:[%s], size:[%d]\n", m_psFuncid, filename, size);
    return true;
}

bool ConfigBusiness::getMethod(char* method, int size){
    memset(method, 0x00, size);
    if (!cfg->getValue(m_psFuncid, "method", method, size)) return false;
    return true;
}

bool ConfigBusiness::setFuncid(const char *psFuncid) { 
    strncpy(m_psFuncid, psFuncid, sizeof(m_psFuncid)); 
    return cfg->init("./conf/business.lua");
}

char* ConfigBusiness::getFuncid() { return m_psFuncid; }
