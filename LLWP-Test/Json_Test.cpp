#include "pch.h"

#include "../LoveLiveWallpaper/src/Json.h"

#include <fstream>

using namespace LLWP;

class JsonTest : public ::testing::Test
{};

TEST_F(JsonTest, Initialize)
{
    ::std::ifstream ifs;
    ifs.open("test.json");
    Json json(Json::Deserialize(ifs));
    ifs.close();
    EXPECT_EQ((int)json["int"], -1234);
    EXPECT_FLOAT_EQ((float)json["float"], 12.34f);
    EXPECT_EQ(json["object"]["key"].as<std::string>(), "value");
    EXPECT_FLOAT_EQ((float)json["object"]["key2"], 1234e-2f);

}