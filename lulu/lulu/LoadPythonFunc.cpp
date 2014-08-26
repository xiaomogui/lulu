#include "stdafx.h"
#include "LoadPythonFunc.h"

LoadPythonFunc::LoadPythonFunc(void)
{
}

LoadPythonFunc::~LoadPythonFunc(void)
{
}

// 根据python脚本文件路径(不包含.py后缀)和python脚本里的函数名，获得次函数在C++里的实例
PyObject* LoadPythonFunc::loadPythonFunc(char *py_file_name, char *py_func_name)
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
		return NULL;
	}

	PyRun_SimpleString("import sys");
	// 将指定路径添加到当前python的模块扫描的路径里
	PyRun_SimpleString("sys.path.append('./python/')");

	// 载入名为python的脚本文件,不包含.py后缀
	pName = PyString_FromString(py_file_name);
	pModule = PyImport_Import(pName);
	if(!pModule)
	{
		printf("can't find %s.py[method: %s]", py_file_name, py_func_name);
		getchar();
		return NULL;
	}

	pDict = PyModule_GetDict(pModule);
	if(!pDict)
	{
		return NULL;
	}

	// 根据函数名得到python方法
	pFunc = PyDict_GetItemString(pDict, py_func_name);
	if(!pFunc || !PyCallable_Check(pFunc))
	{
		printf("can't find function [%s]", py_func_name);
		getchar();
		return NULL;
	}

	return pFunc;
}

// 释放python资源
void LoadPythonFunc::closePython()
{
	
	// 关闭Python
	Py_Finalize();
}
