#ifndef __TCP_H
#define	__TCP_H

/******* 	TCP 	*******/
//TCP首部长度
#define TCP_HEADER_LEN_PLAIN 	20
//源端口位置
#define TCP_SRC_PORT_H_P 0x22	//	
#define TCP_SRC_PORT_L_P 		0x23
//目标端口位置
#define TCP_DST_PORT_H_P 		0x24	//0x2425
#define TCP_DST_PORT_L_P 		0x25

// the tcp seq number is 4-bytes 0x26-0x29
//32位序列号
#define TCP_SEQ_H_P          	0x26	//序列号起始8位
// the tcp confirm seq number is 4-bytes ox2a-0x2d
//32位确认序列号                    
#define TCP_SEQACK_H_P       	0x2a	//确认序列号起始8位

//4位首部长度
#define TCP_HEADER_LEN_P 		0x2e

//flags位置，最高两位保留
#define TCP_FLAGS_P             0x2f

// flags: SYN=2 	6个标志位	一起发送==标志位相加
#define TCP_FLAGS_FIN_V         0x01
#define TCP_FLAGS_SYN_V         0x02
#define TCP_FLAGS_PUSH_V        0x08
#define TCP_FLAGS_SYNACK_V      0x12
#define TCP_FLAGS_ACK_V         0x10
#define TCP_FLAGS_PSHACK_V  	0x18
//  plain len without the options:

//窗口大小自定义(0-65535)

//校验和位置
#define TCP_CHECKSUM_H_P 		0x32	//0x3233
#define TCP_CHECKSUM_L_P 		0x33

//选项起始位置
#define TCP_OPTIONS_P    		0x36

//数据

#endif
