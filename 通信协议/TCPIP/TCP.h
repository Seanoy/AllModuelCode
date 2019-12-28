#ifndef __TCP_H
#define	__TCP_H

/******* 	TCP 	*******/
//TCP�ײ�����
#define TCP_HEADER_LEN_PLAIN 	20
//Դ�˿�λ��
#define TCP_SRC_PORT_H_P 0x22	//	
#define TCP_SRC_PORT_L_P 		0x23
//Ŀ��˿�λ��
#define TCP_DST_PORT_H_P 		0x24	//0x2425
#define TCP_DST_PORT_L_P 		0x25

// the tcp seq number is 4-bytes 0x26-0x29
//32λ���к�
#define TCP_SEQ_H_P          	0x26	//���к���ʼ8λ
// the tcp confirm seq number is 4-bytes ox2a-0x2d
//32λȷ�����к�                    
#define TCP_SEQACK_H_P       	0x2a	//ȷ�����к���ʼ8λ

//4λ�ײ�����
#define TCP_HEADER_LEN_P 		0x2e

//flagsλ�ã������λ����
#define TCP_FLAGS_P             0x2f

// flags: SYN=2 	6����־λ	һ����==��־λ���
#define TCP_FLAGS_FIN_V         0x01
#define TCP_FLAGS_SYN_V         0x02
#define TCP_FLAGS_PUSH_V        0x08
#define TCP_FLAGS_SYNACK_V      0x12
#define TCP_FLAGS_ACK_V         0x10
#define TCP_FLAGS_PSHACK_V  	0x18
//  plain len without the options:

//���ڴ�С�Զ���(0-65535)

//У���λ��
#define TCP_CHECKSUM_H_P 		0x32	//0x3233
#define TCP_CHECKSUM_L_P 		0x33

//ѡ����ʼλ��
#define TCP_OPTIONS_P    		0x36

//����

#endif
