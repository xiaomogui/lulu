// DuboFlashListener.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "PyConnectAMQ.h"

#pragma comment(lib, "ws2_32")

int listenerNetwork(char* capture_filter)
{
	pcap_if_t * allAdapters;//�������б�
	pcap_if_t * adapter;
	pcap_t		   * adapterHandle;//���������
	struct pcap_pkthdr * packetHeader;
	const u_char	   * packetData;
	char errorBuffer[ PCAP_ERRBUF_SIZE ];//������Ϣ������
	u_int netmask;
	// ���˹���
	// char packet_filter[] = "host 103.242.147.30 and tcp port 443";
	char* packet_filter = capture_filter;
	struct bpf_program fcode;

	if( pcap_findalldevs_ex( PCAP_SRC_IF_STRING, NULL, 
				&allAdapters, errorBuffer ) == -1 )
	{//�����������ӵ���������������
		fprintf( stderr, "Error in pcap_findalldevs_ex function: %s\n", errorBuffer );
		return -1;
	}
	if( allAdapters == NULL )
	{//�������κ�������
		printf( "\nNo adapters found! Make sure WinPcap is installed.\n" );
		return 0;
	}
	int crtAdapter = 0;
	for( adapter = allAdapters; adapter != NULL; adapter = adapter->next)
	{//����������������Ϣ(���ƺ�������Ϣ)
		printf( "\n%d.%s ", ++crtAdapter, adapter->name ); 
		printf( "-- %s\n", adapter->description );
	}
	printf( "\n" );
	//ѡ��Ҫ�������ݰ���������
	int adapterNumber;
	printf( "Enter the adapter number between 1 and %d:", crtAdapter );
	scanf_s( "%d", &adapterNumber );
	if( adapterNumber < 1 || adapterNumber > crtAdapter )
	{
		printf( "\nAdapter number out of range.\n" );
		// �ͷ��������б�
		pcap_freealldevs( allAdapters );
		return -1;
	}
	adapter = allAdapters;
	for( crtAdapter = 0; crtAdapter < adapterNumber - 1; crtAdapter++ )
	adapter = adapter->next;
	// ��ָ��������
	adapterHandle = pcap_open( adapter->name, // name of the adapter
							   65536, // portion of the packet to capture
									 // 65536 guarantees that the whole 
									 // packet will be captured
							   PCAP_OPENFLAG_PROMISCUOUS, // promiscuous mode
							   1000, // read timeout - 1 millisecond
							   NULL, // authentication on the remote machine
							   errorBuffer	// error buffer
							  );
	if( adapterHandle == NULL )
	{//ָ����������ʧ��
		fprintf( stderr, "\nUnable to open the adapter\n", adapter->name );
		// �ͷ��������б�
		pcap_freealldevs( allAdapters );
		return -1;
	}

	if(adapter->addresses != NULL)
		/* ��ýӿڵ�һ����ַ������ */
		netmask=((struct sockaddr_in *)(adapter->addresses->netmask))->sin_addr.S_un.S_addr;
	else
		/* ����ӿ�û�е�ַ����ô���Ǽ���һ��C������� */
		netmask=0xffffff; 

	//���������
	if (pcap_compile(adapterHandle, &fcode, packet_filter, 1, netmask) <0 )
	{
		fprintf(stderr,"\nUnable to compile the packet filter. Check the syntax.\n");
		/* �ͷ��豸�б� */
		pcap_freealldevs(allAdapters);
		return -1;
	}

	//���ù�����
	if (pcap_setfilter(adapterHandle, &fcode)<0)
	{
		fprintf(stderr,"\nError setting the filter.\n");
		/* �ͷ��豸�б� */
		pcap_freealldevs(allAdapters);
		return -1;
	}

	printf( "\nCapture session started on  adapter %s\n", adapter->name );
	pcap_freealldevs( allAdapters );//�ͷ��������б�

	// ��ʼ�������ݰ�
	int retValue;
	while( (retValue = pcap_next_ex(adapterHandle, &packetHeader, &packetData) ) >= 0 )
	{
		// timeout elapsed if we reach this point
		if( retValue == 0 )
			continue;

		ip_header *ih;
		tcp_header *tcp_h;
		u_short sport,dport;
		// packet����
		u_int packet_len = packetHeader->len;
		// ��̫��ͷ������
		u_int ethernet_h_len = 14;
		// IPͷ��Ϣ����
		u_int ip_len;
		// TCPͷ��Ϣ����
		u_int tcp_h_len;
		// ���͸�python���������
		send_python_packet_data *send_pdata = (send_python_packet_data *)malloc(sizeof(send_python_packet_data));

		/* ���IP���ݰ�ͷ����λ�� */
		ih = (ip_header *) (packetData + ethernet_h_len); // +��̫��ͷ������

		/* ���TCP�ײ���λ�� */
		ip_len = (ih->ver_ihl & 0xf) * 4;
		tcp_h = (tcp_header *) ((u_char*)ih + ip_len);

		/* �������ֽ�����ת���������ֽ����� */
		sport = ntohs( tcp_h->sport );
		dport = ntohs( tcp_h->dport );

		// ����Դip�˿ں�Ŀ��ip�˿�
		send_pdata->s_ip = ih->saddr;
		send_pdata->sport = sport;
		send_pdata->d_ip = ih->daddr;
		send_pdata->dport = dport;

		// ���TCPͷ������
		tcp_h_len = ((ntohs(tcp_h->len_other_flag) & 0xf000) >> 12) * 4;
		// TCPЭ�����ݳ���
		u_int tcp_data_len = packet_len - ethernet_h_len - ip_len - tcp_h_len;

		send_pdata->tcp_data_len = tcp_data_len;
		send_pdata->tcp_data_addr = (u_char *)((u_char *)tcp_h + tcp_h_len);

		send_to_python(send_pdata);
	}

	// if we get here, there was an error reading the packets
	if( retValue == -1 )
	{
		printf( "Error reading the packets: %s\n", pcap_geterr( adapterHandle ) );
		return -1;
	}

	system( "PAUSE" );
}

int _tmain(int argc, _TCHAR* argv[])
{
	char* capture_filter = getConfigValue("capture_filter");
	if(strcmp(capture_filter,"-1") == 0)
	{
		printf("can't read [capture_filter] from config file");
		return -1;
	}
	else
	{
		printf("capture_filter value : %s\r\n", capture_filter);
	}

	// PyConnectAMQ::connectInit();
	// PyConnectAMQ::sendData();

	listenerNetwork(capture_filter);
	return 0;
}

