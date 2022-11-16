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
#include "packet02.h"

//define port
#define PORT 8800

#define function
void show_req(struct RequestPacket rpt);
void show_resp(struct ResponsePacket rsp);
RequestPacket generatePacket(int n, int tech, int ssNo);
int send_packet(int sockfd, struct sockaddr_in *addr, RequestPacket *rp, socklen_t addr_size);

int main()
{

	char *ip = "127.0.0.1";

    	int sockfd;
    	struct sockaddr_in addr;    
    	socklen_t addr_size;
		
	//create socket
    	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	//set ack_timer
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

	
	//1 corrcet packet
	packet = generatePacket(4085546805, 04, 1);
	send_packet(sockfd, &addr, &packet, addr_size);
	
	//2 not paid
	//packet = generatePacket(4086668821, 03, 2);
	//send_packet(sockfd, &addr, &packet, addr_size);

	//3 not exit (Subscriber number is not found)
	//packet = generatePacket(4086808822, 02, 3);
	//send_packet(sockfd, &addr, &packet, addr_size);
	
	//4 not exit (Subscriber number is found, but the technology does not match)
	//packet = generatePacket(4086808822, 02, 4);
	//send_packet(sockfd, &addr, &packet, addr_size);


	return 0;
}

RequestPacket generatePacket(int ssNo, int tech, int n) {


        struct RequestPacket rp;

        rp.StartPacketId = START_IDENTIFIER;
        rp.ClientId = 11;
        rp.AccPer = ACC_PER;
        rp.SegmentNo = n;
	rp.Technology = tech;
	rp.SourceSubscriberNo = ssNo;
        rp.Length = strlen(rp.Technology + rp.SourceSubscriberNo);
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
	printf("Request Packet send %d bytes\n", ret);

	//receive packet
	printf("begin packet receive");
	addr_size = sizeof(addr);	
	
	//buffer =  malloc(sizeof(RejectPacket));
	//buffer[1024];
	ResponsePacket *rsp = malloc(sizeof(ResponsePacket));
	buffer = (uint8_t *)(ap);

	int rev_pack = recvfrom(sockfd, buffer, sizeof(ResponsePacket), 0, (struct sockaddr*)&addr, &addr_size);
	printf("ACC_PER received %d bytes\n", rev_pack);
	printf("buffer size:%d", sizeof(buffer));
        for(int i = 0; i < sizeof(buffer); i++) {
                printf("%x ", buffer[i]);
        }

	return ret;
}
void show_req(struct RequestPacket rpt){
        printf("\nStart of Packet id:%x ", rpt.StartPacketId);
        printf("\nClient ID:%x ", rpt.ClientId);
        printf("\nAccPer:%x",ACC_PER);
        printf("\nSegment No:%x ", rpt.SegmentNo);
        printf("\nLength:%x ", rpt.SegmentNo);
	printf("\nTechnology:%x ", rpt.Technology);
        printf("\nSourceSubscriberNo:%x ", rpt.SourceSubscriberNo);
        printf("\nEnd of Packet id:%x \n", rpt.EndPacketId);

}
void show_resp(struct ResponsePacket rsp){

        printf("\nStart of Packet id:%x ", rsp.StartPacketId);
        printf("\nClient id:%x ", rsp.ClientId);
        printf("\ndata:%x ", rsp.data);
        printf("\nSegment No:%x ", rpt.SegmentNo);
	printf("\nLength:%x ", rpt.Length);
	printf("\nTechnology:%x ", rpt.Technology);
	printf("\nSourceSubscriberNo:%x ", rpt.SourceSubscriberNo);
        printf("\nEnd of Packet id:%x \n", rsp.EndPacketId);
}

