#pragma once

#include "EngineCore.h"

#pragma warning(disable : 4201)
namespace ECS
{
	template <typename ValueType, size_t NUM_VERSION_BITS, size_t NUM_INDEX_BITS>
	struct Handle
	{
		static_assert(sizeof(ValueType) * CHAR_BIT == NUM_VERSION_BITS + NUM_INDEX_BITS,
					  "Incorretct NumVersionBits and NumIndexBits.");

		using value_type = ValueType;
		static constexpr ValueType MIN_VERSION = 0u;
		static constexpr ValueType MAX_VERSION = (1ull << NUM_INDEX_BITS) - 2u;
		static constexpr ValueType MAX_INDEX = (1ull << NUM_INDEX_BITS) - 2u;
		static constexpr ValueType INVALID_ID = std::numeric_limits<ValueType>::max();

		Handle() : value(0)
		{
		}
		Handle(ValueType value) : value(value)
		{
		}
		Handle(ValueType index, ValueType version) : index(index), version(version)
		{
		}
		operator ValueType() const
		{
			return value;
		}
		union {
			struct
			{
				ValueType index : NUM_INDEX_BITS;
				ValueType version : NUM_VERSION_BITS;
			};
			ValueType value;
		};
	};

	using Handle64 = Handle<uint64, 24, 40>;

	template <typename T, typename Handle, size_t GROW = 1024>
	class HandleTable
	{
	private:
		struct TableData
		{
			typename Handle::value_type version = Handle::MIN_VERSION;
			T *objectPtr = nullptr;
		};

	public:
		HandleTable() : table_(GROW){};
		Handle acquiredHandle(T *objectPtr)
		{
			size_t i = 0;
			for (; i < table_.size(); ++i)
			{
				if (table_[i].objectPtr == nullptr)
				{
					table_[i].objectPtr = objectPtr;
					table_[i].version = table_[i].version + 1;
					if (table_[i].version > Handle::MAX_VERSION)
					{
						table_[i].version = Handle::MIN_VERSION;
					}
					return Handle(i, table_[i].version);
				}
			}
			table_.reserve(table_.size() + GROW);
			table_.push_back({1, objectPtr});
			return Handle(i, table_[i].version);
		};

		void releaseHandle(Handle id)
		{
			assert(id.index < table_.size() && id.version == table_[id.index].version && "Invalid ID");
			table_[id.index].objectPtr = nullptr;
		}

		T *operator[](Handle id)
		{
			assert(id.index < table_.size() && id.version == table_[id.index].version && "Invalid ID");
			return table_[id.index].objectPtr;
		}

	private:
		std::vector<TableData> table_;
	};

} // namespace ECS