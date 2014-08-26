// DuboFlashListener.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "PyConnectAMQ.h"

#pragma comment(lib, "ws2_32")

int listenerNetwork(char* capture_filter)
{
	pcap_if_t * allAdapters;//适配器列表
	pcap_if_t * adapter;
	pcap_t		   * adapterHandle;//适配器句柄
	struct pcap_pkthdr * packetHeader;
	const u_char	   * packetData;
	char errorBuffer[ PCAP_ERRBUF_SIZE ];//错误信息缓冲区
	u_int netmask;
	// 过滤规则
	// char packet_filter[] = "host 103.242.147.30 and tcp port 443";
	char* packet_filter = capture_filter;
	struct bpf_program fcode;

	if( pcap_findalldevs_ex( PCAP_SRC_IF_STRING, NULL, 
				&allAdapters, errorBuffer ) == -1 )
	{//检索机器连接的所有网络适配器
		fprintf( stderr, "Error in pcap_findalldevs_ex function: %s\n", errorBuffer );
		return -1;
	}
	if( allAdapters == NULL )
	{//不存在任何适配器
		printf( "\nNo adapters found! Make sure WinPcap is installed.\n" );
		return 0;
	}
	int crtAdapter = 0;
	for( adapter = allAdapters; adapter != NULL; adapter = adapter->next)
	{//遍历输入适配器信息(名称和描述信息)
		printf( "\n%d.%s ", ++crtAdapter, adapter->name ); 
		printf( "-- %s\n", adapter->description );
	}
	printf( "\n" );
	//选择要捕获数据包的适配器
	int adapterNumber;
	printf( "Enter the adapter number between 1 and %d:", crtAdapter );
	scanf_s( "%d", &adapterNumber );
	if( adapterNumber < 1 || adapterNumber > crtAdapter )
	{
		printf( "\nAdapter number out of range.\n" );
		// 释放适配器列表
		pcap_freealldevs( allAdapters );
		return -1;
	}
	adapter = allAdapters;
	for( crtAdapter = 0; crtAdapter < adapterNumber - 1; crtAdapter++ )
	adapter = adapter->next;
	// 打开指定适配器
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
	{//指定适配器打开失败
		fprintf( stderr, "\nUnable to open the adapter\n", adapter->name );
		// 释放适配器列表
		pcap_freealldevs( allAdapters );
		return -1;
	}

	if(adapter->addresses != NULL)
		/* 获得接口第一个地址的掩码 */
		netmask=((struct sockaddr_in *)(adapter->addresses->netmask))->sin_addr.S_un.S_addr;
	else
		/* 如果接口没有地址，那么我们假设一个C类的掩码 */
		netmask=0xffffff; 

	//编译过滤器
	if (pcap_compile(adapterHandle, &fcode, packet_filter, 1, netmask) <0 )
	{
		fprintf(stderr,"\nUnable to compile the packet filter. Check the syntax.\n");
		/* 释放设备列表 */
		pcap_freealldevs(allAdapters);
		return -1;
	}

	//设置过滤器
	if (pcap_setfilter(adapterHandle, &fcode)<0)
	{
		fprintf(stderr,"\nError setting the filter.\n");
		/* 释放设备列表 */
		pcap_freealldevs(allAdapters);
		return -1;
	}

	printf( "\nCapture session started on  adapter %s\n", adapter->name );
	pcap_freealldevs( allAdapters );//释放适配器列表

	// 开始捕获数据包
	int retValue;
	while( (retValue = pcap_next_ex(adapterHandle, &packetHeader, &packetData) ) >= 0 )
	{
		// timeout elapsed if we reach this point
		if( retValue == 0 )
			continue;

		ip_header *ih;
		tcp_header *tcp_h;
		u_short sport,dport;
		// packet长度
		u_int packet_len = packetHeader->len;
		// 以太网头部长度
		u_int ethernet_h_len = 14;
		// IP头信息长度
		u_int ip_len;
		// TCP头信息长度
		u_int tcp_h_len;
		// 发送给python处理的数据
		send_python_packet_data *send_pdata = (send_python_packet_data *)malloc(sizeof(send_python_packet_data));

		/* 获得IP数据包头部的位置 */
		ih = (ip_header *) (packetData + ethernet_h_len); // +以太网头部长度

		/* 获得TCP首部的位置 */
		ip_len = (ih->ver_ihl & 0xf) * 4;
		tcp_h = (tcp_header *) ((u_char*)ih + ip_len);

		/* 将网络字节序列转换成主机字节序列 */
		sport = ntohs( tcp_h->sport );
		dport = ntohs( tcp_h->dport );

		// 设置源ip端口和目的ip端口
		send_pdata->s_ip = ih->saddr;
		send_pdata->sport = sport;
		send_pdata->d_ip = ih->daddr;
		send_pdata->dport = dport;

		// 获得TCP头部长度
		tcp_h_len = ((ntohs(tcp_h->len_other_flag) & 0xf000) >> 12) * 4;
		// TCP协议数据长度
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

