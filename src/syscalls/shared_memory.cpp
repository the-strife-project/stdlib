#include <shared_memory>
#include <mutex>
#include <unordered_map>

// Some abstraction around shared memory
static std::unordered_map<std::PID, uint8_t*> shared;
static std::mutex lock;

bool std::sm::connect(PID pid, std::SMID smid) {
	// Already connected?
	if(!smRequest(pid, smid))
		return false;

	uint8_t* ptr = (uint8_t*)smMap(smid);
	if(!ptr)
		return false;

	lock.acquire();
	shared[pid] = ptr;
	lock.release();
	return true;

	// TODO: unmap previous, release SMID
}

uint8_t* std::sm::get(PID pid) {
	lock.acquire();
	if(shared.find(pid) == shared.end()) {
		lock.release();
		return nullptr;
	}

	uint8_t* ret = shared[pid];
	lock.release();
	return ret;
}
