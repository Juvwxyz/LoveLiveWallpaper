#include "pch.h"

#include "../LoveLiveWallpaper/src/Json.h"

#include <fstream>

namespace LLWP
{
    class JsonTest : public ::testing::Test
    {
    public:
        Json json;

        void SetUp()
        {
            ::std::ifstream ifs;
            ifs.open("../../LLWP-Test/test.json");
            json = Json::Deserialize(ifs);
            ifs.close();
        }
    
    };

    TEST_F(JsonTest, Initialize)
    {
        EXPECT_EQ((int)json["int"], -1234);
        EXPECT_FLOAT_EQ((float)json["float"], 12.34f);
        EXPECT_EQ(json["object"]["key"].as<std::string>(), "value");
        EXPECT_FLOAT_EQ((float)json["object"]["key2"], 1234e-2f);

    }
}