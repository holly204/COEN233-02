#ifndef packet02_h
#define packet02_h


//define Identifier
#define START_IDENTIFIER  0XFFFF
#define END_IDENTIFIER  0XFFFF

//define ACC
#define  ACC_PER 0XFFF8
#define ACCESS_OK 0XFFFB

#define PAID 0XFFFB
#define NOTPAID 0XFFF9
#define NOTEXIST 0XFFFA

// Structure of Request Packet
typedef struct RequestPacket {
	uint16_t StartPacketId;
	uint8_t ClientId;
	uint16_t AccPer;
	uint8_t SegmentNo;
	uint8_t Length;
	uint8_t Technology;
	unsigned int SourceSubscriberNo;
	uint16_t EndPacketId;
}RequestPacket;
// Structure of response packet
typedef struct ResponsePacket {
	uint16_t StartPacketId;
	uint8_t ClientId;
	uint16_t data;
	uint8_t SegmentNo;
	uint8_t Length;
	uint8_t Technology;
	unsigned int SourceSubscriberNo;
	uint16_t EndPacketId;
}ResponsePacket;

// structure SubscriberData
typedef struct SubscriberData {
	unsigned subscriberNo;
	uint8_t Technology;
	int Paid; //1: paid, 0: not paid, -1:not exist
}SubscriberData;
#endif
