// 读取配置文件

#include "stdafx.h"

char* getConfigValue(char *key)
{
	LoadPythonFunc* loadPythonFunc = new LoadPythonFunc();
	PyObject* pFunc = loadPythonFunc->loadPythonFunc("parseIniFile", "getValue");

	if(pFunc == NULL)
	{
		return "-1";
	}

	// 定义python用到的参数和返回值的变量
	PyObject *pArgs, *pRetVal;

	// 参数进栈
	pArgs = PyTuple_New(3);
	// PyObject* Py_BuildValue(char *format, ...)
	// 把C++的变量转换成一个Python对象。当需要从
	// C++传递变量到Python时，就会使用这个函数。此函数
	// 有点类似C的printf，但格式不同。常用的格式有
	// s 表示字符串，
	// i 表示整型变量，
	// f 表示浮点数，
	// O 表示一个Python对象。
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", config_fiel_path));
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("s", "filter"));
	PyTuple_SetItem(pArgs, 2, Py_BuildValue("s", key));

	// 调用Python函数
	pRetVal = PyObject_CallObject(pFunc, pArgs);
	char* retValue = PyString_AsString(pRetVal);
	//Py_DECREF(pName);
	Py_DECREF(pArgs);
	//Py_DECREF(pModule);
	//Py_DECREF(pRetVal);

	loadPythonFunc->closePython();
	return retValue;
}


// 无用代码，用于以后学习参考 TODO
// 定义python使用的变量
PyObject *pName, *pModule, *pDict, *pFunc;

// 无用代码，用于以后学习参考 TODO
int readIniConf()
{
	FILE *fp;
	if(fp=fopen(config_fiel_path,"r"))
	{
		puts("打开文件成功");
	}
	else
	{
		puts("打开文件成败");
		return -1;
	}

	char key,value;
	while(feof(fp) == 0){
		fscanf(fp,"%s=%s" ,&key, &value);
		printf("KEY:%s-->VALUE:%s", &key, &value);
	}

	fclose(fp);
	return 0;
}

// 无用代码，用于以后学习参考 TODO
int loadPythonFunc()
{
	// 初始化Python
	//在使用Python系统前，必须使用Py_Initialize对其
	//进行初始化。它会载入Python的内建模块并添加系统路
	//径到模块搜索路径中。这个函数没有返回值，检查系统
	//是否初始化成功需要使用Py_IsInitialized。
	Py_Initialize();

	// 检查初始化是否成功
	if ( !Py_IsInitialized() ) 
	{
		return -1;
	}

	// 载入名为python的脚本文件,不包含.py后缀
	pName = PyString_FromString("parseIniFile");
	pModule = PyImport_Import(pName);
	if(!pModule)
	{
		printf("can't find parseIniFile.py");
		getchar();
		return -1;
	}

	pDict = PyModule_GetDict(pModule);
	if(!pDict)
	{
		return -1;
	}

	// 根据函数名得到python方法
	pFunc = PyDict_GetItemString(pDict, "getValue");
	if(!pFunc || !PyCallable_Check(pFunc))
	{
		printf("can't find function [getValue]");
		getchar();
		return -1;
	}

	return 0;
}

// 无用代码，用于以后学习参考 TODO
int closePython()
{
	
	// 关闭Python
	Py_Finalize();
	return 0;
}