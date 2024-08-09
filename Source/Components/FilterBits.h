//#pragma once
//
//
//#include <Component.h>
//
//
//namespace pd
//{
//	struct FilterBitsArgs : public ComponentArgs
//	{
//	};
//
//	class FilterBits : public PooledComponent<FilterBits>
//	{
//		using filter_bits_pool_type = ObjectPool<FilterBits>;
//		using pooled_base_type = PooledComponent<FilterBits>;
//
//		friend class filter_bits_pool_type;
//
//	public:
//		FilterBits() = default;
//		~FilterBits() = default;
//
//	private:
//		void onConstruction() override {}	   // Called when object is allocated
//		void init(ObjectArgs* pArgs) override  // Called when object is created by pool
//		{
//			ASSERT_MSG(pArgs, "Expected FilterBitsArgs was null.");
//
//			auto args = *reinterpret_cast<FilterBitsArgs*>(pArgs);
//		}
//
//	public:
//		void destroy() override {};
//	};
//}
