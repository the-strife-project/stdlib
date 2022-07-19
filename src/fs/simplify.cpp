#include "fs.hpp"

std::string std::simplifyPath(const std::string& path) {
	// Some meme cases
	if(!path.size())
		return "";
	if(path[0] != '/')
		return "";

	// First, remove repeated slashes
	std::string clean;
	bool lastSlash = false;

	for(auto const& x : path) {
		if(x == '/') {
			if(!lastSlash) {
				clean.push_back('/');
				lastSlash = true;
			}
		} else {
			lastSlash = false;
			clean.push_back(x);
		}
	}

	// Now, resolve '.' and '..'
	std::vector<std::string> parts;
	for(auto const& part : clean.split('/')) {
		if(part == ".") {
			// Ignore
		} else if(part == "..") {
			// Take last one back
			if(parts.size() > 1)
				parts.pop_back();
		} else {
			// LGTM
			parts.push_back(part);
		}
	}

	std::string ret;
	for(auto const& x : parts)
		ret += x + "/";

	if(ret.size() > 1)
		ret.pop_back(); // Remove last slash

	return ret;
}
