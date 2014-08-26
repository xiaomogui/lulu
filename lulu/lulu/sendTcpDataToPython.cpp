
#include "stdafx.h"

char* send_to_python(send_python_packet_data *packet_data)
{
	u_char *data_addr = packet_data->tcp_data_addr;
	u_int data_len = packet_data->tcp_data_len;

	// 初始化Python
	//在使用Python系统前，必须使用Py_Initialize对其
	//进行初始化。它会载入Python的内建模块并添加系统路
	//径到模块搜索路径中。这个函数没有返回值，检查系统
	//是否初始化成功需要使用Py_IsInitialized。
	LoadPythonFunc* loadPythonFunc = new LoadPythonFunc();
	PyObject* pFunc = loadPythonFunc->loadPythonFunc("processPcapData", "connect_send");

	if(pFunc == NULL)
	{
		return "err";
	}

	if ( !pFunc || !PyCallable_Check(pFunc) )
	{
		printf("can't find function [connect_send]");
		getchar();
		return "err";
	}

	// 参数进栈
	PyObject* pArgs = PyTuple_New(1);
	// PyObject* Py_BuildValue(char *format, ...)
	// 把C++的变量转换成一个Python对象。当需要从
	// C++传递变量到Python时，就会使用这个函数。此函数
	// 有点类似C的printf，但格式不同。常用的格式有
	// s 表示字符串，
	// i 表示整型变量，
	// f 表示浮点数，
	// O 表示一个Python对象。
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("s#",data_addr,data_len));

	// 调用Python函数
	PyObject* pRetVal = PyObject_CallObject(pFunc, pArgs);
	printf("function return value : %ld\r\n", PyInt_AsLong(pRetVal));
	// Py_DECREF(pName);
	Py_DECREF(pArgs);
	// Py_DECREF(pModule);
	// Py_DECREF(pRetVal);

	// 关闭Python
	Py_Finalize();
	return 0;
}