#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <vector>
#include <iostream>
#include "VirtualServer.hpp"
#include "Location.hpp"

class Config {
	private:
		std::vector<VirtualServer> configs;
		std::string config_txt;
		size_t idx;

		void expect(char c);
		void expect_nl();
		void trim();
		Location get_location();
	public:
		Config(std::string config_txt);
		~Config();
		Config(const Config& source);
		Config& operator =(const Config& source);


	//	std::vector<VirtualServer> get_configs();
		void parse_config();
		std::string get_token(char next_char);
};

//std::ostream &operator << (std::ostream& stream, Location& obj);
#endif
