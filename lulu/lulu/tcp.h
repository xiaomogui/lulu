
#pragma once

/* 4�ֽڵ�IP��ַ */
typedef struct ip_address{
    u_char byte1;
    u_char byte2;
    u_char byte3;
    u_char byte4;
}ip_address;

/* IPv4 �ײ� */
typedef struct ip_header{
    u_char  ver_ihl;        // �汾 (4 bits) + �ײ����� (4 bits)
    u_char  tos;            // ��������(Type of service) 
    u_short tlen;           // �ܳ�(Total length) 
    u_short identification; // ��ʶ(Identification)
    u_short flags_fo;       // ��־λ(Flags) (3 bits) + ��ƫ����(Fragment offset) (13 bits)
    u_char  ttl;            // ���ʱ��(Time to live)
    u_char  proto;          // Э��(Protocol)
    u_short crc;            // �ײ�У���(Header checksum)
    ip_address  saddr;      // Դ��ַ(Source address)
    ip_address  daddr;      // Ŀ�ĵ�ַ(Destination address)
    u_int   op_pad;         // ѡ�������(Option + Padding)
}ip_header;

/* UDP �ײ�*/
typedef struct udp_header{
    u_short sport;          // Դ�˿�(Source port)
    u_short dport;          // Ŀ�Ķ˿�(Destination port)
    u_short len;            // UDP���ݰ�����(Datagram length)
    u_short crc;            // У���(Checksum)
}udp_header;

/* TCP �ײ�*/
typedef struct tcp_header{
    u_short sport;          // Դ�˿�(Source port) 16bit
    u_short dport;          // Ŀ�Ķ˿�(Destination port) 16bit
	u_int seq;// ���(seq) 32bit
	u_int ack;// ȷ�����(ack) 32bit
	u_short len_other_flag;// 16bit - (4bit:�ײ�����,6bit:����,6bit:��־λ)
	u_short win_size;// ���ڴ�С(16bit)
	u_short checksum;// �����(16bit)
	u_short crash;// ����ָ��(16bit)
}tcp_header;

/* ����python����ʱ���ͷ�������ݽṹ*/
typedef struct send_python_packet_data{
	u_int sport;//Դ�˿�
	u_int dport;//Ŀ�Ķ˿�
	ip_address s_ip;//Դip
	ip_address d_ip;//Ŀ��ip
	u_char *tcp_data_addr;//���ݵ�ַ
	u_int tcp_data_len;//���ݳ���
}send_python_packet_data;
