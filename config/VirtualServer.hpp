#ifndef VIRTUALSERVER_HPP
#define VIRTUALSERVER_HPP

#include <vector>
#include "Location.hpp"

class VirtualServer{
	private:
		std::string listen;
		std::string index;
		//bool is_default;
		std::vector<Location> locations;
		std::string server_name;
		std::string root;

	public:
		void parse_vserv();
		void set_listen(std::string listen);
		void set_server_name(std::string server_name);
		void set_index(std::string index);
		void set_location(Location location);
		void set_root(std::string root);
		std::vector<Location> get_locations();

		//void get_listen(std::string listen);
		//void get_server_name(std::string server_name);
		//void get_index(std::string index);
		//void get_location(std::string location);
};

//std::ostream &operator << (std::ostream& stream, const Location& obj);
#endif
