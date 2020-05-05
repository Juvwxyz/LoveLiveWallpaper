#include "pch.h"
#include "../LoveLiveWallpaper/src/Matrix.h"

using namespace LLWP;

class MatrixTest : public ::testing::Test
{
public:
    Matrix m1;

    Matrix m2 = LLWP::Matrix
    ({
        1,0,0,1,
        0,1,0,1,
        0,0,1,0,
        0,0,0,1
    });

    Matrix m3 = m1;
    Matrix m4 = std::move(m2);

    Matrix ma = LLWP::Matrix
    ({
        1,  2, 3, 4,
        5,  6, 7, 8,
        9, 10,11,12,
        13,14,15,16
    });
    Matrix mb = LLWP::Matrix
    ({
        1,1,1,1,
        1,1,1,1,
        1,1,1,1,
        1,1,1,1
    });
protected:

};

TEST_F(MatrixTest, Initialize)
{
    EXPECT_FLOAT_EQ(m1(0, 0), 1.f);
    EXPECT_FLOAT_EQ(m1(0, 1), 0.f);
    EXPECT_FLOAT_EQ(m1(0, 2), 0.f);
    EXPECT_FLOAT_EQ(m1(0, 3), 0.f);
    EXPECT_FLOAT_EQ(m1(1, 0), 0.f);
    EXPECT_FLOAT_EQ(m1(1, 1), 1.f);
    EXPECT_FLOAT_EQ(m1(1, 2), 0.f);
    EXPECT_FLOAT_EQ(m1(1, 3), 0.f);
    EXPECT_FLOAT_EQ(m1(2, 0), 0.f);
    EXPECT_FLOAT_EQ(m1(2, 1), 0.f);
    EXPECT_FLOAT_EQ(m1(2, 2), 1.f);
    EXPECT_FLOAT_EQ(m1(2, 3), 0.f);
    EXPECT_FLOAT_EQ(m1(3, 0), 0.f);
    EXPECT_FLOAT_EQ(m1(3, 1), 0.f);
    EXPECT_FLOAT_EQ(m1(3, 2), 0.f);
    EXPECT_FLOAT_EQ(m1(3, 3), 1.f);

    EXPECT_FLOAT_EQ(m2(0, 0), 1.f);
    EXPECT_FLOAT_EQ(m2(0, 1), 0.f);
    EXPECT_FLOAT_EQ(m2(0, 2), 0.f);
    EXPECT_FLOAT_EQ(m2(0, 3), 1.f);
    EXPECT_FLOAT_EQ(m2(1, 0), 0.f);
    EXPECT_FLOAT_EQ(m2(1, 1), 1.f);
    EXPECT_FLOAT_EQ(m2(1, 2), 0.f);
    EXPECT_FLOAT_EQ(m2(1, 3), 1.f);
    EXPECT_FLOAT_EQ(m2(2, 0), 0.f);
    EXPECT_FLOAT_EQ(m2(2, 1), 0.f);
    EXPECT_FLOAT_EQ(m2(2, 2), 1.f);
    EXPECT_FLOAT_EQ(m2(2, 3), 0.f);
    EXPECT_FLOAT_EQ(m2(3, 0), 0.f);
    EXPECT_FLOAT_EQ(m2(3, 1), 0.f);
    EXPECT_FLOAT_EQ(m2(3, 2), 0.f);
    EXPECT_FLOAT_EQ(m2(3, 3), 1.f);

    EXPECT_FLOAT_EQ(m3(0, 0), 1.f);
    EXPECT_FLOAT_EQ(m3(0, 1), 0.f);
    EXPECT_FLOAT_EQ(m3(0, 2), 0.f);
    EXPECT_FLOAT_EQ(m3(0, 3), 0.f);
    EXPECT_FLOAT_EQ(m3(1, 0), 0.f);
    EXPECT_FLOAT_EQ(m3(1, 1), 1.f);
    EXPECT_FLOAT_EQ(m3(1, 2), 0.f);
    EXPECT_FLOAT_EQ(m3(1, 3), 0.f);
    EXPECT_FLOAT_EQ(m3(2, 0), 0.f);
    EXPECT_FLOAT_EQ(m3(2, 1), 0.f);
    EXPECT_FLOAT_EQ(m3(2, 2), 1.f);
    EXPECT_FLOAT_EQ(m3(2, 3), 0.f);
    EXPECT_FLOAT_EQ(m3(3, 0), 0.f);
    EXPECT_FLOAT_EQ(m3(3, 1), 0.f);
    EXPECT_FLOAT_EQ(m3(3, 2), 0.f);
    EXPECT_FLOAT_EQ(m3(3, 3), 1.f);

    EXPECT_FLOAT_EQ(m4(0, 0), 1.f);
    EXPECT_FLOAT_EQ(m4(0, 1), 0.f);
    EXPECT_FLOAT_EQ(m4(0, 2), 0.f);
    EXPECT_FLOAT_EQ(m4(0, 3), 1.f);
    EXPECT_FLOAT_EQ(m4(1, 0), 0.f);
    EXPECT_FLOAT_EQ(m4(1, 1), 1.f);
    EXPECT_FLOAT_EQ(m4(1, 2), 0.f);
    EXPECT_FLOAT_EQ(m4(1, 3), 1.f);
    EXPECT_FLOAT_EQ(m4(2, 0), 0.f);
    EXPECT_FLOAT_EQ(m4(2, 1), 0.f);
    EXPECT_FLOAT_EQ(m4(2, 2), 1.f);
    EXPECT_FLOAT_EQ(m4(2, 3), 0.f);
    EXPECT_FLOAT_EQ(m4(3, 0), 0.f);
    EXPECT_FLOAT_EQ(m4(3, 1), 0.f);
    EXPECT_FLOAT_EQ(m4(3, 2), 0.f);
    EXPECT_FLOAT_EQ(m4(3, 3), 1.f);
}

