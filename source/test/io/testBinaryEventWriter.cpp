#include "gtest/gtest.h"
#include "core/Event.h"
#include "io/BinaryEventWriter.h"
#include <istream>
#include <sstream>

using namespace des;

namespace testBinaryEventWriter
{
  constexpr std::streamsize BUFF_SIZE = 1024;
  constexpr std::streamsize EVT_SIZE = sizeof(SimTime) + sizeof(EventType) + sizeof(EventTag);
}
using namespace testBinaryEventWriter;

TEST(testBinaryEventWriter, ctor)
{
  std::stringstream str{std::ios_base::in | std::ios_base::out | std::ios_base::binary};

  {
    ASSERT_NO_THROW(BinaryEventWriter{str.rdbuf()});
    ASSERT_NO_THROW(BinaryEventWriter{str});
    ASSERT_THROW(BinaryEventWriter{nullptr}, std::invalid_argument);
  }

  {
    BinaryEventWriter ser{str.rdbuf()};
    EXPECT_EQ(std::ios_base::goodbit, ser.stream().rdstate());
  }
}

TEST(testBinaryEventWriter, write_bufferCtor)
{
  std::stringstream str{std::ios_base::in | std::ios_base::out | std::ios_base::binary};
  BinaryEventWriter writer{str.rdbuf()};

  char buffer[BUFF_SIZE];
  char* pBuffer = buffer;
  
  Event e{12, 34, 56};

  // Write event (should not throw)
  ASSERT_NO_THROW(writer.write(e));
  EXPECT_TRUE(writer.stream().good());

  // Read stream data from buffer
  str.read(buffer, EVT_SIZE);
  EXPECT_TRUE(str.good());

  // Check buffer
  EXPECT_EQ(e.time(), *(SimTime*)pBuffer);
  pBuffer += sizeof(SimTime);
  EXPECT_EQ(e.type(), *(EventType*)pBuffer);
  pBuffer += sizeof(EventType);
  EXPECT_EQ(e.tag(), *(EventTag*)pBuffer);
  
  // Check for EOF
  str.read(buffer, 1);
  EXPECT_FALSE(str.good());
  EXPECT_TRUE(str.eof());
}

TEST(testBinaryEventWriter, write_streamCtor)
{
  std::stringstream str{std::ios_base::in | std::ios_base::out | std::ios_base::binary};
  BinaryEventWriter writer{str};

  char buffer[BUFF_SIZE];
  char* pBuffer = buffer;
  
  Event e{12, 34, 56};

  // Write event (should not throw)
  ASSERT_NO_THROW(writer.write(e));
  EXPECT_TRUE(writer.stream().good());

  // Read stream data from buffer
  str.read(buffer, EVT_SIZE);
  EXPECT_TRUE(str.good());

  // Check buffer
  EXPECT_EQ(e.time(), *(SimTime*)pBuffer);
  pBuffer += sizeof(SimTime);
  EXPECT_EQ(e.type(), *(EventType*)pBuffer);
  pBuffer += sizeof(EventType);
  EXPECT_EQ(e.tag(), *(EventTag*)pBuffer);
  
  // Check for EOF
  str.read(buffer, 1);
  EXPECT_FALSE(str.good());
  EXPECT_TRUE(str.eof());
}
