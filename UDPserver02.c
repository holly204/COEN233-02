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
#include <errno.h>
#include "packet02.h"

//define prot
#define PORT 8800


#define function
void show_req(struct RequestPacket rpt);
void show_resp(struct ResponsePacket rsp);
ResponsePacket generate_recv(struct RequestPacket rqt);
int receive_packet(int sockfd,struct sockaddr_in *client_addr, socklen_t addr_size);

int main()
{

        char *ip = "127.0.0.1";

        int sockfd, b;
        struct sockaddr_in server_addr, client_addr;
        socklen_t addr_size;

        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0){
                perror("socket error!");
                exit(1);
        }

        memset(&server_addr, '\0', sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORT);
        server_addr.sin_addr.s_addr = inet_addr(ip);
	
	b = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
        if(b < 0){
                perror("bind error!");
                exit(1);
        
	}

        receive_packet(sockfd, &client_addr, addr_size);

	return 0;
}

ResponsePacket generate_recv(struct RequestPacket rqt){
	struct ResponsePacket rsp;
	rsp.StartPacketId = START_IDENTIFIER;
	rsp.ClientId = rqt.ClientId;
	rsp.data = ACCESS_OK;
	rsp.SegmentNo = rqt.SegmentNo;
	rsp.Length = rqt.Length;
	rsp.Technology = rqt.Technology;
	rsp.SourceSubscriberNo = rqt.SourceSubscriberNo;
	rsp.EndPacketId = END_IDENTIFIER;
	//printf("Generate ACK");
	return rsp;
}


int receive_packet(int sockfd,struct sockaddr_in *client_addr, socklen_t addr_size) {
        // 1. call recvfrom to receive request packet from client
        // 2. read Verification_Database.txt to check access or denied
        // 3. send packet back to client.
	int rev = 1;
	RequestPacket *rp = malloc(sizeof(RequestPacket));
	while(rev>0){
		addr_size = sizeof(client_addr);
		uint8_t *buffer = (uint8_t *)rp;
        	rev = recvfrom(sockfd, buffer, sizeof(RequestPacket), 0,(struct sockaddr*)&client_addr, &addr_size);
		printf("request packetreceived %d bytes\n", rev);
		show_req(*rp);

		uint8_t response[sizeof(ResponsePacket)] = {0};
		// Must use the addr_size from the previous recvfrom to specify addr length
		int send_ack = sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, addr_size);
		printf("ACcess send ack %d bytes\n", send_ack);

	}	
       	return rev;
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
        printf("\nSegment No:%x ", rsp.SegmentNo);
	printf("\nLength:%x ", rsp.Length);
	printf("\nTechnology:%x ", rsp.Technology);
	printf("\nSourceSubscriberNo:%x ", rsp.SourceSubscriberNo);
        printf("\nEnd of Packet id:%x \n", rsp.EndPacketId);
}

