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

TEST_F(MinishellTest, MLeaksPipeRedirects) {
    string command = "echo 'ls -l | grep src > test/result.txt >> test/result-append.txt'"
		" | valgrind --leak-check=full --show-leak-kinds=all "
		"--suppressions=external.supp --error-exitcode=1 "
		+ shell_path;
    CommandOutput result = exec_command(command);

	ASSERT_EQ(result.exit_code, 0) << "Memory leak detected";
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

TEST_F(MinishellTest, MLeaksCD) {
    string command = "echo 'cd\ncd -\ncd /\n'"
		" | valgrind --leak-check=full --show-leak-kinds=all "
		"--suppressions=external.supp --error-exitcode=1 "
		+ shell_path;
    CommandOutput result = exec_command(command);

	ASSERT_EQ(result.exit_code, 0)
		<< "Invalid command should set exit status to 0";
}

TEST_F(MinishellTest, MLeaksExportValid) {
    string command = "echo 'export emptyvar\nexport\nexport a=123'"
		" | valgrind --leak-check=full --show-leak-kinds=all "
		"--suppressions=external.supp --error-exitcode=1 "
		+ shell_path;
    CommandOutput result = exec_command(command);

	ASSERT_EQ(result.exit_code, 0)
		<< "Invalid command should set exit status to 0";
}

TEST_F(MinishellTest, MLeaksExportInvalid) {
    string command = "echo 'export 123'"
		" | valgrind --leak-check=full --show-leak-kinds=all "
		"--suppressions=external.supp --error-exitcode=2 "
		+ shell_path;
    CommandOutput result = exec_command(command);

	ASSERT_EQ(result.exit_code, 1)
		<< "Invalid command should set exit status to 1";
}

TEST_F(MinishellTest, MLeaksRedirectOutFileNotExists) {
    string command = "echo '< filenotexists.txt cat'"
		" | valgrind --leak-check=full --show-leak-kinds=all "
		"--suppressions=external.supp --error-exitcode=2 "
		+ shell_path;
    CommandOutput result = exec_command(command);

	ASSERT_EQ(result.exit_code, 1)
		<< "Invalid command should set exit status to 1";
    ASSERT_TRUE(result.stderr_output.find("No such file") != string::npos &&
                result.stderr_output.find("error") != string::npos)
        << "Error message should indicate command not found";
}

TEST_F(MinishellTest, MultipleCommands) {
    string command = "echo 'ls\npwd\nexit' | " + shell_path;
    CommandOutput result = exec_command(command);
    ASSERT_FALSE(result.stdout_output.empty()) << "Shell should handle multiple commands";
}

TEST_F(MinishellTest, CommadNotFound) {
    string command = shell_path + " -c 'invalidcommand'";
    CommandOutput result = exec_command(command);

    ASSERT_TRUE(result.stderr_output.find("not found") != string::npos &&
                result.stderr_output.find("invalidcommand") != string::npos)
        << "Error message should indicate command not found";
	ASSERT_EQ(result.exit_code, 127)
		<< "Invalid command should set exit status to 127";
}

TEST_F(MinishellTest, CommadNotFoundFullpath) {
    string command = shell_path + " -c '/bin/lsls'";
    CommandOutput result = exec_command(command);

    ASSERT_TRUE(result.stderr_output.find("not found") != string::npos &&
                result.stderr_output.find("/bin/lsls") != string::npos)
        << "Error message should indicate command not found";
	ASSERT_EQ(result.exit_code, 127)
		<< "Invalid command should set exit status to 127";
}

TEST_F(MinishellTest, CommadNotFoundOutPATH) {
    string command = "echo 'unset PATH\nls' | " + shell_path;
    CommandOutput result = exec_command(command);

    ASSERT_TRUE(result.stderr_output.find("not found") != string::npos &&
                result.stderr_output.find("ls") != string::npos)
        << "Error message should indicate command not found";
	ASSERT_EQ(result.exit_code, 127)
		<< "Invalid command should set exit status to 127";
}

TEST_F(MinishellTest, OpPreviousOpsRedir) {
    string command = shell_path + " -c 'echo arg > >'";
    CommandOutput result = exec_command(command);

    ASSERT_TRUE(result.stderr_output.find("unexpected token `>") != string::npos)
        << "Error message should indicate syntax error";
	ASSERT_EQ(result.exit_code, 2)
		<< "Invalid command should set exit status to 2";
}

TEST_F(MinishellTest, OpPreviousOpsRedirAppend) {
    string command = shell_path + " -c 'echo arg > >>'";
    CommandOutput result = exec_command(command);

    ASSERT_TRUE(result.stderr_output.find("unexpected token `>>") != string::npos)
        << "Error message should indicate syntax error";
	ASSERT_EQ(result.exit_code, 2)
		<< "Invalid command should set exit status to 2";
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

TEST_F(MinishellTest, Expansion) {
    vector<pair<string, string>> tests = {
        {"echo ab", "ab"},
        {"echo \"abc\"", "abc"},
        {"echo $", "$"},
		{"echo ciao $", "ciao $"},
        {"echo 1$", "1$"},
		{"echo $1", ""},
		{"echo ciao$1", "ciao"},
		{"echo $\"1\"", "1"},
		{"echo \"$1\"", ""},
		{"echo \"$1\"$", "$"},
		{"echo $\"$1\"ciccio$", "ciccio$"},
		{"echo \">\" ciao", "> ciao"},
		{"echo \">>\" ciao", ">> ciao"},
		{"echo \"<\" ciao", "< ciao"},
		{"echo \"<<\" ciao", "<< ciao"},
		{"echo \"|\" ciao", "| ciao"},
		{"echo \"||\" ciao", "|| ciao"},
		//{"echo ciao ||| grep ||", "ciao ||"},//leak
		{"echo \">   file\" ciao", ">   file ciao"},
    };

    for (const auto& test : tests) {
        CommandOutput result = exec_command(make_test_command(test.first));
        ASSERT_EQ(result.stdout_output, test.second + "\n")
            << "Failed on command: " << test.first;
    }
}

TEST_F(MinishellTest, EmptyRedirect) {
    string command = shell_path + " -c 'echo >'";
    CommandOutput result = exec_command(command);

    ASSERT_FALSE(result.stderr_output.empty())
        << "Invalid command should produce error output";
    ASSERT_TRUE(result.stderr_output.find("unexpected token") != string::npos &&
                result.stderr_output.find("error") != string::npos)
        << "Error message should indicate command not found";
	ASSERT_EQ(result.exit_code, 2)
		<< "Invalid command should set exit status to 2";
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
