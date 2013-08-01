#include <stdlib.h>
#include "JSONEncoder.h"
#include "JSONEncoderTest.h"
#include "g2log.hpp"
#include "DpiMsgLR.h"

TEST_F(JSONEncoderTest, Construct) {
   networkMonitor::DpiMsgLR testMsg;

   JSONEncoder encoder(&testMsg);
   JSONEncoder* pEncoder = new JSONEncoder(&testMsg);
   delete pEncoder;
}

TEST_F(JSONEncoderTest, EncodeAnEmptyMessage) {
   networkMonitor::DpiMsgLR testMsg;

   JSONEncoder encoder(&testMsg);

   std::string encodedMessage = encoder.Encode();

   EXPECT_EQ("{\"uuid\": \"\"}", encodedMessage);
}

TEST_F(JSONEncoderTest, EncodeAMessageWithAString) {
   networkMonitor::DpiMsgLR testMsg;

   testMsg.set_uuid("ABC123");
   JSONEncoder encoder(&testMsg);

   std::string encodedMessage = encoder.Encode();

   EXPECT_EQ("{\"uuid\": \"ABC123\"}", encodedMessage);
}

TEST_F(JSONEncoderTest, EncodeAMessageWithAnInt) {
   networkMonitor::DpiMsgLR testMsg;

   testMsg.set_uuid("ABC123");
   JSONEncoder encoder(&testMsg);
   testMsg.set_destport(123);
   std::string encodedMessage = encoder.Encode();

   EXPECT_EQ("{\"destPort\": 123, \"uuid\": \"ABC123\"}", encodedMessage);
}

TEST_F(JSONEncoderTest, EncodeAMessageWithIpSrcDst) {
   networkMonitor::DpiMsgLR testMsg;

   testMsg.set_uuid("ABC123");
   JSONEncoder encoder(&testMsg);
   testMsg.set_ipsrc(0x11223344);
   testMsg.set_ipdst(0x55667788);
   std::string encodedMessage = encoder.Encode();

   EXPECT_EQ("{\"uuid\": \"ABC123\", \"ipDst\": \"136.119.102.85\", \"ipSrc\": \"68.51.34.17\"}", encodedMessage);
}

TEST_F(JSONEncoderTest, EncodeAMessageWithAnInt64) {
   networkMonitor::DpiMsgLR testMsg;

   testMsg.set_uuid("ABC123");
   JSONEncoder encoder(&testMsg);
   testMsg.set_deltatime(12345678900L);

   std::string encodedMessage = encoder.EncodeWithCallback(testMsg.CleanupName);

   EXPECT_EQ("{\"deltaTime\": 12345678900, \"uuid\": \"ABC123\"}", encodedMessage);
}

TEST_F(JSONEncoderTest, EncodeAMessageWithATimeT) {
   networkMonitor::DpiMsgLR testMsg;

   testMsg.set_uuid("ABC123");
   JSONEncoder encoder(&testMsg);
   testMsg.set_starttime(12345678900L);

   std::string encodedMessage = encoder.EncodeWithCallback(testMsg.CleanupName);

   EXPECT_EQ("{\"uuid\": \"ABC123\", \"startTime\": \"2361/03/21 19:15:00\"}", encodedMessage);
}

TEST_F(JSONEncoderTest, EncodeAMessageWithEthSrcDst) {
   networkMonitor::DpiMsgLR testMsg;

   testMsg.set_uuid("ABC123");
   JSONEncoder encoder(&testMsg);
   testMsg.set_ethsrc(12345678900L);
   testMsg.set_ethdst(12345678901L);

   std::string encodedMessage = encoder.EncodeWithCallback(testMsg.CleanupName);

   EXPECT_EQ("{\"uuid\": \"ABC123\", \"ethDst\": \"35:1c:dc:df:02:00\", \"ethSrc\": \"34:1c:dc:df:02:00\"}", encodedMessage);
}

TEST_F(JSONEncoderTest, EncodeAMessageWithRepeatedStringField) {
   networkMonitor::DpiMsgLR testMsg;

   testMsg.set_uuid("ABC123");
   testMsg.add_accept_encodingq_proto_http("1");
   testMsg.add_accept_encodingq_proto_http("2");
   testMsg.add_accept_encodingq_proto_http("3");

   JSONEncoder encoder(&testMsg);

   std::string encodedMessage = encoder.EncodeWithCallback(testMsg.CleanupName);

   EXPECT_EQ("{\"uuid\": \"ABC123\", \"#acceptEncoding\": [\"1\", \"2\", \"3\"]}", encodedMessage);
}