TEST_F(MatrixTest, OperatorMul)
{
    Matrix m5 = m1 * m2;

    EXPECT_FLOAT_EQ(m5(0, 0), 1.f);
    EXPECT_FLOAT_EQ(m5(0, 1), 0.f);
    EXPECT_FLOAT_EQ(m5(0, 2), 0.f);
    EXPECT_FLOAT_EQ(m5(0, 3), 1.f);
    EXPECT_FLOAT_EQ(m5(1, 0), 0.f);
    EXPECT_FLOAT_EQ(m5(1, 1), 1.f);
    EXPECT_FLOAT_EQ(m5(1, 2), 0.f);
    EXPECT_FLOAT_EQ(m5(1, 3), 1.f);
    EXPECT_FLOAT_EQ(m5(2, 0), 0.f);
    EXPECT_FLOAT_EQ(m5(2, 1), 0.f);
    EXPECT_FLOAT_EQ(m5(2, 2), 1.f);
    EXPECT_FLOAT_EQ(m5(2, 3), 0.f);
    EXPECT_FLOAT_EQ(m5(3, 0), 0.f);
    EXPECT_FLOAT_EQ(m5(3, 1), 0.f);
    EXPECT_FLOAT_EQ(m5(3, 2), 0.f);
    EXPECT_FLOAT_EQ(m5(3, 3), 1.f);

    Matrix m6 = ma * mb;

    EXPECT_FLOAT_EQ(m6(0, 0), 10.f);
    EXPECT_FLOAT_EQ(m6(0, 1), 10.f);
    EXPECT_FLOAT_EQ(m6(0, 2), 10.f);
    EXPECT_FLOAT_EQ(m6(0, 3), 10.f);
    EXPECT_FLOAT_EQ(m6(1, 0), 26.f);
    EXPECT_FLOAT_EQ(m6(1, 1), 26.f);
    EXPECT_FLOAT_EQ(m6(1, 2), 26.f);
    EXPECT_FLOAT_EQ(m6(1, 3), 26.f);
    EXPECT_FLOAT_EQ(m6(2, 0), 42.f);
    EXPECT_FLOAT_EQ(m6(2, 1), 42.f);
    EXPECT_FLOAT_EQ(m6(2, 2), 42.f);
    EXPECT_FLOAT_EQ(m6(2, 3), 42.f);
    EXPECT_FLOAT_EQ(m6(3, 0), 58.f);
    EXPECT_FLOAT_EQ(m6(3, 1), 58.f);
    EXPECT_FLOAT_EQ(m6(3, 2), 58.f);
    EXPECT_FLOAT_EQ(m6(3, 3), 58.f);
}
