#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <vector>
#include <iostream>

class Location {
	public:
		void set_uri(std::string uri);
//		void set_methods(std::string method);
		void set_methods(std::vector<std::string> methods);
		void set_root(std::string root);
		void set_is_autoindex(bool autoindex);
		void set_upload_path(std::string upload_path);
		void set_index(std::string index);
		void set_max_body_size(size_t max_body_size);
		void set_cgi_path(std::string cgi_path);
		void set_return(std::string ret);
		void set_location(Location location);
		void set_depth(int depth);
		void set_alias(std::string alias);
		std::vector<Location> get_locations() const;
		std::string get_uri() const;
		std::vector<std::string> get_methods() const;
		std::string get_root() const;
		bool get_is_autoindex() const;
		std::string get_upload_path() const;
		std::string get_index() const;
		size_t get_max_body_size() const;
		std::string get_cgi_path() const;
		std::string get_return() const;
		int get_depth() const;
		std::string get_alias() const;
		Location(const Location& src);
		Location();
		Location& operator=(const Location& src);
	private:
		std::string uri;
		std::string root;
		std::string index;
		std::string ret;
		std::vector<std::string> methods;
		std::string upload_path;
		std::string cgi_path;
		int depth;
		std::string alias;
		bool autoindex;
		size_t max_body_size;
		std::vector<Location> locations;
};

std::ostream& operator <<(std::ostream& stream, const Location& obj);

#endif
