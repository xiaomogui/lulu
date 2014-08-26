#include "stdafx.h"
#include "LoadPythonFunc.h"

LoadPythonFunc::LoadPythonFunc(void)
{
}

LoadPythonFunc::~LoadPythonFunc(void)
{
}

// ����python�ű��ļ�·��(������.py��׺)��python�ű���ĺ���������ôκ�����C++���ʵ��
PyObject* LoadPythonFunc::loadPythonFunc(char *py_file_name, char *py_func_name)
{
	// ��ʼ��Python
	//��ʹ��Pythonϵͳǰ������ʹ��Py_Initialize����
	//���г�ʼ������������Python���ڽ�ģ�鲢���ϵͳ·
	//����ģ������·���С��������û�з���ֵ�����ϵͳ
	//�Ƿ��ʼ���ɹ���Ҫʹ��Py_IsInitialized��
	Py_Initialize();

	// ����ʼ���Ƿ�ɹ�
	if ( !Py_IsInitialized() ) 
	{
		return NULL;
	}

	PyRun_SimpleString("import sys");
	// ��ָ��·����ӵ���ǰpython��ģ��ɨ���·����
	PyRun_SimpleString("sys.path.append('./python/')");

	// ������Ϊpython�Ľű��ļ�,������.py��׺
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

	// ���ݺ������õ�python����
	pFunc = PyDict_GetItemString(pDict, py_func_name);
	if(!pFunc || !PyCallable_Check(pFunc))
	{
		printf("can't find function [%s]", py_func_name);
		getchar();
		return NULL;
	}

	return pFunc;
}

// �ͷ�python��Դ
void LoadPythonFunc::closePython()
{
	
	// �ر�Python
	Py_Finalize();
}
