#include "gtest/gtest.h"
#include "core/Event.h"
#include "io/JsonEventReader.h"
#include <ostream>
#include <sstream>

using namespace des;

namespace testJsonEventReader
{
  constexpr std::streamsize BUFF_SIZE = 1024;
}
using namespace testJsonEventReader;

TEST(testJsonEventReader, ctor)
{
  std::stringstream str{std::ios_base::in | std::ios_base::out};

  {
    ASSERT_NO_THROW(JsonEventReader{str.rdbuf()});
    ASSERT_NO_THROW(JsonEventReader{str});
    ASSERT_THROW(JsonEventReader{nullptr}, std::invalid_argument);
  }

  {
    JsonEventReader ser{str.rdbuf()};
    EXPECT_EQ(std::ios_base::goodbit, ser.stream().rdstate());
  }
}

TEST(testJsonEventReader, read_bufferCtor)
{
  std::stringstream str{std::ios_base::in | std::ios_base::out};
  JsonEventReader reader{str.rdbuf()};
  
  char buffer[BUFF_SIZE];

  // Create JSON string
  std::stringstream stringBuilder{};
  stringBuilder << "{";
  stringBuilder << "\"time\":" << (SimTime)12;
  stringBuilder << ",\"type\":" << (EventType)34;
  stringBuilder << ",\"tag\":" << (EventTag)56;
  stringBuilder << "}";
  const std::string json{stringBuilder.str()};

  // Write JSON to stream
  str << json;
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

TEST(testJsonEventReader, read_streamCtor)
{
  std::stringstream str{std::ios_base::in | std::ios_base::out};
  JsonEventReader reader{str};
  
  char buffer[BUFF_SIZE];

  // Create JSON string
  std::stringstream stringBuilder{};
  stringBuilder << "{";
  stringBuilder << "\"time\":" << (SimTime)12;
  stringBuilder << ",\"type\":" << (EventType)34;
  stringBuilder << ",\"tag\":" << (EventTag)56;
  stringBuilder << "}";
  const std::string json{stringBuilder.str()};

  // Write JSON to stream
  str << json;
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

TEST(testJsonEventReader, read_missingTime)
{
  std::stringstream str{std::ios_base::in | std::ios_base::out};
  JsonEventReader reader{str.rdbuf()};
  
  char buffer[BUFF_SIZE];

  // Create JSON string
  std::stringstream stringBuilder{};
  stringBuilder << "{";
  stringBuilder << "\"type\":" << (EventType)34;
  stringBuilder << ",\"tag\":" << (EventTag)56;
  stringBuilder << "}";
  const std::string json{stringBuilder.str()};

  // Write JSON to stream
  str << json;
  EXPECT_TRUE(str.good());

  // Read event (should throw)
  ASSERT_THROW(reader.read(), EventReadException);
  EXPECT_TRUE(reader.stream().good());

  // Check for EOF
  str.read(buffer, 1);
  EXPECT_FALSE(str.good());
  EXPECT_TRUE(str.eof());
}

TEST(testJsonEventReader, read_missingType)
{
  std::stringstream str{std::ios_base::in | std::ios_base::out};
  JsonEventReader reader{str.rdbuf()};
  
  char buffer[BUFF_SIZE];

  // Create JSON string
  std::stringstream stringBuilder{};
  stringBuilder << "{";
  stringBuilder << "\"time\":" << (SimTime)12;
  stringBuilder << ",\"tag\":" << (EventTag)56;
  stringBuilder << "}";
  const std::string json{stringBuilder.str()};

  // Write JSON to stream
  str << json;
  EXPECT_TRUE(str.good());

  // Read event (should throw)
  ASSERT_THROW(reader.read(), EventReadException);
  EXPECT_TRUE(reader.stream().good());

  // Check for EOF
  str.read(buffer, 1);
  EXPECT_FALSE(str.good());
  EXPECT_TRUE(str.eof());
}

TEST(testJsonEventReader, read_missingTag)
{
  std::stringstream str{std::ios_base::in | std::ios_base::out};
  JsonEventReader reader{str.rdbuf()};
  
  char buffer[BUFF_SIZE];

  // Create JSON string
  std::stringstream stringBuilder{};
  stringBuilder << "{";
  stringBuilder << "\"time\":" << (SimTime)12;
  stringBuilder << ",\"type\":" << (EventType)34;
  stringBuilder << "}";
  const std::string json{stringBuilder.str()};

  // Write JSON to stream
  str << json;
  EXPECT_TRUE(str.good());

  // Read event (should throw)
  ASSERT_THROW(reader.read(), EventReadException);
  EXPECT_TRUE(reader.stream().good());

  // Check for EOF
  str.read(buffer, 1);
  EXPECT_FALSE(str.good());
  EXPECT_TRUE(str.eof());
}

TEST(testJsonEventReader, read_malformed)
{
  std::stringstream str{std::ios_base::in | std::ios_base::out};
  JsonEventReader reader{str.rdbuf()};

  // Create JSON string with missing closing brace
  std::stringstream stringBuilder{};
  stringBuilder << "{";
  stringBuilder << "\"time\":" << (SimTime)12;
  stringBuilder << ",\"type\":" << (EventType)34;
  stringBuilder << ",\"tag\":" << (EventTag)56;
  const std::string json{stringBuilder.str()};

  // Write JSON to stream
  str << json;
  EXPECT_TRUE(str.good());

  // Read event (should throw)
  ASSERT_THROW(reader.read(), EventReadException);
  EXPECT_FALSE(reader.stream().good());
}

TEST(testJsonEventReader, readMultiple)
{
  std::stringstream str{std::ios_base::in | std::ios_base::out};
  JsonEventReader reader{str};
  
  char buffer[BUFF_SIZE];

  // Create JSON string
  std::stringstream stringBuilder{};

  stringBuilder << "{";
  stringBuilder << "\"time\":" << (SimTime)112;
  stringBuilder << ",\"type\":" << (EventType)134;
  stringBuilder << ",\"tag\":" << (EventTag)156;
  stringBuilder << "}";

  stringBuilder << "{";
  stringBuilder << "\"time\":" << (SimTime)212;
  stringBuilder << ",\"type\":" << (EventType)234;
  stringBuilder << ",\"tag\":" << (EventTag)256;
  stringBuilder << "}";

  stringBuilder << "{";
  stringBuilder << "\"time\":" << (SimTime)312;
  stringBuilder << ",\"type\":" << (EventType)334;
  stringBuilder << ",\"tag\":" << (EventTag)356;
  stringBuilder << "}";

  const std::string json{stringBuilder.str()};

  // Write JSON to stream
  str << json;
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
