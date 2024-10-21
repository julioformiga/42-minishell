#include <gtest/gtest.h>
#include <iostream>
#include <unistd.h>
#include <libgen.h>

using namespace std;

TEST(minishell, dumb_test) {
    cout << "Test 1" << endl;
    ASSERT_TRUE(true);
}

TEST(minishell, exit_without_mleaks) {
    const char* command = "echo 'ls\nexit' | valgrind --leak-check=full --show-leak-kinds=all --suppressions=external.supp --error-exitcode=1 bin/minishell";
    int result = system(command);
    ASSERT_EQ(result, 0) << "Memory leak detected or error occurred";
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
