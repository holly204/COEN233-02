/*
COEN 233 Computer Networks Program assignment 2
Client using customized protocol on top of UDP protocol for requesting identification from server for access permission to the network.
Name: Li Huang
Student ID: W1641460
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "packet02.h"

//define port
#define PORT 8800

#define function
void show_req(struct RequestPacket rpt);
void show_resp(struct ResponsePacket rsp);
RequestPacket generatePacket(char *ssNo, char *tech, int n);
int send_packet(int sockfd, struct sockaddr_in *addr, RequestPacket *rp, socklen_t addr_size);

int main()
{

	char *ip = "127.0.0.1";

    	int sockfd;
    	struct sockaddr_in addr;    
    	socklen_t addr_size;
		
	//create socket
    	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	//set ack_timer s seconds
	struct timeval ack_timer;
	// Timeout Value as required
	ack_timer.tv_sec = 3;
	ack_timer.tv_usec = 0;

	// verifying for connection to the server
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&ack_timer,sizeof(struct timeval));	
	// apply address
        memset(&addr, '\0', sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(PORT);
        addr.sin_addr.s_addr = inet_addr(ip);
	RequestPacket packet;


	//1 corrcet packet
	char ssNo1[]= "4085546805";
	char tech1[]= "04";
	packet = generatePacket(ssNo1, tech1, 1);
	send_packet(sockfd, &addr, &packet, addr_size);

	//2 not paidi
	char ssNo2[]= "4086668821";
	char tech2[]= "03";
	packet = generatePacket(ssNo2, tech2, 2);
	send_packet(sockfd, &addr, &packet, addr_size);

	//3 not exit (Subscriber number is not found)
	
	char ssNo3[]= "4086808829";
	char tech3[]= "03";
	packet = generatePacket(ssNo3, tech3, 3);
	send_packet(sockfd, &addr, &packet, addr_size);
	
	//4 not exit (Subscriber number is found, but the technology does not match)
	char ssNo4[]= "4086808821";
	char tech4[]= "05";
	packet = generatePacket(ssNo4, tech4, 4);
	send_packet(sockfd, &addr, &packet, addr_size);
	close(sockfd);

	return 0;
}

RequestPacket generatePacket(char *ssNo, char *tech, int n) {


        struct RequestPacket rp;

        rp.StartPacketId = START_IDENTIFIER;
        rp.ClientId = 11;
        rp.AccPer = ACC_PER;
        rp.SegmentNo = n;
	rp.Technology = atoi(tech);
	rp.SourceSubscriberNo =  atoi(ssNo);
        rp.Length = sizeof(rp.Technology + rp.SourceSubscriberNo);
        rp.EndPacketId = END_IDENTIFIER;

        return rp;
}
int send_packet(int sockfd, struct sockaddr_in *addr, RequestPacket *rp, socklen_t addr_size) {
        // 1. Call sendto to send request packet to server.
        // 2. Call recvfrom to receive packet from server.
        // 3. If timeout, start timer, go back to 2 
        // 4. Print access or Reject packet
	uint8_t *buffer;
	buffer = (uint8_t *)(rp);
        int ret = sendto(sockfd, buffer, sizeof(*rp),0, (struct sockaddr *)addr, sizeof(*addr));
	show_req(*rp);
	
	//receive packet
	addr_size = sizeof(addr);	
	
	ResponsePacket * rsp = malloc(sizeof(ResponsePacket));
	//uint8_t response[sizeof(ResponsePacket)] = {0};
	int rev_back = recvfrom(sockfd, rsp, sizeof(ResponsePacket), 0, (struct sockaddr*)&addr, &addr_size);
	show_resp(*rsp);
	return ret;
}
void show_req(struct RequestPacket rpt){
        printf("*************Send request packet to server*****************\n");
	printf("Start of Packet id: %x \n", rpt.StartPacketId);
        printf("Client ID:%d \n", rpt.ClientId);
        printf("AccPer:%x\n",ACC_PER);
        printf("Segment No:%d \n", rpt.SegmentNo);
        printf("Length:%d \n", rpt.Length);
	printf("Technology:%d \n", rpt.Technology);
        printf("SourceSubscriberNo:%u \n", rpt.SourceSubscriberNo);
        printf("End of Packet id:%x \n", rpt.EndPacketId);

}
void show_resp(struct ResponsePacket rsp){
	
	printf("*************Receive response packet from server*****************\n");
	printf("Start of Packet id:%x \n", rsp.StartPacketId);
	printf("Client id:%d \n", rsp.ClientId);
        printf("data:%x \n", rsp.data);
        printf("Segment No:%d \n", rsp.SegmentNo);
	printf("Length:%d \n", rsp.Length);
	printf("Technology:%d \n", rsp.Technology);
	printf("SourceSubscriberNo:%u \n", rsp.SourceSubscriberNo);
        printf("End of Packet id:%x \n", rsp.EndPacketId);

        if (rsp.data == ACCESS_OK){
                printf("ClientID: %d Subscriber No: %u Technology: %d is Acc_OK\n", rsp.ClientId, rsp.SourceSubscriberNo, rsp.Technology);
        }else if (rsp.data == NOTPAID){
                printf("ClientID: %d Subscriber No: %u Technology: %d is Not Paid\n", rsp.ClientId, rsp.SourceSubscriberNo, rsp.Technology);
        }else if(rsp.data == NOTEXIST){
                printf("ClientID: %d Subscriber No: %u Technology: %d is Not Exist\n", rsp.ClientId, rsp.SourceSubscriberNo, rsp.Technology);
        }else{
                printf("ERROR");
        }
	
}
