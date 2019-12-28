#include "UDP.h"

void make_udp_reply_from_request(unsigned char *buf, char *data, unsigned int datalen, unsigned  int port)
 {
     unsigned int i = 0, tol_len;
     unsigned  int ck;
     //��ǰ���ARP��ICMPһ����
    make_eth(buf);
     // total length field in the IP header must be set:
     //��IP Header
     tol_len = IP_HEADER_LEN + UDP_HEADER_LEN + datalen;
     buf[IP_TOTLEN_H_P] = tol_len >> 8;
     buf[IP_TOTLEN_L_P] = tol_len;
     //��ICMP
     make_ip(buf);
     //����UDP�Ķ˿ں�
    buf[UDP_DST_PORT_H_P] = port >> 8;
     buf[UDP_DST_PORT_L_P] = port & 0xff;
     // source port does not matter and is what the sender used.
     // calculte the udp length:���16bit���ȣ���65535-14-20-8����һ������õĽ�С��ԭ��ô���������潲����
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
     printf("udp�ͻ��˵�IP��ַ���˿ں� �� \r\n");

    while(i < sizeof(ipv4_addr))
     {
         //ע���������ǽ�������UDP Server�����UDP Client��IP��ַ
        printf("%d", buf[IP_DST_P + i]);

        if(i != sizeof(ipv4_addr) - 1)
         {
             printf(".");
         }

        i++;
     }

    i = 0;
     //���pc�˵�udp port
     printf(":%d \r\n", port);

    //���ڴ�ӡUDP Client������������
    printf("udp�ͻ��˷��͵����� �� \r\n");
     printf("%s \r\n", data);

    //ʵ��UDP Server����ӦUDP Client�Ŀ���LED����
    //�磺led1=on,led1=off
     if(strcmp(data, "led1=on") == 0)
     {
         GPIO_ResetBits(GPIOA, GPIO_Pin_8);
     }

    if(strcmp(data, "led1=off") == 0)
     {
         GPIO_SetBits(GPIOA, GPIO_Pin_8);
     }

    //�磺led2=on,led2=off
     if(strcmp(data, "led2=on") == 0)
     {
         GPIO_ResetBits(GPIOD, GPIO_Pin_2);
     }

    if(strcmp(data, "led2=off") == 0)
     {
         GPIO_SetBits(GPIOD, GPIO_Pin_2);
     }

#endif
     //�����16�ֽ���UDP��α�ײ�����IP��Դ��ַ-0x1a+Ŀ���ַ-0x1e���ͱ�׼���в��죩��
    //+UDP�ײ�=4+4+8=16
     ck = checksum(&buf[IP_SRC_P], 16 + datalen, 1);
     buf[UDP_CHECKSUM_H_P] = ck >> 8;
     buf[UDP_CHECKSUM_L_P] = ck & 0xff;
     enc28j60PacketSend(UDP_HEADER_LEN + IP_HEADER_LEN + ETH_HEADER_LEN + datalen, buf);
 }


 void test(){
	 /*--------------------- udp server start, we listen on udp port 1200=0x4B0 -----------------------------*/
       if (buf[IP_PROTO_P]==IP_PROTO_UDP_V&&buf[UDP_DST_PORT_H_P]==4&&buf[UDP_DST_PORT_L_P]==0xb0)
       {
         //UDP���ݳ���
          udpdatalen=buf[UDP_LEN_H_P];
           udpdatalen=udpdatalen<<8;
           udpdatalen=(udpdatalen+buf[UDP_LEN_L_P])-UDP_HEADER_LEN;
           //udpdatalen=buf[UDP_LEN_L_P]-UDP_HEADER_LEN;
            //��ȡpc�˵�udp port
           pcudpport=buf[UDP_SRC_PORT_H_P]<<8 | buf[UDP_SRC_PORT_L_P];
         //��udp�ͻ��˵õ�������bufд��buf1����Ϊ�����ʵ����Ҫ�������Ϣ��������Ӧ�Ķ���
          for(i1=0; i1<udpdatalen; i1++)
                         buf1[i1]=buf[UDP_DATA_P+i1];
                
           make_udp_reply_from_request(buf,buf1,udpdatalen,pcudpport);         
       }
 /*----------------------------------------udp end -----------------------------------------------*/
 }
 
 
 