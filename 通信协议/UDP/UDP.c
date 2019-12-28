#include "UDP.h"

void make_udp_reply_from_request(unsigned char *buf, char *data, unsigned int datalen, unsigned  int port)
 {
     unsigned int i = 0, tol_len;
     unsigned  int ck;
     //如前面的ARP和ICMP一样的
    make_eth(buf);
     // total length field in the IP header must be set:
     //如IP Header
     tol_len = IP_HEADER_LEN + UDP_HEADER_LEN + datalen;
     buf[IP_TOTLEN_H_P] = tol_len >> 8;
     buf[IP_TOTLEN_L_P] = tol_len;
     //如ICMP
     make_ip(buf);
     //本地UDP的端口号
    buf[UDP_DST_PORT_H_P] = port >> 8;
     buf[UDP_DST_PORT_L_P] = port & 0xff;
     // source port does not matter and is what the sender used.
     // calculte the udp length:最大16bit长度，即65535-14-20-8，但一般会设置的较小，原因么，上文里面讲过。
    buf[UDP_LEN_H_P] = datalen >> 8;
     buf[UDP_LEN_L_P] = UDP_HEADER_LEN + datalen;
     // zero the checksum
     buf[UDP_CHECKSUM_H_P] = 0;
     buf[UDP_CHECKSUM_L_P] = 0;

    // copy the data:
     while(i < datalen)
     {
         buf[UDP_DATA_P + i] = data[i];
         i++;
     }

#ifdef UDP_DEBUG
     i = 0;
     printf("UDP Server Test. \r\n");
     printf("udp客户端的IP地址及端口号 ： \r\n");

    while(i < sizeof(ipv4_addr))
     {
         //注意这里我们建立的是UDP Server，输出UDP Client的IP地址
        printf("%d", buf[IP_DST_P + i]);

        if(i != sizeof(ipv4_addr) - 1)
         {
             printf(".");
         }

        i++;
     }

    i = 0;
     //输出pc端的udp port
     printf(":%d \r\n", port);

    //串口打印UDP Client发过来的数据
    printf("udp客户端发送的数据 ： \r\n");
     printf("%s \r\n", data);

    //实现UDP Server来响应UDP Client的控制LED命令
    //如：led1=on,led1=off
     if(strcmp(data, "led1=on") == 0)
     {
         GPIO_ResetBits(GPIOA, GPIO_Pin_8);
     }

    if(strcmp(data, "led1=off") == 0)
     {
         GPIO_SetBits(GPIOA, GPIO_Pin_8);
     }

    //如：led2=on,led2=off
     if(strcmp(data, "led2=on") == 0)
     {
         GPIO_ResetBits(GPIOD, GPIO_Pin_2);
     }

    if(strcmp(data, "led2=off") == 0)
     {
         GPIO_SetBits(GPIOD, GPIO_Pin_2);
     }

#endif
     //这里的16字节是UDP的伪首部，即IP的源地址-0x1a+目标地址-0x1e（和标准的有差异），
    //+UDP首部=4+4+8=16
     ck = checksum(&buf[IP_SRC_P], 16 + datalen, 1);
     buf[UDP_CHECKSUM_H_P] = ck >> 8;
     buf[UDP_CHECKSUM_L_P] = ck & 0xff;
     enc28j60PacketSend(UDP_HEADER_LEN + IP_HEADER_LEN + ETH_HEADER_LEN + datalen, buf);
 }


 void test(){
	 /*--------------------- udp server start, we listen on udp port 1200=0x4B0 -----------------------------*/
       if (buf[IP_PROTO_P]==IP_PROTO_UDP_V&&buf[UDP_DST_PORT_H_P]==4&&buf[UDP_DST_PORT_L_P]==0xb0)
       {
         //UDP数据长度
          udpdatalen=buf[UDP_LEN_H_P];
           udpdatalen=udpdatalen<<8;
           udpdatalen=(udpdatalen+buf[UDP_LEN_L_P])-UDP_HEADER_LEN;
           //udpdatalen=buf[UDP_LEN_L_P]-UDP_HEADER_LEN;
            //获取pc端的udp port
           pcudpport=buf[UDP_SRC_PORT_H_P]<<8 | buf[UDP_SRC_PORT_L_P];
         //将udp客户端得到的数据buf写入buf1，因为下面的实验需要输入的信息来做出相应的动作
          for(i1=0; i1<udpdatalen; i1++)
                         buf1[i1]=buf[UDP_DATA_P+i1];
                
           make_udp_reply_from_request(buf,buf1,udpdatalen,pcudpport);         
       }
 /*----------------------------------------udp end -----------------------------------------------*/
 }
 
 
 