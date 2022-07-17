#include <shared_memory>
#include <mutex>
#include <unordered_map>

// Some abstraction around shared memory
static std::unordered_map<std::PID, uint8_t*> shared;
static std::unordered_map<std::PID, size_t> npages;
static std::mutex lock;

#include <cstdio>
bool std::sm::connect(std::PID pid, std::SMID smid) {
	// Already connected?
	if(!smRequest(pid, smid))
		return false;

	uint8_t* ptr = (uint8_t*)smMap(smid);
	if(!ptr)
		return false;
	size_t sz = smGetSize(smid);

	lock.acquire();
	shared[pid] = ptr;
	npages[pid] = sz;
	lock.release();
	return true;

	// TODO: unmap previous, release SMID
}

uint8_t* std::sm::get(std::PID pid) {
	lock.acquire();
	if(!shared.has(pid)) {
		lock.release();
		return nullptr;
	}

	uint8_t* ret = shared[pid];
	lock.release();
	return ret;
}

size_t std::sm::getPages(std::PID pid) {
	lock.acquire();
	if(!npages.has(pid)) {
		lock.release();
		return 0;
	}

	size_t ret = npages[pid];
	lock.release();
	return ret;
}

static std::unordered_map<std::SMID, uint8_t*> links;

std::pair<uint8_t*, size_t> std::sm::link(std::PID pid, std::SMID smid) {
	if(!smRequest(pid, smid))
		return {nullptr, 0};
	uint8_t* ptr = (uint8_t*)smMap(smid);
	if(!ptr)
		return {nullptr, 0};

	links[smid] = ptr;
	return {ptr, smGetSize(smid)};
}

void std::sm::unlink(std::SMID smid) {
	if(!links.has(smid))
		return;

	size_t npages = smGetSize(smid);
	if(!npages)
		return;

	munmap(links[smid], npages);
	smDrop(smid);

	// TODO
	links.erase(std::pair<std::SMID, uint8_t*>(smid, nullptr));
}
