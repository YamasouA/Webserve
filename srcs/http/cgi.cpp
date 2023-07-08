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

bool Cgi::check_meta_var(std::string var1, std::string var2) {
	// 必須の箇所にデータが入ってるか
	// フォーマットがあるやつはそれを確認

	if (var1 == "auth_type" || var1 ==  "content_length" || var1 ==  "content_type"
		var1 == "gateway_interface" || var1 ==  "path_info" || var1 ==  "path_translated" ||
		var1 == "query_string" ||  var1 == "remote_addr" ||  var1 == "remote_host" ||
		var1 == "remote_ident" || var1 == "remote_user" || var1 ==  "request_method" ||
		var1 ==  "script_name" || var1 == "server_name" || var1 == "server_port" ||
		var1 == "server_protocol" || var1 == "server_software") {
			if (var2 != "") {
				return true;
			}
	}
	return false;
}

std::string Cgi::encode_uri() {
	std::ostringstream rets;
	for(size_t n = 0; n < url.size(); n++) {
	  unsigned char c = (unsigned char)url[n];
	  if (isalnum(c) || c == '_' || c == '.' || c == '/' )
	    rets << c;
	  else {
	    char buf[8];
	    sprintf(buf, "%02x", (int)c);
	    rets << '%' << buf[0] << buf[1];
	  }
	}
	return rets.str();
}


void Cgi::fix_up() {
    //if exist message-body, must set CONTENT_LENGTH value [MUST]
	if (envs.count("content_length") == 0 && httpreq.getContentBody()) {
		throw new Error();
	}
    if (envs.count("content_type") == 0 && httpreq.getContentBody()) {
        throw new Error();
    }
    if (envs.count("gateway_interface") == 0 && envs["gateway_interface"] == "CGI/1.1") {
        throw new Error();
    }
    if (envs.count("path-info") == 0) {
        throw new Error();
    }
    if (envs.count("path_translated") == 0) {
        throw new Error();
    }
    if (envs.count("remote_addr") == 0) {
        throw new Error();
    }
    if (envs.count("remote_host") == 0) {
        throw new Error();
    }
    if (envs.count("request_method") == 0) {
        throw new Error();
    }
    if (envs.count("request_name") == 0) {
        throw new Error();
    }
    if (envs.count("script_name") == 0) {
        throw new Error();
    }
    if (envs.count("server_name") == 0) {
        throw new Error();
    }
    if (envs.count("server_port") == 0) {
        throw new Error();
    }
    if (envs.count("server_protocol") == 0) {
        throw new Error();
    }
    if (envs.count("server_software") == 0) {
        throw new Error();
    }

    //if exist message-body, must set CONTENT_TYPE value　セットされていない場合はスクリプトが受信したデータのmime型を決定しようと試みる可能性がある
    //  未知のままであれば、スクリプトは型を application/octet-stream とみなすかもしれないし、誤りとして拒絶するかもしれない
    //  リクエストにCONTENT_TYPEが存在した場合はsetしなければならない [MUST]
    //GATEWAY_INTERFACE is must set value [MUST] CGI/1.1

	envs["gateway_interface"] = "CGI/1.1"; // move before cgi

    // PATH_INFO　文字大小保存 制限を課しても課さなくても良い
    // PATH_TRANSLATED  QUERY_STRINGとかこの辺りはhttpreqで処理した方が良さそう？
    // REMOTE_ADDR [MUST] acceptの第２引数で取得できそう？
    // REMOTE_HOST(完全修飾ドメイン名) [SHOULD] 多分hostname
	envs.erase("HTTP_HOST");
    // REMOTE_IDENT [MAY]
    // REMOTE_USER http認証をclientが求めている場合は[MUST]
    // REQUEST_METHOD [MUST] 文字大小区別
	envs["REQUEST_METHOD"] = getMethod();
    // SCRIPT_NAME [MUST] CGIスクリプトを識別することができる(URL符号化されていない)URL path
    //  pahtがNULLの場合は値は省略可能だが変数はセットしなければならない PATH_INFO部はまったく含まれない
    //
    // SERVER_NAME [MUST] 文字大小を区別しないhostnameまたはネットワークアドレス
    // SERVER_PORT [MUST] clientからリクエストを受信したTCP/IP port番号
    // SERVER_PROTOCOL [MUST] CGIリクエストに使用されるアプリケーションプロトコルの名前とバーション。clientとの通信でserverが使用するプロトコルの
    //  バージョンと違ってもよい[MAY]
    //
    // SERVER_SOFTWARE [MUST] CGIリクエストを行い、ゲートウェイを実行するサーバーソフトウェアの名前とバージョン clientに報告されたサーバーの説明があれば
    //  それと同じであるべき [SHOULD]
    //
    //
    //
    // set sever info meta vars
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
	std::map<std::string, std::string> envs = httpreq.get_cgi_envs();;

    // Protocol-Specific Meta-Variables [MUST] 名前がHTTP_で始まるメタ変数は使用されるプロトコルがHTTPであればclient request header filedから読んだ値
    //  を含む
    //  HTTPヘッダフィールド名は全て大文字に変換し、'-'は'_'に変換し、頭に"HTTP"を付けてメタ変数とする。同じ名前のヘッダフィールドが複数受信された場合、
    //  サーバーは同じ意味の一つの値に書き換えなければならない [MUST] サーバーは必要であればデータ表現(文字集合など)をCGIメタ変数で適切なように変更
    //  しなければならない [MUST]
    //  サーバーは受信した全てのヘッダフィールドのメタ変数を作成する必要はない。特に認証情報を伝搬するもの(Authorizationなど)や他のメタ変数が
    //  スクリプトから利用可能なもの(Content-Type、Content-Length)は削除すべき [SHOULD]
    //  サーバーはConnectionヘッダフィールドなどのクライアントとの通信に関係するだけのヘッダフィールドを削除してもよい [MAY]
	for (; it != httpreq.end(); it++) {
		std::string envs_var = "HTTP_";
		std::string http_req_field;
		std::transform(it->first.begin(), it->first.end(), http_req_field.begin(), ::toupper);
		std::replace(http_req_field.begin(), http_req_field.end(), '-', '_');
		envs_var += http_req_field;
		envs[envs_var] = it->second;
	}

	fix_up();
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
