#ifndef CONFIGPASER_HPP
#define CONFIGPASER_HPP

#include "virtualServer.hpp"
#include "Location.hpp"

class configPaser {
	public:
		explicit configPaser(const std::string& strs);
		configPaser(const configPaser& src);
		configPaser& operator=(const configPaser& rhs);
		~configPaser();
	private:
		std::string buf;
		std::vector<virtualServer> serve_confs;
		size_t idx;
};

#endif
