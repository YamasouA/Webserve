#include "Lcation.hpp"

void set_uri(std::string uri)
{
	this.uri = uri;
}

//void set_method(std::string method)
void set_methods(std::vector<std::string> methods)
{
	this.methods = methods;
}

void set_root(std::string root)
{
	this.root = root;
}

void set_is_autoindex(bool autoindex)
{
	this.autoindex = autoindex;
}

void set_upload_path(std::string upload_path)
{
	this.upload_path = upload_path;
}

void set_index(std::string index)
{
	this.index = index;
}

void set_max_body_size(size_t max_body_size)
{
	this.max_body_size = max_body_size;
}

void set_cgi_path(std::string cgi_path)
{
	this.cgi_path = cgi_path;
}

void set_return(std::string ret)
{
	this.ret = ret;
}

std::string Location::get_uri() const{
	return uri;
}
std::vector<std::string> Location::get_method() const{
	return methods;
}
std::string Location::get_root() const{
	return root;
}
bool Location::get_is_autoindex() const{
	return is_autoindex;
}
std::string Location::get_upload_path() const{
	return upload_path;
}
std::string Location::get_index() const{
	return index;
}
size_t Location::get_max_body_size() const {
	return max_body_size;
}

std::string Location::get_cgi_path() const {
	return cgi_path;
}

std::string Location::get_return() const {
	return ret;
}

std::ostream& operator <<(std::ostream& stream, const Location& obj) {
	return stream << "uri: " << obj.get_uri() << std::endl
			<< "methods: " << obj.get_method() << std::endl
			<< "root: " << obj.get_root() << std::endl
			<< "is_autoindex: " << obj.get_is_autoindex() << std::endl
			<< "upload_path: " << obj.get_upload_path() << std::endl
			<< "index: " << obj.get_index() << std::endl
			<< "max_body_size: " << obj.get_max_body_size() << std::endl
			<< "cgi_path: " << obj.get_cgi_path() << std::endl
			<< "return: " << obj.get_cgi_path() << std::endl;
}
