#pragma once

class LoadPythonFunc
{
public:
	LoadPythonFunc(void);
	PyObject* loadPythonFunc(char *py_file_name, char *py_func_name);
	void closePython();
public:
	~LoadPythonFunc(void);
private:
	// ����pythonʹ�õı���
	PyObject *pName, *pModule, *pDict, *pFunc;
};
