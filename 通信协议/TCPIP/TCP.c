// make a return tcp header from a received tcp packet
// rel_ack_num is how much we must step the seq number received from the
// other side. We do not send more than 255 bytes of text (=data) in the tcp packet.
// If mss=1 then mss is included in the options list
//
// After calling this function you can fill in the first data byte at TCP_OPTIONS_P+4
// If cp_seq=0 then an initial sequence number is used (should be use in synack)
// otherwise it is copied from the packet we received
// TCP首部的填充，与UDP类似，但是TCP加入了握手和MSS可选项的设置
#include "TCP.h"
void make_tcphead(unsigned char * buf,unsigned  int rel_ack_num,unsigned char  mss,unsigned char  cp_seq)
{
    unsigned char  i=0;
    unsigned char  tseq;
    while(i<2)
    {
        buf[TCP_DST_PORT_H_P+i]=buf[TCP_SRC_PORT_H_P+i];
        buf[TCP_SRC_PORT_H_P+i]=0; // clear source port
        i++;
    }
    // set source port  (http):
    buf[TCP_SRC_PORT_L_P]=wwwport;
        //序列号和确认序列号的长度为32位
    i=4;
    // sequence numbers: add the rel_ack_num to SEQACK
        //将序列号的值+rel_ack_num之后返回，来完成握手过程
    while(i>0)
    {
        rel_ack_num=buf[TCP_SEQ_H_P+i-1]+rel_ack_num;
        tseq=buf[TCP_SEQACK_H_P+i-1];
        buf[TCP_SEQACK_H_P+i-1]=0xff&rel_ack_num;
        if (cp_seq)
        {
            // copy the acknum sent to us into the sequence number
            buf[TCP_SEQ_H_P+i-1]=tseq;
        }
        else
        {
            buf[TCP_SEQ_H_P+i-1]= 0; // some preset vallue
        }
        rel_ack_num=rel_ack_num>>8;
        i--;
    }
    if (cp_seq==0)
    {
        // put inital seq number
        buf[TCP_SEQ_H_P+0]= 0;
        buf[TCP_SEQ_H_P+1]= 0;
        // we step only the second byte, this allows us to send packts 
        // with 255 bytes or 512 (if we step the initial seqnum by 2)
        buf[TCP_SEQ_H_P+2]= seqnum; 
        buf[TCP_SEQ_H_P+3]= 0;
        // step the inititial seq num by something we will not use
        // during this tcp session:
        seqnum+=2;
    }
    // zero the checksum
    buf[TCP_CHECKSUM_H_P]=0;
    buf[TCP_CHECKSUM_L_P]=0;
    
    // The tcp header length is only a 4 bit field (the upper 4 bits).
    // It is calculated in units of 4 bytes. 
    // E.g 24 bytes: 24/4=6 => 0x60=header len field
    //buf[TCP_HEADER_LEN_P]=(((TCP_HEADER_LEN_PLAIN+4)/4)) <<4; // 0x60
    //TCP可选项里面的MSS (Maximum Segment Size) 
    if (mss)
    {
        // the only option we set is MSS to 1460:
        // 1460 in hex is 0x5B4
        buf[TCP_OPTIONS_P]=2;
        buf[TCP_OPTIONS_P+1]=4;
        buf[TCP_OPTIONS_P+2]=0x05; 
        buf[TCP_OPTIONS_P+3]=0xb4;
        // 24 bytes:
        buf[TCP_HEADER_LEN_P]=0x60;
    }
    else
    {
        // no options:
        // 20 bytes:
        buf[TCP_HEADER_LEN_P]=0x50;
    }
}


//make_tcp_synack_from_syn，与make_udp_reply_from_request过程类似
void make_tcp_synack_from_syn(unsigned char *buf)
{
    unsigned  int ck, i = 0;
    make_eth(buf);
    // total length field in the IP header must be set:
    // 20 bytes IP + 24 bytes (20tcp+4tcp options)
    buf[IP_TOTLEN_H_P] = 0;
    buf[IP_TOTLEN_L_P] = IP_HEADER_LEN + TCP_HEADER_LEN_PLAIN + 4;
    make_ip(buf);
    buf[TCP_FLAGS_P] = TCP_FLAGS_SYNACK_V;
    make_tcphead(buf, 1, 1, 0);
#ifdef TCP_DEBUG
    i = 0;
    printf("TCP Server Test. \r\n");
    printf("tcp客户端的IP地址及端口号 ： \r\n");

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
    //输出pc端的tcp port
    printf("：%d \r\n", wwwport);
#endif
    // calculate the checksum, len=8 (start from ip.src) + TCP_HEADER_LEN_PLAIN + 4 (one option: mss)
    ck = checksum(&buf[IP_SRC_P], 8 + TCP_HEADER_LEN_PLAIN + 4, 2);
    buf[TCP_CHECKSUM_H_P] = ck >> 8;
    buf[TCP_CHECKSUM_L_P] = ck & 0xff;
    // add 4 for option mss:
    enc28j60PacketSend(IP_HEADER_LEN + TCP_HEADER_LEN_PLAIN + 4 + ETH_HEADER_LEN, buf);
}

