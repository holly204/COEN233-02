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
#define LINE 5


#define function
void readFile (struct SubscriberData subscriber[]);
ResponsePacket generate_recv(struct RequestPacket rqt);

void show_req(struct RequestPacket rpt);
void show_resp(struct ResponsePacket rsp);
int verify (struct SubscriberData subscriber[],unsigned int subscriberNo,uint8_t Technology);
int receive_packet(int sockfd,struct sockaddr_in *client_addr, socklen_t addr_size, struct SubscriberData *subscriber);
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
	
	struct SubscriberData subscriber[LINE];
	readFile(subscriber);
        receive_packet(sockfd, &client_addr, addr_size, subscriber);

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
	return rsp;
}


int receive_packet(int sockfd,struct sockaddr_in *client_addr, socklen_t addr_size, struct SubscriberData *subscriber) {
        // 1. call recvfrom to receive request packet from client
        // 2. read Verification_Database.txt to check access or denied
        // 3. send packet back to client.
	int rev = 1;
	RequestPacket *rp;
	while(rev>0){
		addr_size = sizeof(client_addr);
		uint8_t *buffer = (uint8_t *)rp;
        	rev = recvfrom(sockfd, buffer, sizeof(RequestPacket), 0,(struct sockaddr*)&client_addr, &addr_size);
		show_req(*rp);
		
		ResponsePacket * rsp = malloc(sizeof(ResponsePacket));
		
		*rsp = generate_recv(*rp); 		
		int pay = verify (subscriber, rp->SourceSubscriberNo, rp->Technology);
		
		if (pay == 1){
			rsp->data = ACCESS_OK;
		}else if(pay == 0){
			rsp->data = NOTPAID;
		}
		else{
			rsp->data = NOTEXIST;
		}
		// Must use the addr_size from the previous recvfrom to specify addr length
		int send_ack = sendto(sockfd, rsp, sizeof(ResponsePacket), 0, (struct sockaddr*)&client_addr, addr_size);
		show_resp(*rsp);
	}	
       	return rev;
}
//read file
void readFile(struct SubscriberData subscriber[]) {
	char str[30];
	int i = 0;
	FILE *fp;
	//open the database file
	fp = fopen("Verification_Database.txt", "rt");

	if(fp == NULL)
	{
		printf("Error opening file\n");
	}
	
	while (fgets(str, sizeof(str), fp) != NULL)
	{
		char *token;
		token = strtok(str," ");
		//printf("token: %s", token);
		subscriber[i].subscriberNo = (unsigned) atoi(token);
		token = strtok(NULL," ");
		subscriber[i].Technology = atoi(token);
		token = strtok(NULL," ");
		subscriber[i].Paid = atoi(token);
		i++;
	}

	fclose(fp);
}
//verify subscriberNo and Technology 
int verify (struct SubscriberData subscriber[],unsigned int subscriberNo,uint8_t Technology) {
	int status = -1;// -1 Not exit
	for (int j = 0; j < LINE; j++) {
		if (subscriber[j].subscriberNo == subscriberNo && subscriber[j].Technology == Technology) {
			status = subscriber[j].Paid;
		}
		
	}
	return status;
}
void show_req(struct RequestPacket rpt){
	printf("************Request packet from client*******************\n");
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

	printf("************Send Response packet to client*******************\n");
        printf("Start of Packet id:%x \n", rsp.StartPacketId);
        printf("Client id:%d \n", rsp.ClientId);
        if (rsp.data == ACCESS_OK){
		printf("Acc_OK:%x \n", ACCESS_OK);
	}else if (rsp.data == NOTPAID){
		printf("Not paid: %x\n", NOTPAID);
	}else if(rsp.data == NOTEXIST){
		printf("Not exist: %x\n", NOTEXIST);
	}else{
		printf("ERROR");
	}
	//printf("data:%x \n", rsp.data);
        printf("Segment No:%d \n", rsp.SegmentNo);
	printf("Length:%d \n", rsp.Length);
	printf("Technology:%d \n", rsp.Technology);
	printf("SourceSubscriberNo:%u \n", rsp.SourceSubscriberNo);
        printf("End of Packet id:%x \n", rsp.EndPacketId);
}
