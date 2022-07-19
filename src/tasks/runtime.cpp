#include "tasks.hpp"

std::RuntimeV std::Runtime::parse() {
	std::RuntimeV ret;

	char* cur = (char*)raw;
	size_t emptyCount = 0;
	while(emptyCount != 2) {
		std::string str(cur);

		if(str.size()) {
			if(emptyCount == 0) {
				ret.args.push_back(str);
			} else {
				// Where's the '='?
				char* aux = str.c_str();
				size_t pos = 0;
				while(pos < str.size() && aux[pos] != '=')
					++pos;
				aux[pos] = 0;

				std::string name(aux);
				std::string value(aux + pos + 1);
				ret.env[name] = value;
			}
		} else {
			++emptyCount;
		}

		cur += str.size() + 1;
	}

	return ret;
}
