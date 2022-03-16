#ifndef _STDLIB_LOADER_HPP
#define _STDLIB_LOADER_HPP

namespace std {
	namespace Loader {
		struct Error {
			enum {
				NONE,
				NO_MEMORY,
				NOT_ELF,
				NOT_64,
				NOT_LE,
				BAD_ARCH,
				INVALID_OFFSET,
				NO_PHDRS,
				NO_SECTIONS,
				NO_SHSTRTAB,
				NO_DYNSTR,
				UNSUPPORTED_RELOCATION,
				FAILED_RELOCATION
			};
		};
	};
};

#endif
