#ifndef VIRTUALSERVER_HPP
#define VIRTUALSERVER_HPP

#include <vector>
#include <string>
#include "Location.hpp"

class virtualServer {
	public:
		void parse_vserv();
		void set_listen(std::string listen);
		void set_server_name(std::string server_name);
		void set_index(std::string index);
		void set_location(Location location);
		void set_root(std::string root);
		std::vector<Location> get_locations();
		void get_listen();
		void get_server_name();
		void get_index();
		void get_location();
		void get_root();
	private:
		int listen;
		std::string server_name;
		std::string path;
		std::string error_page;
		std::vector<Location> locations;
};

std::ostream& operator <<(std::ostream& stream, const virtualServer& obj) {
#endif
