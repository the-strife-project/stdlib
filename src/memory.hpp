#ifndef _STDLIB_MEMORY_HPP
#define _STDLIB_MEMORY_HPP

namespace std {
	// Reference Counted Pointer
	// Kind of like standard's shared_ptr, but without atomicity
	template<typename T> class rc_ptr {
	private:
		struct Ptr {
			T* ptr = nullptr;
			size_t count = 0;
		};

		Ptr* ptr = nullptr;

	public:
		rc_ptr() = default;
		rc_ptr(T* ptr_) {
			ptr = new Ptr;
			ptr->ptr = ptr_;
			++ptr->count;
		}

		rc_ptr(const rc_ptr& other) {
			ptr = other.ptr;
			++ptr->count;
		}

		rc_ptr& operator=(const rc_ptr& other) {
			ptr = other.ptr;
			++ptr->count;
			return *this;
		}

		rc_ptr(rc_ptr&& other) {
			ptr = other.ptr;
			other.ptr = nullptr;
		}

		rc_ptr& operator=(rc_ptr&& other) {
			ptr = other.ptr;
			other.ptr = nullptr;
			return *this;
		}

		~rc_ptr() {
			if(!--ptr->count)
				delete ptr;
			ptr = nullptr;
		}

		inline T* operator*() { return ptr->ptr; }
		inline T* operator->() { return ptr->ptr; }
		inline const T* data() const { return ptr->ptr; }
		inline T* data() { return ptr->ptr; }
	};



	class Buffer {
	private:
		rc_ptr<uint8_t> data = nullptr;
		size_t sz = 0;

	public:
		Buffer() = default;
		Buffer(rc_ptr<uint8_t> data, size_t sz)
			: data(data), sz(sz)
		{}

		inline const uint8_t* get() const { return data.data(); }
		inline uint8_t* get() { return data.data(); }

		inline size_t size() const { return sz; }
	};
};

#endif
