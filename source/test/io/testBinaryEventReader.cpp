#include "gtest/gtest.h"
#include "core/Event.h"
#include "io/BinaryEventReader.h"
#include <ostream>
#include <sstream>

using namespace des;

namespace testBinaryEventReader
{
  constexpr std::streamsize BUFF_SIZE = 1024;
  constexpr std::streamsize EVT_SIZE = sizeof(SimTime) + sizeof(EventType) + sizeof(EventTag);
}
using namespace testBinaryEventReader;

TEST(testBinaryEventReader, ctor)
{
  std::stringstream str{std::ios_base::in | std::ios_base::out | std::ios_base::binary};

  {
    ASSERT_NO_THROW(BinaryEventReader{str.rdbuf()});
    ASSERT_NO_THROW(BinaryEventReader{str});
    ASSERT_THROW(BinaryEventReader{nullptr}, std::invalid_argument);
  }

  {
    BinaryEventReader ser{str.rdbuf()};
    EXPECT_EQ(std::ios_base::goodbit, ser.stream().rdstate());
  }
}

TEST(testBinaryEventReader, read_bufferCtor)
{
  std::stringstream str{std::ios_base::in | std::ios_base::out | std::ios_base::binary};
  BinaryEventReader reader{str.rdbuf()};

  char buffer[BUFF_SIZE];
  char* pBuffer = buffer;

  // Fill buffer with event data
  *(SimTime*)pBuffer = 12;
  pBuffer += sizeof(SimTime);
  *(EventType*)pBuffer = 34;
  pBuffer += sizeof(EventType);
  *(EventTag*)pBuffer = 56;
  pBuffer += sizeof(EventTag);

  // Write buffer to stream
  str.write(buffer, EVT_SIZE);
  EXPECT_TRUE(str.good());

  try
  {
    // Read event (should not throw)
    Event evt = reader.read();
    EXPECT_TRUE(reader.stream().good());

    // Check event
    EXPECT_EQ(12, evt.time());
    EXPECT_EQ(34, evt.type());
    EXPECT_EQ(56, evt.tag());
  }
  catch(EventReadException& ex)
  {
    FAIL() << "EventReadException:  " << ex.what();
  }

  // Check for EOF
  str.read(buffer, 1);
  EXPECT_FALSE(str.good());
  EXPECT_TRUE(str.eof());
}

TEST(testBinaryEventReader, read_streamCtor)
{
  std::stringstream str{std::ios_base::in | std::ios_base::out | std::ios_base::binary};
  BinaryEventReader reader{str};

  char buffer[BUFF_SIZE];
  char* pBuffer = buffer;

  // Fill buffer with event data
  *(SimTime*)pBuffer = 12;
  pBuffer += sizeof(SimTime);
  *(EventType*)pBuffer = 34;
  pBuffer += sizeof(EventType);
  *(EventTag*)pBuffer = 56;
  pBuffer += sizeof(EventTag);

  // Write buffer to stream
  str.write(buffer, EVT_SIZE);
  EXPECT_TRUE(str.good());

  try
  {
    // Read event (should not throw)
    Event evt = reader.read();
    EXPECT_TRUE(reader.stream().good());

    // Check event
    EXPECT_EQ(12, evt.time());
    EXPECT_EQ(34, evt.type());
    EXPECT_EQ(56, evt.tag());
  }
  catch(EventReadException& ex)
  {
    FAIL() << "EventReadException:  " << ex.what();
  }

  // Check for EOF
  str.read(buffer, 1);
  EXPECT_FALSE(str.good());
  EXPECT_TRUE(str.eof());
}

TEST(testBinaryEventReader, read_bufferUnderflow)
{
  std::stringstream str{std::ios_base::in | std::ios_base::out | std::ios_base::binary};
  BinaryEventReader reader{str.rdbuf()};

  char buffer[BUFF_SIZE];
  char* pBuffer = buffer;

  // Partially fill buffer with event data
  *(SimTime*)pBuffer = 12;
  pBuffer += sizeof(SimTime);
  *(EventType*)pBuffer = 34;
  pBuffer += sizeof(EventType);

  // Write buffer to stream
  str.write(buffer, sizeof(SimTime) + sizeof(EventType));
  EXPECT_TRUE(str.good());

  // Read event (should throw)
  ASSERT_THROW(reader.read(), EventReadException);
  EXPECT_FALSE(reader.stream().good());

  // Check for EOF
  str.read(buffer, 1);
  EXPECT_FALSE(str.good());
  EXPECT_TRUE(str.eof());
}

TEST(testBinaryEventReader, readMultiple)
{
  std::stringstream str{std::ios_base::in | std::ios_base::out | std::ios_base::binary};
  BinaryEventReader reader{str};

  char buffer[BUFF_SIZE];
  char* pBuffer = buffer;

  // Fill buffer with event data
  *(SimTime*)pBuffer = 112;
  pBuffer += sizeof(SimTime);
  *(EventType*)pBuffer = 134;
  pBuffer += sizeof(EventType);
  *(EventTag*)pBuffer = 156;
  pBuffer += sizeof(EventTag);

  *(SimTime*)pBuffer = 212;
  pBuffer += sizeof(SimTime);
  *(EventType*)pBuffer = 234;
  pBuffer += sizeof(EventType);
  *(EventTag*)pBuffer = 256;
  pBuffer += sizeof(EventTag);

  *(SimTime*)pBuffer = 312;
  pBuffer += sizeof(SimTime);
  *(EventType*)pBuffer = 334;
  pBuffer += sizeof(EventType);
  *(EventTag*)pBuffer = 356;
  pBuffer += sizeof(EventTag);

  // Write buffer to stream
  str.write(buffer, EVT_SIZE*3);
  EXPECT_TRUE(str.good());

  try
  {
    // Read events (should not throw)
    Event evt1 = reader.read();
    EXPECT_TRUE(reader.stream().good());
    EXPECT_EQ(112, evt1.time());
    EXPECT_EQ(134, evt1.type());
    EXPECT_EQ(156, evt1.tag());

    Event evt2 = reader.read();
    EXPECT_TRUE(reader.stream().good());
    EXPECT_EQ(212, evt2.time());
    EXPECT_EQ(234, evt2.type());
    EXPECT_EQ(256, evt2.tag());

    Event evt3 = reader.read();
    EXPECT_TRUE(reader.stream().good());
    EXPECT_EQ(312, evt3.time());
    EXPECT_EQ(334, evt3.type());
    EXPECT_EQ(356, evt3.tag());
  }
  catch(EventReadException& ex)
  {
    FAIL() << "EventReadException:  " << ex.what();
  }

  // Check for EOF
  str.read(buffer, 1);
  EXPECT_FALSE(str.good());
  EXPECT_TRUE(str.eof());
}
