#ifndef _STDLIB_USERSPACE_PCI_HPP
#define _STDLIB_USERSPACE_PCI_HPP

#include <types>

namespace std {
	struct PCI {
		static const uint32_t BAD_DEVICE = ~0u;
		static const uint32_t BAD_BAR = 0;

		enum {
			GET_DEVICE,
			GET_BAR,
			DO_MSI,
			ENABLE_MMIO,
			BECOME_BUSMASTER,
		};

		enum {
			OK,
			NO_CAPABILITIES,
			NO_MSI_OR_MSIX,
			MSIX_UNSUPPORTED,
		};
	};
};

#endif
