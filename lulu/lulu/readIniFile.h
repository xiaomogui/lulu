#pragma once

#include "LoadPythonFunc.h"

#define config_fiel_path "./config.ini"
// const char config_fiel_path[] = "./config.ini";

int readIniConf();

char* getConfigValue(char *key);

int loadPythonFunc();

int closePython();