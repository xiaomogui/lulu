// ��ȡ�����ļ�

#include "stdafx.h"

char* getConfigValue(char *key)
{
	LoadPythonFunc* loadPythonFunc = new LoadPythonFunc();
	PyObject* pFunc = loadPythonFunc->loadPythonFunc("parseIniFile", "getValue");

	if(pFunc == NULL)
	{
		return "-1";
	}

	// ����python�õ��Ĳ����ͷ���ֵ�ı���
	PyObject *pArgs, *pRetVal;

	// ������ջ
	pArgs = PyTuple_New(3);
	// PyObject* Py_BuildValue(char *format, ...)
	// ��C++�ı���ת����һ��Python���󡣵���Ҫ��
	// C++���ݱ�����Pythonʱ���ͻ�ʹ������������˺���
	// �е�����C��printf������ʽ��ͬ�����õĸ�ʽ��
	// s ��ʾ�ַ�����
	// i ��ʾ���ͱ�����
	// f ��ʾ��������
	// O ��ʾһ��Python����
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", config_fiel_path));
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("s", "filter"));
	PyTuple_SetItem(pArgs, 2, Py_BuildValue("s", key));

	// ����Python����
	pRetVal = PyObject_CallObject(pFunc, pArgs);
	char* retValue = PyString_AsString(pRetVal);
	//Py_DECREF(pName);
	Py_DECREF(pArgs);
	//Py_DECREF(pModule);
	//Py_DECREF(pRetVal);

	loadPythonFunc->closePython();
	return retValue;
}


// ���ô��룬�����Ժ�ѧϰ�ο� TODO
// ����pythonʹ�õı���
PyObject *pName, *pModule, *pDict, *pFunc;

// ���ô��룬�����Ժ�ѧϰ�ο� TODO
int readIniConf()
{
	FILE *fp;
	if(fp=fopen(config_fiel_path,"r"))
	{
		puts("���ļ��ɹ�");
	}
	else
	{
		puts("���ļ��ɰ�");
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

// ���ô��룬�����Ժ�ѧϰ�ο� TODO
int loadPythonFunc()
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
		return -1;
	}

	// ������Ϊpython�Ľű��ļ�,������.py��׺
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

	// ���ݺ������õ�python����
	pFunc = PyDict_GetItemString(pDict, "getValue");
	if(!pFunc || !PyCallable_Check(pFunc))
	{
		printf("can't find function [getValue]");
		getchar();
		return -1;
	}

	return 0;
}

// ���ô��룬�����Ժ�ѧϰ�ο� TODO
int closePython()
{
	
	// �ر�Python
	Py_Finalize();
	return 0;
}