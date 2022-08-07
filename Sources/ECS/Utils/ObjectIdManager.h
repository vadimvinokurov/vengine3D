#pragma once

#include "EnginePlatform.h"

#pragma warning(disable : 4201)

template <size_t NUM_INDEX_BITS, size_t NUM_VERSION_BITS>
struct IDHandle_base
{
	static_assert(sizeof(idtype) * CHAR_BIT == NUM_VERSION_BITS + NUM_INDEX_BITS,
				  "Incorrect NumVersionBits and NumIndexBits.");

	static constexpr idtype MIN_VERSION = 0u;
	static constexpr idtype MAX_VERSION = (1ull << NUM_INDEX_BITS) - 2u;
	static constexpr idtype MAX_INDEX = (1ull << NUM_INDEX_BITS) - 2u;

	IDHandle_base(idtype value = 0) : value(value)
	{
	}
	IDHandle_base(idtype index, idtype version) : index(index), version(version)
	{
		assert(index < MAX_INDEX && version < MAX_VERSION && "Incorrect id constructor");
	}
	operator idtype() const
	{
		return value;
	}
	union {
		struct
		{
			idtype index : NUM_INDEX_BITS;
			idtype version : NUM_VERSION_BITS;
		};
		idtype value;
	};
};

#ifdef ENV64BIT
using IDHandle = IDHandle_base<24, 40>;
#else
using IDHandle = IDHandle_base<12, 20>;
#endif

inline size_t getIdIndex(idtype id) {
	return IDHandle(id).index;
}

template <typename T, size_t GROW = 1024>
class ObjectIdManager
{
private:
	struct TableData
	{
		idtype version = IDHandle::MIN_VERSION;
		T *objectPtr = nullptr;
	};

public:
	ObjectIdManager() : table_(GROW){};
	idtype acquiredId(T *objectPtr)
	{
		size_t i = 0;
		for (; i < table_.size(); ++i)
		{
			if (table_[i].objectPtr == nullptr)
			{
				table_[i].objectPtr = objectPtr;
				table_[i].version = table_[i].version + 1;
				if (table_[i].version > IDHandle::MAX_VERSION)
				{
					table_[i].version = IDHandle::MIN_VERSION;
				}
				return IDHandle(i, table_[i].version);
			}
		}
		table_.resize(table_.size() + GROW);
		table_[i].objectPtr = objectPtr;
		table_[i].version = 1;
		assert(i < IDHandle::MAX_INDEX && "Max table capacity reached!");
		return IDHandle(i, table_[i].version);
	};

	void releaseId(idtype id)
	{
		IDHandle idhandle = id;
		assert(idhandle.index < table_.size() && idhandle.version == table_[idhandle.index].version && "Invalid ID");
		table_[idhandle.index].objectPtr = nullptr;
	}

	T *operator[](idtype id)
	{
		IDHandle idhandle = id;
		assert(idhandle.index < table_.size() && idhandle.version == table_[idhandle.index].version && "Invalid ID");
		return table_[idhandle.index].objectPtr;
	}

private:
	std::vector<TableData> table_;
};
