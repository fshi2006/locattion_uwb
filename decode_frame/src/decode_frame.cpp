#include "decode_frame.hpp"



//uart reicer flag
#define b_uart_head  80  //收到55 头 标志位
#define b_rx_over    0x40  //收到完整的帧标志
// USART Receiver buffer
#define RX_BUFFER_SIZE 100 //接收缓冲区字节数
using namespace std;
void Decode_frame(unsigned char data);
volatile unsigned char rx_buffer[RX_BUFFER_SIZE]; //接收数据缓冲区
volatile unsigned char rx_wr_index; //缓冲写指针
volatile unsigned char RC_Flag_1;  //接收1位标志字节
//解算后的角度值

float 	roll,  //偏航角
				pitch,//俯仰
				yaw, //滚转
				tempr;//温度

float 	wx,wy,wz;//角速度

void DecodeFrame::Decode_frame(unsigned char data){
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;
	ucRxBuffer[ucRxCnt++]=data; 
	
	if (ucRxBuffer[0] != 0x6D )
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<65)
		return;
	else
	{
		if(ucRxBuffer[1] == 0x63)
		{
			for(int i=0;i<130;i++)
			{
				if((ucRxBuffer[i]=='m') && (ucRxBuffer[i+1]=='c'))
				{
				std::cout <<  ucRxBuffer << std::endl;
				memcpy(buf,&ucRxBuffer[i+6],8);
				sscanf((const char* )buf,"%x",&Range_t[0]); 				
				memcpy(buf,&ucRxBuffer[i+15],8);
				sscanf((const char* )buf,"%x",&Range_t[1]); 	
				memcpy(buf,&ucRxBuffer[i+24],8);
				sscanf((const char* )buf,"%x",&Range_t[2]); 
				memcpy(buf,&ucRxBuffer[i+33],8);
				sscanf((const char* )buf,"%x",&Range_t[3]); 			
				memcpy(&Tag,&ucRxBuffer[i+60],1);
				}
				if(Range_t[0] != 0)
					Range_deca[0] = Range_t[0];
				if(Range_t[1] != 0)
					Range_deca[1] = Range_t[1];
				if(Range_t[2] != 0)
					Range_deca[2] = Range_t[2];
				if(Range_t[3] != 0)
					Range_deca[3] = Range_t[3];
			}
			std::cout <<"1: "  << Range_deca[0] <<"  2:  " << Range_deca[1]<<"  3:  "   << Range_deca[2] <<"  4:  "  << Range_deca[3]  <<std::endl;
				
				//memcpy(&dis0,&ucRxBuffer[6],8);
				//memcpy(&dis1,&ucRxBuffer[15],8);
				//memcpy(&dis2,&ucRxBuffer[24],8);
				//memcpy(&dis3,&ucRxBuffer[33],8);
				// for(int i=0;i<8;i++)
				// {
				// 	dis0[i] = ucRxBuffer[6+i];
				// 	dis1[i] = ucRxBuffer[15+i];
				// 	dis2[i] = ucRxBuffer[24+i];
				// 	dis3[i] = ucRxBuffer[33+i];

				// }
				// A0 = hex2deci(dis0);
				// A1  = hex2deci(dis1);
				// A2 = hex2deci(dis2);
				// A3  = hex2deci(dis3);
				// std::cout  <<  "___________________________________________________________" <<std::endl;
				// std::cout  << dis0 << "  A0 = " << A0 <<std::endl<< dis1 <<std::endl<< dis2<<std::endl << dis3 <<std::endl;


		}
		ucRxCnt=0;
	}

}

uint32_t DecodeFrame::hex2deci(const char *strHex)
	{
		uint32_t dwValue = 0;
		int cnt=0;
		const char *pCh = strHex;
		while (*pCh != 0 && cnt<8) {
			cnt++;
			dwValue <<= 4;
			if (*pCh >= '0' && *pCh <= '9')
				dwValue += *pCh - '0';
			else if (*pCh >= 'A' && *pCh <= 'F')
				dwValue += *pCh - 'A' + 10;
			else if (*pCh >= 'a' && *pCh <= 'f')
				dwValue += *pCh - 'a' + 10;
			else
				return 0;
			pCh++;
			}
		//std::cout  << "000000000000000000" << dwValue <<std::endl;
		return dwValue;
	}