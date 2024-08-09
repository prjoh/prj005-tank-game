#pragma once


#include <Component.h>


namespace pd
{
	template <class T>
	class CommandComponent : public PooledComponent<T>
	{
	public:
		CommandComponent() = default;
		virtual ~CommandComponent() = default;

	//private:
	//	void onConstruction() override {}		   // Called when object is allocated
	//	void init(ObjectArgs* pArgs) override {};  // Called when object is created by pool

	public:
		//void destroy() override {};
		virtual void reset() = 0;
	};
}
