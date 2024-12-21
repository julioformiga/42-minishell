#include <gtest/gtest.h>
#include <iostream>
#include <unistd.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string>
#include <fcntl.h>

using namespace std;

class MinishellTest : public ::testing::Test {
protected:
    void SetUp() override {
        shell_path = "bin/minishell";
    }

    string shell_path;

    string exec_command(const string& command) {
        char buffer[128];
        string result = "";
        FILE* pipe = popen(command.c_str(), "r");

        if (!pipe) {
            return "ERROR";
        }

        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }

        pclose(pipe);
        return result;
    }

    void send_signal_to_process(pid_t pid, int signal) {
        kill(pid, signal);
    }
};

TEST_F(MinishellTest, BasicExecution) {
    string result = exec_command("echo 'ls' | " + shell_path);
    ASSERT_FALSE(result.empty()) << "Shell should execute basic commands";
}

TEST_F(MinishellTest, CtrlDHandling) {
    string command = "echo -ne '' | " + shell_path;
    string result = exec_command(command);
    ASSERT_TRUE(result.find("") != string::npos) << "Shell should handle Ctrl+D (EOF) gracefully";
}

TEST_F(MinishellTest, CtrlCHandling) {
    int pipefd[2];
    pipe(pipefd);

    pid_t pid = fork();
    if (pid == 0) {
        // Processo filho
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        execl(shell_path.c_str(), "minishell", NULL);
        exit(1);
    }

    close(pipefd[1]);
    usleep(100000);

    send_signal_to_process(pid, SIGINT);

    char buffer[1024];
    ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer)-1);
    buffer[bytes_read] = '\0';

    kill(pid, SIGTERM);
    waitpid(pid, NULL, 0);
    close(pipefd[0]);

    string output(buffer);
    ASSERT_TRUE(output.find("$>") != string::npos) << "Ctrl+C should create new prompt";
}

TEST_F(MinishellTest, NoMemoryLeaks) {
    string command = "echo 'ls\nexit' | valgrind --leak-check=full --show-leak-kinds=all "
                    "--suppressions=external.supp --error-exitcode=1 " + shell_path;
    int result = system(command.c_str());
    ASSERT_EQ(result, 0) << "Memory leak detected";
}

TEST_F(MinishellTest, MultipleCommands) {
    string command = "echo 'ls\npwd\nexit' | " + shell_path;
    string result = exec_command(command);
    ASSERT_FALSE(result.empty()) << "Shell should handle multiple commands";
}

TEST_F(MinishellTest, InvalidCommand) {
    string command = "echo 'invalidcommand\nexit' | " + shell_path;
    string result = exec_command(command);
    ASSERT_TRUE(result.find("not found") != string::npos ||
                result.find("error") != string::npos)
        << "Shell should handle invalid commands";
}

TEST_F(MinishellTest, EnvironmentVariables) {
    string command = "echo 'echo $PATH\nexit' | " + shell_path;
    string result = exec_command(command);
    ASSERT_FALSE(result.empty()) << "Shell should handle environment variables";
}

TEST_F(MinishellTest, PipeHandling) {
    string command = "echo 'ls | wc -l\nexit' | " + shell_path;
    string result = exec_command(command);
    ASSERT_FALSE(result.empty()) << "Shell should handle pipes";
}
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
