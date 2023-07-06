#include "cgi.hpp"

Cgi::Cgi(httpReq httpReq)
{}

Cgi::Cgi(const &Cgi src)
{}

Cgi& Cgi::operator=(const &Cgi rhs)
{}

Cgi::~Cgi()
{}

void Cgi::reset_env() {
}

bool Cgi::is_meta_var() {
	std::map<std::string, std::string> envs;

	for (; it != httpreq.end(); it++) {
		if
	}
}

bool Cgi::check_meta_var(string var) {
	// 必須の箇所にデータが入ってるか
	// フォーマットがあるやつはそれを確認

	if (var == "auth_type" || var ==  "content_length" || var ==  "content_type"
		var == "gateway_interface" || var ==  "path_info" || var ==  "path_translated" ||
		var == "query_string" ||  var == "remote_addr" ||  var == "remote_host" || var == "remote_ident", "remote_user", "request_method", "script_name",
							"server_name", "server_port", "server_protocol", "server_software"}
}



// "AUTH_TYPE" | "CONTENT_LENGTH" |
// "CONTENT_TYPE" | "GATEWAY_INTERFACE" |
// "PATH_INFO" | "PATH_TRANSLATED" |
// "QUERY_STRING" | "REMOTE_ADDR" |
// "REMOTE_HOST" | "REMOTE_IDENT" |
// "REMOTE_USER" | "REQUEST_METHOD" |
// "SCRIPT_NAME" | "SERVER_NAME" |
// "SERVER_PORT" | "SERVER_PROTOCOL" |
// "SERVER_SOFTWARE" | scheme |
//  protocol-var-name | extension-var-name"


void Cgi::set_env() {
    // httpreq will have meta var info with map<std::string, std::string>
    //
	std::map<std::string, std::string>::iterator it = httpreq.begin();
	char **env_ptr;
	std::map<std::string, std::string> envs;

	for (; it != httpreq.end(); it++) {
	    if (check_meta_var() {
		    envs[it->first] = it->second;
        }
	}
    //CONTENT_LENGTH is set exist message-body [MUST]
    //CONTENT_TYPE is set exist message-body　セットされていない場合はスクリプトが受信したデータのmime型を決定しようと試みる可能性がある
    //  未知のままであれば、スクリプトは型を application/octet-stream とみなすかもしれないし、誤りとして拒絶するかもしれない
    //  リクエストにCONTENT_TYPEが存在した場合はsetしなければならない [MUST]
    //GATEWAY_INTERFACE is must set value [MUST] CGI/1.1
    //PATH_INFO　文字大小保存 制限を課しても課さなくても良い
    //
    //
    // set sever info meta vars
}

void Cgi::send_body() {
	write(1, httpreq.body.c_str(), body.length());
}

void Cgi::run_handler() {
	execve(script_name, NULL, )
}

void Cgi::fork_process() {
	pid_t pid;
	int fd[2];

	xpipe(fd);
	backup_stdin = xdup(STDIN_FILENO);
	backup_stdout = xdup(STDOUT_FILENO);
	reset_env();
	set_env();

	pid = xfork();
	// 子プロセス
	if (pid == 0) {
		set_signal_handler(SIGINT, SIG_DFL);
		set_signal_handler(SIGQUIT, SIG_DFL);
		xdup2(fd[1], 1);

		run_handler();

		xclose(fd[1]);
		xclose(fd[0]);
		exit(1);
	}
	send_body();
	xdup2(fd[0], 0);
	xclose(fd[1]);
	xclose(fd[0]);
	return pid;
}

void Cgi::run_cgi() {
	int backup_stdin;
	int backup_stdout;

	xdup2(backup_stdin, STDIN_FILENO);
	xdup2(backup_stdout, STDOUT_FILENO);

	fork_process();

	xclose(backup_stdin);
	xclose(backup_stdout);
}
