#include "VirtualServer.hpp"

void VirtualServer::set_listen(std::string listen) {
	this->listen = listen;
}

void VirtualServer::set_server_name(std::string server_name) {
	this->server_name = server_name;
}

void VirtualServer::set_index(std::string index) {
	this->index = index;
}

void VirtualServer::set_location(Location location) {
	this->locations.push_back(location);
}


void VirtualServer::set_root(std::string root) {
	this->root = root;
}

std::vector<Location> VirtualServer::get_locations() {
	return this->locations;
}
