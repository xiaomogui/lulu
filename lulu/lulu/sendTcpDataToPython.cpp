
#include "stdafx.h"

char* send_to_python(send_python_packet_data *packet_data)
{
	u_char *data_addr = packet_data->tcp_data_addr;
	u_int data_len = packet_data->tcp_data_len;

	// ��ʼ��Python
	//��ʹ��Pythonϵͳǰ������ʹ��Py_Initialize����
	//���г�ʼ������������Python���ڽ�ģ�鲢���ϵͳ·
	//����ģ������·���С��������û�з���ֵ�����ϵͳ
	//�Ƿ��ʼ���ɹ���Ҫʹ��Py_IsInitialized��
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

	// ������ջ
	PyObject* pArgs = PyTuple_New(1);
	// PyObject* Py_BuildValue(char *format, ...)
	// ��C++�ı���ת����һ��Python���󡣵���Ҫ��
	// C++���ݱ�����Pythonʱ���ͻ�ʹ������������˺���
	// �е�����C��printf������ʽ��ͬ�����õĸ�ʽ��
	// s ��ʾ�ַ�����
	// i ��ʾ���ͱ�����
	// f ��ʾ��������
	// O ��ʾһ��Python����
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("s#",data_addr,data_len));

	// ����Python����
	PyObject* pRetVal = PyObject_CallObject(pFunc, pArgs);
	printf("function return value : %ld\r\n", PyInt_AsLong(pRetVal));
	// Py_DECREF(pName);
	Py_DECREF(pArgs);
	// Py_DECREF(pModule);
	// Py_DECREF(pRetVal);

	// �ر�Python
	Py_Finalize();
	return 0;
}