#include <Components/FSMData.h>


namespace pd
{
	const std::unordered_map<size_t, eConditionType> g_ConditionTypes = {
		{std::hash<std::string_view>{}("GreaterThan"), eConditionType::GreaterThan},
		{std::hash<std::string_view>{}("LessThan"), eConditionType::LessThan},
		{std::hash<std::string_view>{}("GreaterEq"), eConditionType::GreaterEq},
		{std::hash<std::string_view>{}("LessEq"), eConditionType::LessEq},
		{std::hash<std::string_view>{}("Eq"), eConditionType::Eq},
		{std::hash<std::string_view>{}("NotEq"), eConditionType::NotEq},
	};
}