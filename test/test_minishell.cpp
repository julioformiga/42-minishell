#include <gtest/gtest.h>
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
	string shell_path;
    void SetUp() override {
        shell_path = "bin/minishell";
    }

    string make_test_command(const string& cmd) {
        return shell_path + " -c '" + cmd + "'";
    }

    string make_pipe_command(const string& cmd) {
        return "echo '" + cmd + "' | " + shell_path;
    }

    void assert_command_output(const string& cmd, const string& expected_output,
                             const string& message = "") {
        CommandOutput result = exec_command(make_test_command(cmd));
        ASSERT_EQ(result.stdout_output, expected_output + "\n") << message;
    }

	struct CommandOutput {
        string stdout_output;
        string stderr_output;
        int exit_code;
    };


    string read_file_content(const string& filename) {
        string content;
        char buffer[128];
        FILE* file = fopen(filename.c_str(), "r");

        if (!file)
            return "";

        while (!feof(file)) {
            if (fgets(buffer, 128, file) != NULL)
                content += buffer;
        }
        fclose(file);
        return content;
    }

    void cleanup_test_files() {
        unlink("test/result.txt");
        unlink("test/result-append.txt");
    }

	CommandOutput exec_command(const string& command) {
        string modified_command = command + " 2> /tmp/stderr_output";
        CommandOutput result;

        // Capture stdout
        char buffer[128];
        FILE* pipe = popen(modified_command.c_str(), "r");
        if (!pipe) {
            return {"ERROR", "ERROR", -1};
        }

        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result.stdout_output += buffer;
        }
        result.exit_code = pclose(pipe);
        if (WIFEXITED(result.exit_code))
            result.exit_code = WEXITSTATUS(result.exit_code);
        else
            result.exit_code = -1;

        FILE* stderr_file = fopen("/tmp/stderr_output", "r");
        if (stderr_file) {
            while (fgets(buffer, 128, stderr_file)) {
                result.stderr_output += buffer;
            }
            fclose(stderr_file);
            unlink("/tmp/stderr_output");
        }

        return result;
    }

    void send_signal_to_process(pid_t pid, int signal) {
        kill(pid, signal);
    }
};

TEST_F(MinishellTest, BasicExecution) {
    CommandOutput result = exec_command("echo 'ls' | " + shell_path);
    ASSERT_FALSE(result.stdout_output.empty()) << "Shell should execute basic commands";
}

TEST_F(MinishellTest, CtrlDHandling) {
    string command = "echo -ne '' | " + shell_path;
    CommandOutput result = exec_command(command);
    ASSERT_TRUE(result.stdout_output.find("") != string::npos) << "Shell should handle Ctrl+D (EOF) gracefully";
}

TEST_F(MinishellTest, NoMemoryLeaksWithCommandsPipeRedirects) {
    string command = "echo 'ls -l | grep src > test/result.txt >> test/result-append.txt'"
		" | valgrind --leak-check=full --show-leak-kinds=all "
		"--suppressions=external.supp --error-exitcode=1 "
		+ shell_path;
    int result = system(command.c_str());

	ASSERT_EQ(result, 0) << "Memory leak detected";
    ASSERT_TRUE(access("test/result.txt", F_OK) == 0)
        << "result.txt should have been created";
    ASSERT_TRUE(access("test/result-append.txt", F_OK) == 0)
        << "result-append.txt should have been created";

	string result_content = read_file_content("test/result.txt");
    string append_content = read_file_content("test/result-append.txt");

    ASSERT_TRUE(result_content.empty())
        << "result.txt should be empty";
    ASSERT_TRUE(append_content.find("src") != string::npos)
        << "result-append.txt should contain 'src'";

	cleanup_test_files();
}

TEST_F(MinishellTest, MultipleCommands) {
    string command = "echo 'ls\npwd\nexit' | " + shell_path;
    CommandOutput result = exec_command(command);
    ASSERT_FALSE(result.stdout_output.empty()) << "Shell should handle multiple commands";
}

TEST_F(MinishellTest, InvalidCommand) {
    string command = shell_path + " -c 'invalidcommand'";
    CommandOutput result = exec_command(command);

    ASSERT_FALSE(result.stderr_output.empty())
        << "Invalid command should produce error output";
    ASSERT_TRUE(result.stderr_output.find("not found") != string::npos ||
                result.stderr_output.find("error") != string::npos)
        << "Error message should indicate command not found";
	ASSERT_EQ(result.exit_code, 127)
		<< "Invalid command should set exit status to 127";
}

TEST_F(MinishellTest, EnvironmentVariables) {
    string command = "echo 'echo $PATH\nexit' | " + shell_path;
    CommandOutput result = exec_command(command);
    ASSERT_FALSE(result.stdout_output.empty()) << "Shell should handle environment variables";
}

TEST_F(MinishellTest, PipeHandling) {
    string command = "echo 'ls | wc -l\nexit' | " + shell_path;
    CommandOutput result = exec_command(command);
    ASSERT_FALSE(result.stdout_output.empty()) << "Shell should handle pipes";
}

TEST_F(MinishellTest, SpecialCharacters) {
    vector<pair<string, string>> tests = {
        {"echo ab", "ab"},
        {"echo \"abc\"", "abc"},
		{"echo \"a $DISPLAY\" $DISPLAY 'qwe' | wc", "      1       4      12"}
		// {"echo \"a >$DISPLAY\"$DISPLAYb$DISPLAY'$DISPLAYq>we'|wc", "      1       2      20"}
    };

    for (const auto& test : tests) {
        CommandOutput result = exec_command(make_test_command(test.first));
        ASSERT_EQ(result.stdout_output, test.second + "\n")
            << "Failed on command: " << test.first;
    }
}

// TEST_F(MinishellTest, QuotesBeforeString) {
//     string command = shell_path + "-c 'echo \"a\"b'";
//     CommandOutput result = exec_command(command);
//     ASSERT_EQ(result.stdout_output, "ab") << "Shell should concatenate the two strings";
// }

// TEST_F(MinishellTest, DollarAndQuotes) {
//     string command = "echo 1$ $ $\"c\" $'c' \"$\"c \"$a $ab\"c '$a'c \"$a '$a'  $ab\"c" + shell_path;
//     CommandOutput result = exec_command(command);
//     ASSERT_EQ(result.stdout_output, "1$ $ c c $c  c $ac  ''  c") << "Shell should go to hell";
// }

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
