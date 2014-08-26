
#pragma once

/* 4字节的IP地址 */
typedef struct ip_address{
    u_char byte1;
    u_char byte2;
    u_char byte3;
    u_char byte4;
}ip_address;

/* IPv4 首部 */
typedef struct ip_header{
    u_char  ver_ihl;        // 版本 (4 bits) + 首部长度 (4 bits)
    u_char  tos;            // 服务类型(Type of service) 
    u_short tlen;           // 总长(Total length) 
    u_short identification; // 标识(Identification)
    u_short flags_fo;       // 标志位(Flags) (3 bits) + 段偏移量(Fragment offset) (13 bits)
    u_char  ttl;            // 存活时间(Time to live)
    u_char  proto;          // 协议(Protocol)
    u_short crc;            // 首部校验和(Header checksum)
    ip_address  saddr;      // 源地址(Source address)
    ip_address  daddr;      // 目的地址(Destination address)
    u_int   op_pad;         // 选项与填充(Option + Padding)
}ip_header;

/* UDP 首部*/
typedef struct udp_header{
    u_short sport;          // 源端口(Source port)
    u_short dport;          // 目的端口(Destination port)
    u_short len;            // UDP数据包长度(Datagram length)
    u_short crc;            // 校验和(Checksum)
}udp_header;

/* TCP 首部*/
typedef struct tcp_header{
    u_short sport;          // 源端口(Source port) 16bit
    u_short dport;          // 目的端口(Destination port) 16bit
	u_int seq;// 序号(seq) 32bit
	u_int ack;// 确认序号(ack) 32bit
	u_short len_other_flag;// 16bit - (4bit:首部长度,6bit:保留,6bit:标志位)
	u_short win_size;// 窗口大小(16bit)
	u_short checksum;// 检验和(16bit)
	u_short crash;// 紧急指针(16bit)
}tcp_header;

/* 调用python方法时传送封包的数据结构*/
typedef struct send_python_packet_data{
	u_int sport;//源端口
	u_int dport;//目的端口
	ip_address s_ip;//源ip
	ip_address d_ip;//目的ip
	u_char *tcp_data_addr;//数据地址
	u_int tcp_data_len;//数据长度
}send_python_packet_data;
