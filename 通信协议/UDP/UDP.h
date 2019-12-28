#ifndef __UDP_H
#define	__UDP_H
 // ******* UDP *******
//UDP首部长度
#define UDP_HEADER_LEN  8

//源端口位置
#define UDP_SRC_PORT_H_P 0x22
#define UDP_SRC_PORT_L_P 0x23

//目标端口位置
#define UDP_DST_PORT_H_P 0x24
#define UDP_DST_PORT_L_P 0x25

//UDP数据长度位置
#define UDP_LEN_H_P       0x26
#define UDP_LEN_L_P       0x27

//UDP校验和位置
#define UDP_CHECKSUM_H_P 0x28
#define UDP_CHECKSUM_L_P 0x29

//UDP数据起始地址
#define UDP_DATA_P 0x2a

#endif
