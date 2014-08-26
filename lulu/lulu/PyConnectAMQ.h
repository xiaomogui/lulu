#pragma once

#include "stdafx.h"
#include "LoadPythonFunc.h"

// 无用，备用
class PyConnectAMQ
{
public:
	PyConnectAMQ(void);
	static char* connectInit()
	{
		LoadPythonFunc* loadPythonFunc = new LoadPythonFunc();
		PyObject* pFunc = loadPythonFunc->loadPythonFunc("recv_tcpdata_from_c", "connect_amq");

		if(pFunc == NULL)
		{
			return "err";
		}

		// 参数进栈
		PyObject* pArgs = PyTuple_New(0);

		// 调用Python函数
		PyObject_CallObject(pFunc, pArgs);

		loadPythonFunc->closePython();
		return "success";
	}
	static char* sendData()
	{
		LoadPythonFunc* loadPythonFunc = new LoadPythonFunc();
		PyObject* pFunc = loadPythonFunc->loadPythonFunc("recv_tcpdata_from_c", "recv_from_c");

		if(pFunc == NULL)
		{
			return "err";
		}

		// 参数进栈
		PyObject* pArgs = PyTuple_New(1);

		PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", "welcome!"));

		// 调用Python函数
		PyObject* pRetVal = PyObject_CallObject(pFunc, pArgs);
		printf("function return value : %ld\r\n", PyInt_AsLong(pRetVal));
		// Py_DECREF(pName);
		Py_DECREF(pArgs);
		// Py_DECREF(pModule);
		// Py_DECREF(pRetVal);

		loadPythonFunc->closePython();
		return "success";
	}
public:
	~PyConnectAMQ(void);
};
