#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP


#include <map>

class Vserver{
	private:
		bool is_default;
		std::map<Location> locations;
		std::string server_name;
		std::string root;

	public:
};

#endif
