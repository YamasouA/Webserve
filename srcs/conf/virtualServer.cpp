#include "virtualServer.hpp"

virtualServer::virtualServer()
{}

virtualServer::virtualServer(const virtualServer& src)
{
	(void)src;
}

virtualServer& virtualServer::operator=(const virtualServer& rhs)
{
	if (this == &rhs) {
		return *this;
	}
	return *this;
}

virtualServer::~virtualServer()
{}

void virtualServer::set_listen(std::string listen){
	this->listen = listen;
}
void virtualServer::set_server_name(std::string server_name){
	this->server_name = server_name;
}
//void virtualServer::set_index(std::string index){
//	this->index = index;
//}

void virtualServer::set_location(Location location){
	locations.push_back(location);
}
void virtualServer::set_root(std::string root){
	this->root = root;
}

//int virtualServer::get_listen(){
std::string virtualServer::get_listen() const{
	return listen;
}
std::string virtualServer::get_server_name() const{
	return server_name;
}
//std::string virtualServer::get_index(){
//	return index;
//}
std::vector<Location> virtualServer::get_locations() const{
	return locations;
}
std::string virtualServer::get_root() const{
	return root;
}

std::ostream& operator <<(std::ostream& stream, const virtualServer& obj) {
		const std::vector<Location> tmp = obj.get_locations();
		stream << "listen: " << obj.get_listen() << std::endl
		<< "server_name: " << obj.get_server_name() << std::endl
//		<< "index: " << obj.get_index() << std::endl
		<< "locations:" << std::endl;
		for (std::vector<Location>::const_iterator it = tmp.begin(); it != tmp.end(); ++it) {
			stream << "location: " << *it << std::endl;
		}
		stream << std::endl
		<< "root: " << obj.get_root() << std::endl;
		return stream;
}
