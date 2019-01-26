#include <gtest/gtest.h>

#include "../LogStream.h"

using muduo::string;

TEST(LogStreamTest, testLogStreamBooleans) {
    muduo::LogStream os;
    const muduo::LogStream::Buffer& buf = os.buffer();
    EXPECT_EQ(buf.toString(), string(""));
    os<<true;
    EXPECT_EQ(buf.toString(), string("1"));
    os<<'\n';
    EXPECT_EQ(buf.toString(), string("1\n"));
    os<< false;
    EXPECT_EQ(buf.toString(), string("1\n0"));
}