void test(){
//Web_Server函数中的while(1)死循环中的TCP部分，这是个主过程，里面有很多子函数将在下面说明。
/*-----------------tcp port www start, compare only the lower byte-----------------------------------*/
if(buf[IP_PROTO_P] == IP_PROTO_TCP_V && buf[TCP_DST_PORT_H_P] == 0 && buf[TCP_DST_PORT_L_P] == mywwwport)
{
    /*
        以下为 TCP的状态变迁图 的部分实现。
    */
    //若为客户端的SYN请求，则返回SYN+ACK
    if(buf[TCP_FLAGS_P] & TCP_FLAGS_SYN_V)                              	//第一次握手
    {
        make_tcp_synack_from_syn(buf);                                      //第二次握手
        // make_tcp_synack_from_syn does already send the syn,ack
        continue;
    }

    //若为客户端的ACK请求，即完成三次握手，可以传输数据了
    if(buf[TCP_FLAGS_P] & TCP_FLAGS_ACK_V)                              	//第三次握手
    {
        init_len_info(buf); // init some data structures
        // we can possibly have no data, just ack:
        dat_p = get_tcp_data_pointer();

        //无数据，返回ack
        if(dat_p == 0)
        {
            if(buf[TCP_FLAGS_P] & TCP_FLAGS_FIN_V)
            {
                // finack, answer with ack
                make_tcp_ack_from_any(buf);
            }

            // just an ack with no data, wait for next packet
            continue;
        }

        //有数据，好了，下面就是HTTP协议规定的数据了
        if(strncmp("GET ", (char *) & (buf[dat_p]), 4) != 0)
        {
            // head, post and other methods:
            //
            // for possible status codes see:
            // http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
            plen = fill_tcp_data_p(buf, 0, PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>200 OK</h1>"));
            goto SENDTCP;
        }

        //密码部分
        if(strncmp("/ ", (char *) & (buf[dat_p + 4]), 2) == 0)
        {
            plen = fill_tcp_data_p(buf, 0, PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n"));
            plen = fill_tcp_data_p(buf, plen, PSTR("<p>Usage: "));
            plen = fill_tcp_data(buf, plen, baseurl);
            plen = fill_tcp_data_p(buf, plen, PSTR("password</p>"));
            goto SENDTCP;
        }

        cmd = analyse_get_url((char *) & (buf[dat_p + 5]));

        // for possible status codes see:
        // http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
        if(cmd == -1)
        {
            plen = fill_tcp_data_p(buf, 0, PSTR("HTTP/1.0 401 Unauthorized\r\nContent-Type: text/html\r\n\r\n<h1>401 Unauthorized</h1>"));
            goto SENDTCP;
        }

        if(cmd == 0)              // 用户程序
        {
            GPIO_SetBits(GPIOA, GPIO_Pin_8);
            i = 0;                           // 命令 = 1
        }

        if(cmd == 1)                         // 用户程序
        {
            GPIO_ResetBits(GPIOA, GPIO_Pin_8);
            i = 1;                           // 命令 = 0
        }

        // if (cmd==-2) or any other value
        // just display the status:
        plen = print_webpage(buf, (i));
    SENDTCP:
        make_tcp_ack_from_any(buf);       // send ack for http get
        make_tcp_ack_with_data(buf, plen); // send data
        continue;
    }
}
/*-------------------------------------- tcp port www end ---------------------------------------*/
}

//主过程中的各TCP相关的子函数
// do some basic length calculations and store the result in static varibales
void init_len_info(unsigned char *buf)
{
    //IP Packet长度
    info_data_len = (buf[IP_TOTLEN_H_P] << 8) | (buf[IP_TOTLEN_L_P] & 0xff);
    //减去IP首部长度
    info_data_len -= IP_HEADER_LEN;
    //TCP首部长度，因为TCP协议规定了只有四位来表明长度，所需要如下处理，4*6=24
    info_hdr_len = (buf[TCP_HEADER_LEN_P] >> 4) * 4; // generate len in bytes;
    //减去TCP首部长度
    info_data_len -= info_hdr_len;

    if(info_data_len <= 0)
    {
        info_data_len = 0;
    }
}

// get a pointer to the start of tcp data in buf
// Returns 0 if there is no data
// You must call init_len_info once before calling this function
unsigned  int get_tcp_data_pointer(void)
{
    if(info_data_len)
    {
        //在buf中数据开始的位置
        return((unsigned  int)TCP_SRC_PORT_H_P + info_hdr_len);
    }

    else
    {
        return(0);
    }
}

// fill in tcp data at position pos. pos=0 means start of
// tcp data. Returns the position at which the string after
// this string could be filled.
unsigned  int fill_tcp_data_p(unsigned char *buf, unsigned  int pos, const unsigned char *progmem_s)
{
    char c;

    // fill in tcp data at position pos
    //
    // with no options the data starts after the checksum + 2 more bytes (urgent ptr)
    while((c = pgm_read_byte(progmem_s++)))
    {
        buf[TCP_CHECKSUM_L_P + 3 + pos] = c;
        pos++;
    }

    return(pos);
}

// fill in tcp data at position pos. pos=0 means start of
// tcp data. Returns the position at which the string after
// this string could be filled.
unsigned  int fill_tcp_data(unsigned char *buf, unsigned  int pos, const char *s)
{
    // fill in tcp data at position pos
    //
    // with no options the data starts after the checksum + 2 more bytes (urgent ptr)
    while(*s)
    {
        buf[TCP_CHECKSUM_L_P + 3 + pos] = *s;
        pos++;
        s++;
    }

    return(pos);
}

// Make just an ack packet with no tcp data inside
// This will modify the eth/ip/tcp header
void make_tcp_ack_from_any(unsigned char *buf)
{
    unsigned  int j;
    make_eth(buf);
    // fill the header:
    buf[TCP_FLAGS_P] = TCP_FLAGS_ACK_V;

    if(info_data_len == 0)
    {
        // if there is no data then we must still acknoledge one packet
        make_tcphead(buf, 1, 0, 1); // no options
    }

    else
    {
        make_tcphead(buf, info_data_len, 0, 1); // no options
    }

    // total length field in the IP header must be set:
    // 20 bytes IP + 20 bytes tcp (when no options)
    j = IP_HEADER_LEN + TCP_HEADER_LEN_PLAIN;
    buf[IP_TOTLEN_H_P] = j >> 8;
    buf[IP_TOTLEN_L_P] = j & 0xff;
    make_ip(buf);
    // calculate the checksum, len=8 (start from ip.src) + TCP_HEADER_LEN_PLAIN + data len
    j = checksum(&buf[IP_SRC_P], 8 + TCP_HEADER_LEN_PLAIN, 2);
    buf[TCP_CHECKSUM_H_P] = j >> 8;
    buf[TCP_CHECKSUM_L_P] = j & 0xff;
    enc28j60PacketSend(IP_HEADER_LEN + TCP_HEADER_LEN_PLAIN + ETH_HEADER_LEN, buf);
}

// you must have called init_len_info at some time before calling this function
// dlen is the amount of tcp data (http data) we send in this packet
// You can use this function only immediately after make_tcp_ack_from_any
// This is because this function will NOT modify the eth/ip/tcp header except for
// length and checksum
void make_tcp_ack_with_data(unsigned char *buf, unsigned  int dlen)
{
    unsigned  int j;
    // fill the header:
    // This code requires that we send only one data packet
    // because we keep no state information. We must therefore set
    // the fin here:
    buf[TCP_FLAGS_P] = TCP_FLAGS_ACK_V | TCP_FLAGS_PUSH_V | TCP_FLAGS_FIN_V;
    // total length field in the IP header must be set:
    // 20 bytes IP + 20 bytes tcp (when no options) + len of data
    j = IP_HEADER_LEN + TCP_HEADER_LEN_PLAIN + dlen;
    buf[IP_TOTLEN_H_P] = j >> 8;
    buf[IP_TOTLEN_L_P] = j & 0xff;
    fill_ip_hdr_checksum(buf);
    // zero the checksum
    buf[TCP_CHECKSUM_H_P] = 0;
    buf[TCP_CHECKSUM_L_P] = 0;
    // calculate the checksum, len=8 (start from ip.src) + TCP_HEADER_LEN_PLAIN + data len
    j = checksum(&buf[IP_SRC_P], 8 + TCP_HEADER_LEN_PLAIN + dlen, 2);
    buf[TCP_CHECKSUM_H_P] = j >> 8;
    buf[TCP_CHECKSUM_L_P] = j & 0xff;
    enc28j60PacketSend(IP_HEADER_LEN + TCP_HEADER_LEN_PLAIN + dlen + ETH_HEADER_LEN, buf);
}









