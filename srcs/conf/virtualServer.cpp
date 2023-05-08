#include "virtualServer.hpp"

void set_listen(std::string listen){
	this.listen = listen;
}
void set_server_name(std::string server_name){
	this.server_name = server_name;
}
void set_index(std::string index){
	this.index = index;
}
void set_location(Location location){
	locations.push_back(location);
}
void set_root(std::string root){
	this.root = root;
}

int get_listen(){
	return listen;
}
std::string get_server_name(){
	return server_name;
}
std::string get_index(){
	return index;
}
std::vector<Location> get_locations(){
	return locations;
}
std::string get_root(){
	return root;
}

std::ostream& operator <<(std::ostream& stream, const virtualServer& obj) {
	return stream << "listen: " << obj.get_listen() << std::endl
		<< "server_name: " << obj.get_server_name() << std::endl
		<< "index: " << obj.get_index() << std::endl
		<< "location: " << obj.get_location() << std::endl
		<< "root: " << obj.get_root() << std::endl;
}
