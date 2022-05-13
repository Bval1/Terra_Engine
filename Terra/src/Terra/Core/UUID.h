#pragma once
#include <xhash>
namespace Terra {

	class UUID
	{
	public:
		UUID();	// generates UUID randomly
		UUID(uint64_t uuid);	// will be used when deserializing entities given an id
		UUID(const UUID&) = default;
		
		// This can be used to cast or implicit cast to uint64_t
		operator uint64_t() const { return m_UUID; }	
	
	private:
		uint64_t m_UUID;  // traditionally a 128 bit value, win32 function CreateGUID create one
	};
	

}

// Provide hash function to hash a UUID type in  unordered_map<UUID, Entity>
namespace std {

	template<>	// template specialization of UUID
	struct hash<Terra::UUID>
	{
		std::size_t operator()(const Terra::UUID& uuid) const // will return a size_t  when hashing UUID
		{
			return hash<uint64_t>()((uint64_t)uuid);	// redirect it to another specilization of hash class
		}
	};
}