 // Native C++ test framework header
#ifndef TEST_FRAMEWORK_HPP
#define TEST_FRAMEWORK_HPP

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <sstream>
#include <stdexcept>

class TestFramework {
private:
    struct TestCase {
        std::string name;
        std::function<void()> test;
    };

    static std::vector<TestCase>& getTests() {
        static std::vector<TestCase> tests;
        return tests;
    }

public:
    static int& getCurrentTestFailures() {
        static int failures = 0;
        return failures;
    }


    static void markFailure() {
        getCurrentTestFailures()++;
    }

    static void registerTest(const std::string& name, std::function<void()> test) {
        getTests().push_back({name, test});
    }

    static int runAllTests() {
        int passed = 0;
        int failed = 0;
        int totalExpectationFailures = 0;

        std::cout << "\n========== Running Tests ==========\n" << std::endl;

        for (const auto& test : getTests()) {
            getCurrentTestFailures() = 0;

            try {
                test.test();


                if (getCurrentTestFailures() > 0) {
                    std::cout << "✗ " << test.name << std::endl;
                    std::cout << "  Expectation failures: "
                              << getCurrentTestFailures() << std::endl;
                    totalExpectationFailures += getCurrentTestFailures();
                    failed++;
                } else {
                    std::cout << "✓ " << test.name << std::endl;
                    passed++;
                }
            } catch (const std::exception& e) {
                std::cout << "✗ " << test.name << std::endl;
                std::cout << "  Error: " << e.what() << std::endl;


                totalExpectationFailures += getCurrentTestFailures();
                failed++;
            } catch (...) {
                std::cout << "✗ " << test.name << std::endl;
                std::cout << "  Error: Unknown exception" << std::endl;


                totalExpectationFailures += getCurrentTestFailures();
                failed++;
            }
        }

        std::cout << "\n==================================\n";
        std::cout << "Tests passed: " << passed << "\n";
        std::cout << "Tests failed: " << failed << "\n";
        std::cout << "Total tests:  " << (passed + failed) << "\n";
        std::cout << "Expectation failures: " << totalExpectationFailures << "\n";

        std::cout << "==================================\n" << std::endl;

        return (failed == 0) ? 0 : 1;
    }
};


#define ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            throw std::runtime_error(std::string("Assertion failed: ") + #condition); \
        } \
    } while (false)


#define ASSERT_FALSE(condition) \
    do { \
        if ((condition)) { \
            throw std::runtime_error(std::string("Assertion failed: !") + #condition); \
        } \
    } while (false)


#define ASSERT_EQ(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            std::ostringstream oss; \
            oss << "Expected: " << (expected) << ", Got: " << (actual); \
            throw std::runtime_error(oss.str()); \
        } \
    } while (false)


#define ASSERT_NE(val1, val2) \
    do { \
        if ((val1) == (val2)) { \
            throw std::runtime_error(std::string("Values should not be equal")); \
        } \
    } while (false)

#define ASSERT_THROW(statement) \
    do { \
        bool exception_thrown = false; \
        try { \
            statement; \
        } catch (const std::exception&) { \
            exception_thrown = true; \
        } \
        if (!exception_thrown) { \
            throw std::runtime_error("Expected exception but none was thrown"); \
        } \
    } while (false)


#define ASSERT_NO_THROW(statement) \
    do { \
        try { \
            statement; \
        } catch (const std::exception& e) { \
            throw std::runtime_error(std::string("Unexpected exception: ") + e.what()); \
        } \
    } while (false)


#define EXPECT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            std::cerr << "  Warning: Expectation failed: " << #condition << std::endl; \
            TestFramework::markFailure(); \
        } \
    } while (false)


#define EXPECT_FALSE(condition) \
    do { \
        if ((condition)) { \
            std::cerr << "  Warning: Expectation failed: !" << #condition << std::endl; \
            TestFramework::markFailure(); \
        } \
    } while (false)


#define EXPECT_EQ(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            std::cerr << "  Warning: Expected: " << (expected) \
                      << ", Got: " << (actual) << std::endl; \
            TestFramework::markFailure(); \
        } \
    } while (false)


#define EXPECT_NE(val1, val2) \
    do { \
        if ((val1) == (val2)) { \
            std::cerr << "  Warning: Values should not be equal" << std::endl; \
            TestFramework::markFailure(); \
        } \
    } while (false)

#define TEST(testName) \
    void test_##testName(); \
    struct Register_##testName { \
        Register_##testName() { \
            TestFramework::registerTest(#testName, test_##testName); \
        } \
    } register_##testName; \
    void test_##testName()

#endif // TEST_FRAMEWORK_HPP