TEST_F(JSONEncoderTest, EncodeAMessageWithSingleBool) {
   networkMonitor::DpiMsgLR testMsg;

   testMsg.set_uuid("ABC123");
   testMsg.set_captured(true);
   JSONEncoder encoder(&testMsg);

   std::string encodedMessage = encoder.EncodeWithCallback(testMsg.CleanupName);

   EXPECT_EQ("{\"uuid\": \"ABC123\", \"captured\": true}", encodedMessage);
   testMsg.set_captured(false);
   JSONEncoder encoder2(&testMsg);

   encodedMessage = encoder2.EncodeWithCallback(testMsg.CleanupName);

   EXPECT_EQ("{\"uuid\": \"ABC123\", \"captured\": false}", encodedMessage);
}

TEST_F(JSONEncoderTest, EncodeAMessageWithRenamer) {
   networkMonitor::DpiMsgLR testMsg;

   testMsg.set_uuid("ABC123");
   testMsg.add_uri_fullq_proto_http("1");
   testMsg.add_application_endq_proto_base("test");
   testMsg.set_application_id_endq_proto_base(1234);
   testMsg.set_sessionlenserver(567);
   testMsg.set_deltasessionlenserver(567);
   testMsg.set_sessionlenclient(899);
   testMsg.set_deltasessionlenclient(899);
   testMsg.set_packetcount(88);
   testMsg.set_deltapackets(88);
   testMsg.add_loginq_proto_aim("aLogin");
   testMsg.add_domainq_proto_smb("aDomain12345");
   testMsg.add_uriq_proto_http("not/this/one");
   testMsg.add_serverq_proto_http("thisname12345");
   testMsg.add_referer_serverq_proto_http("notThisOne");
   testMsg.add_methodq_proto_ftp("RUN");
   testMsg.add_methodq_proto_ftp("COMMAND");
   testMsg.add_methodq_proto_ftp("LONGLONGLONGLONG");
   testMsg.add_senderq_proto_smtp("test1_123456");
   testMsg.add_receiverq_proto_smtp("test2_123");
   testMsg.add_subjectq_proto_smtp("test3_12345");
   testMsg.add_versionq_proto_http("4.0");
   JSONEncoder encoder(&testMsg);

   std::string encodedMessage = encoder.EncodeWithCallback(testMsg.CleanupName);

   EXPECT_EQ("{\"#server\": [\"thisname12345\"], \"#sender\": [\"test1_123456\"]"
           ", \"deltaPackets\": 88, \"#uri\": [\"not/this/one\"]"
           ", \"sessionLenClient\": 899, \"#refererServer\": [\"notThisOne\"]"
           ", \"#receiver\": [\"test2_123\"], \"#method\": [\"RUN\", \"COMMAND\","
           " \"LONGLONGLONGLONG\"], \"uuid\": \"ABC123\", \"packetCount\": 88,"
           " \"sessionLenServer\": 567, \"deltaSessionLenServer\": 567,"
           " \"deltaSessionLenClient\": 899, \"#subject\": [\"test3_12345\"],"
           " \"#applicationEnd\": [\"test\"], \"#applicationIdEnd\": 1234,"
           " \"#login\": [\"aLogin\"], \"#uriFull\": [\"1\"], \"#version\":"
           " [\"4.0\"], \"#domain\": [\"aDomain12345\"]}", encodedMessage);

}

TEST_F(JSONEncoderTest, encodesQuickSearchFields) {
   networkMonitor::DpiMsgLR testMsg;

   testMsg.set_uuid("ABC123");
   testMsg.add_application_endq_proto_base("test");
   testMsg.add_application_endq_proto_base("unknown");
   testMsg.add_applicationq_proto_base("test");
   testMsg.add_applicationq_proto_base("unknown");
   testMsg.set_sessionlenserver(567);
   testMsg.set_deltasessionlenserver(67);
   testMsg.set_sessionlenclient(899);
   testMsg.set_deltasessionlenclient(99);

   JSONEncoder encoder(&testMsg);

   std::string encodedMessage = encoder.EncodeWithCallback(testMsg.CleanupName);

   EXPECT_EQ("{\"sessionLenClient\": 899, \"uuid\": \"ABC123\", "
           "\"#application\": [\"test\", \"unknown\"], \"sessionLenServer\": 567, "
           "\"deltaSessionLenServer\": 67, \"deltaSessionLenClient\": 99, "
           "\"#applicationEnd\": [\"test\", \"unknown\"]}", encodedMessage);
   testMsg.UpdateQuickSearchFields();
   encodedMessage = encoder.EncodeWithCallback(testMsg.CleanupName);

   EXPECT_EQ("{\"sessionLenClient\": 899, \"applicationPath\": \"/test/unknown\", "
           "\"application\": \"unknown\", \"uuid\": \"ABC123\", \"#application\": [\"test\", "
           "\"unknown\"], \"sessionLenServer\": 567, \"deltaSessionLenServer\": 67, "
           "\"deltaSessionLenClient\": 99, \"bytesTotal\": 1466, \"bytesTotalDelta\": 166, "
           "\"#applicationEnd\": [\"test\", \"unknown\"]}", encodedMessage);
}