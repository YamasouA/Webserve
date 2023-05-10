#ifndef VIRTUALSERVER_HPP
#define VIRTUALSERVER_HPP

#include <vector>
#include <string>
#include "Location.hpp"

class virtualServer {
	public:
		virtualServer();
		virtualServer(const virtualServer& src);
		virtualServer& operator=(const virtualServer& rhs);
		~virtualServer();

		void parse_vserv();
		//void set_listen(std::string listen);
		void set_listen(int listen);
		void set_server_name(std::string server_name);
//		void set_index(std::string index);
		void set_location(Location location);
//		void set_root(std::string root);
		std::vector<Location> get_locations() const;
		//std::string get_listen() const;
		int get_listen() const;
		std::string get_server_name() const;
//		std::string get_index();
//		std::string get_location() const;
//		std::string get_root() const;
	private:
		int listen;
//		std::string listen;
		std::string server_name;
//		std::string root;
		std::vector<Location> locations;
		std::string path;
		std::string error_page;
};

std::ostream& operator <<(std::ostream& stream, const virtualServer& obj);
#endif
