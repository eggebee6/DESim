#include "gtest/gtest.h"
#include "core/Event.h"
#include "io/JsonEventWriter.h"
#include "nlohmann/json.hpp"
#include <istream>
#include <sstream>

using namespace des;

namespace testJsonEventWriter
{
  constexpr std::streamsize BUFF_SIZE = 1024;
}
using namespace testJsonEventWriter;

TEST(testJsonEventWriter, ctor)
{
  std::stringstream str{std::ios_base::in | std::ios_base::out};

  {
    ASSERT_NO_THROW(JsonEventWriter{str.rdbuf()});
    ASSERT_NO_THROW(JsonEventWriter{str});
    ASSERT_THROW(JsonEventWriter{nullptr}, std::invalid_argument);
  }

  {
    JsonEventWriter ser{str.rdbuf()};
    EXPECT_EQ(std::ios_base::goodbit, ser.stream().rdstate());
  }
}

TEST(testJsonEventWriter, write_bufferCtor)
{
  std::stringstream str{std::ios_base::in | std::ios_base::out};
  JsonEventWriter writer{str.rdbuf()};

  char buffer[BUFF_SIZE];

  Event e{12, 34, 56};

  // Write event (should not throw)
  ASSERT_NO_THROW(writer.write(e));
  EXPECT_TRUE(writer.stream().good());

  // Read object from stream
  nlohmann::json obj{};
  str >> obj;

  const auto itEnd = obj.end();

  auto it = obj.find("time");
  ASSERT_TRUE(it != itEnd);
  EXPECT_EQ(12, obj["time"]);

  it = obj.find("type");
  ASSERT_TRUE(it != itEnd);
  EXPECT_EQ(34, obj["type"]);

  it = obj.find("tag");
  ASSERT_TRUE(it != itEnd);
  EXPECT_EQ(56, obj["tag"]);

  // Check for EOF
  str.read(buffer, 1);
  EXPECT_FALSE(str.good());
  EXPECT_TRUE(str.eof());
}

TEST(testJsonEventWriter, write_streamCtor)
{
  std::stringstream str{std::ios_base::in | std::ios_base::out};
  JsonEventWriter writer{str};

  char buffer[BUFF_SIZE];

  Event e{12, 34, 56};

  // Write event (should not throw)
  ASSERT_NO_THROW(writer.write(e));
  EXPECT_TRUE(writer.stream().good());

  // Read object from stream
  nlohmann::json obj{};
  str >> obj;

  const auto itEnd = obj.end();

  auto it = obj.find("time");
  ASSERT_TRUE(it != itEnd);
  EXPECT_EQ(12, obj["time"]);

  it = obj.find("type");
  ASSERT_TRUE(it != itEnd);
  EXPECT_EQ(34, obj["type"]);

  it = obj.find("tag");
  ASSERT_TRUE(it != itEnd);
  EXPECT_EQ(56, obj["tag"]);

  // Check for EOF
  str.read(buffer, 1);
  EXPECT_FALSE(str.good());
  EXPECT_TRUE(str.eof());
}
