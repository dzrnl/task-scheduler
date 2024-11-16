#include <lib/TaskScheduler/TaskScheduler.h>

#include <gtest/gtest.h>

#include <cmath>

TEST(TaskSchedulerTestSuite, ReadmeTest) {
    TaskScheduler scheduler;
    std::stringstream ss;

    float a = 1;
    float b = -2;
    float c = 0;

    auto& id1 = scheduler.add([&ss](float a, float c) {
        ss << "1";
        return -4 * a * c;
    }, a, c);

    auto& id2 = scheduler.add([&ss](float b, float v) {
        ss << "2";
        return b * b + v;
    }, b, scheduler.getFutureResult<float>(id1));

    auto& id3 = scheduler.add([&ss](float b, float d) {
        ss << "3";
        return -b + std::sqrt(d);
    }, b, scheduler.getFutureResult<float>(id2));

    auto& id4 = scheduler.add([&ss](float b, float d) {
        ss << "4";
        return -b - std::sqrt(d);
    }, b, scheduler.getFutureResult<float>(id2));

    auto& id5 = scheduler.add([&ss](float a, float v) {
        ss << "5";
        return v / (2 * a);
    }, a, scheduler.getFutureResult<float>(id3));

    auto& id6 = scheduler.add([&ss](float a, float v) {
        ss << "6";
        return v / (2 * a);
    }, a, scheduler.getFutureResult<float>(id4));

    scheduler.executeAll();

    ASSERT_EQ(scheduler.getResult<float>(id5), 2);
    ASSERT_EQ(scheduler.getResult<float>(id6), 0);

    ASSERT_EQ(ss.str(), "123456");
}

TEST(TaskSchedulerTestSuite, OrderTest) {
    TaskScheduler scheduler;
    std::stringstream ss;

    float a = 1;
    float b = -2;
    float c = 0;

    auto& id1 = scheduler.add([&ss](float a, float c) {
        ss << "1";
        return -4 * a * c;
    }, a, c);

    auto& id2 = scheduler.add([&ss](float b, float v) {
        ss << "2";
        return b * b + v;
    }, b, scheduler.getFutureResult<float>(id1));

    auto& id3 = scheduler.add([&ss](float b, float d) {
        ss << "3";
        return -b + std::sqrt(d);
    }, b, scheduler.getFutureResult<float>(id2));

    auto& id4 = scheduler.add([&ss](float b, float d) {
        ss << "4";
        return -b - std::sqrt(d);
    }, b, scheduler.getFutureResult<float>(id2));

    auto& id5 = scheduler.add([&ss](float a, float v) {
        ss << "5";
        return v / (2 * a);
    }, a, scheduler.getFutureResult<float>(id3));

    auto& id6 = scheduler.add([&ss](float a, float v) {
        ss << "6";
        return v / (2 * a);
    }, a, scheduler.getFutureResult<float>(id4));

    scheduler.getResult<float>(id5);

    ASSERT_EQ(ss.str(), "1235");

    scheduler.getResult<float>(id6);

    ASSERT_EQ(ss.str(), "123546");
}

TEST(TaskSchedulerTestSuite, OneArgTest) {
    TaskScheduler scheduler;
    std::stringstream ss;

    float a = 1;

    auto& id1 = scheduler.add([&ss](float a) {
        ss << "1";
        return a * 2;
    }, a);

    auto& id2 = scheduler.add([&ss](float b) {
        ss << "2";
        return b / 2;
    }, scheduler.getFutureResult<float>(id1));

    scheduler.executeAll();

    ASSERT_EQ(scheduler.getResult<float>(id1), 2);
    ASSERT_EQ(scheduler.getResult<float>(id2), 1);

    ASSERT_EQ(ss.str(), "12");
}

TEST(TaskSchedulerTestSuite, ThreeArgsTest) {
    TaskScheduler scheduler;
    std::stringstream ss;

    float a = 1;
    float b = 3;
    float c = 1;

    auto& id1 = scheduler.add([&ss](float a, float b, float c) {
        ss << "1";
        return a + b + c;
    }, a, b, c);

    auto& id2 = scheduler.add([&ss](float d, float b, float c) {
        ss << "2";
        return d * b * c;
    }, scheduler.getFutureResult<float>(id1), b, c);

    scheduler.executeAll();

    ASSERT_EQ(scheduler.getResult<float>(id1), 5);
    ASSERT_EQ(scheduler.getResult<float>(id2), 15);

    ASSERT_EQ(ss.str(), "12");
}

TEST(TaskSchedulerTestSuite, FunctorTest) {
    TaskScheduler scheduler;

    struct Functor {
        float a_;

        Functor(float a) : a_(a) {}

        float operator()(float b) const {
            return a_ * b;
        }
    };

    Functor functor(2);
    int b = 3;

    auto& id1 = scheduler.add(functor, b);

    ASSERT_EQ(scheduler.getResult<float>(id1), 6);
}

TEST(TaskSchedulerTestSuite, IntTypeTest) {
    TaskScheduler scheduler;
    std::stringstream ss;

    int a = 1;
    int b = -2;
    int c = 0;

    auto& id1 = scheduler.add([&ss](int a, int c) {
        ss << "1";
        return -4 * a * c;
    }, a, c);

    auto& id2 = scheduler.add([&ss](int b, int v) {
        ss << "2";
        return b * b + v;
    }, b, scheduler.getFutureResult<int>(id1));

    auto& id3 = scheduler.add([&ss](int b, int d) {
        ss << "3";
        return -b + static_cast<int>(std::sqrt(d));
    }, b, scheduler.getFutureResult<int>(id2));

    auto& id4 = scheduler.add([&ss](int b, int d) {
        ss << "4";
        return -b - static_cast<int>(std::sqrt(d));
    }, b, scheduler.getFutureResult<int>(id2));

    auto& id5 = scheduler.add([&ss](int a, int v) {
        ss << "5";
        return v / (2 * a);
    }, a, scheduler.getFutureResult<int>(id3));

    auto& id6 = scheduler.add([&ss](int a, int v) {
        ss << "6";
        return v / (2 * a);
    }, a, scheduler.getFutureResult<int>(id4));

    ASSERT_EQ(scheduler.getResult<int>(id5), 2);

    ASSERT_EQ(ss.str(), "1235");

    ASSERT_EQ(scheduler.getResult<int>(id6), 0);

    ASSERT_EQ(ss.str(), "123546");
}

TEST(TaskSchedulerTestSuite, StringTypeTest) {
    TaskScheduler scheduler;
    std::stringstream ss;

    std::string a = "A";
    std::string b = "bb";

    auto& id1 = scheduler.add([&ss](const std::string& a, const std::string& b) {
        ss << "1";
        return a + b;
    }, a, b);

    auto& id2 = scheduler.add([&ss](const std::string& a, const std::string& c) {
        ss << "2";
        return a + c;
    }, a, scheduler.getFutureResult<std::string>(id1));

    scheduler.executeAll();

    ASSERT_EQ(scheduler.getResult<std::string>(id2), "AAbb");
}
