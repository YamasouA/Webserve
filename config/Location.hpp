#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <vector>
#include <string>
#include <iostream>
#include <ostream>
/*
typedef enum {
	PERFECT,
	REG1,
	REG2,
	HEAD_MATCH,
	NO_PREFIX,
} PREFIX;
*/
class Location {
	private:
		std::string uri;
		std::vector<std::string> methods;
		std::string root;
		bool is_autoindex;
		std::string upload_path;
		std::string index;
		size_t max_body_size;
		std::string cgi_path;
//		PREFIX prefix;
	public:
		//Location();
		//~Location();
		//Location(const Location& source);
		//Location& operator =(const Location& source);
		void set_uri(std::string uri);
		void set_method(std::string method);
		void set_root(std::string root);
		void set_is_autoindex(bool autoindex);
		void set_upload_path(std::string upload_path);
		void set_index(std::string index);
		void set_max_body_size(size_t max_body_size);
		void set_cgi_path(std::string cgi_path);
		std::string get_uri() const;
		std::vector<std::string> get_method() const;
		std::string get_root() const;
		bool get_is_autoindex() const;
		std::string get_upload_path() const;
		std::string get_index() const;
		size_t get_max_body_size() const;
		std::string get_cgi_path() const;
};

std::ostream &operator << (std::ostream& stream, const Location& obj);

#endif
