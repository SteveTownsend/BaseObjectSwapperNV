#pragma once
#if 0
#	define WIN32_LEAN_AND_MEAN
#	define NOMINMAX

#	include <ranges>

#	include <ankerl/unordered_dense.h>
#	include <boost/container_hash/hash.hpp>
#	include <spdlog/sinks/basic_file_sink.h>
#	include <srell.hpp>

#	include <CLibUtil/distribution.hpp>
#	include <CLibUtil/hash.hpp>
#	include <CLibUtil/numeric.hpp>
#	include <CLibUtil/rng.hpp>
#	include <CLibUtil/string.hpp>
#	include <ClibUtil/simpleINI.hpp>

#	include <ClibUtil/editorID.hpp>

#	define DLLEXPORT __declspec(dllexport)

using namespace std::literals;

using namespace clib_util;
using SeedRNG = clib_util::RNG;

// for visting variants
template <class... Ts>
struct overload : Ts...
{
	using Ts::operator()...;
};

using FormIDStr = std::variant<RE::FormID, std::string>;

template <class K, class D>
using Map = ankerl::unordered_dense::map<K, D>;
template <class T>
using Set = ankerl::unordered_dense::set<T>;
template <class T>
using OrderedSet = std::set<T>;

using FormIDSet = Set<RE::FormID>;
using FormIDOrSet = std::variant<RE::FormID, FormIDSet>;
using FormIDOrderedSet = OrderedSet<RE::FormID>;

template <class T>
using FormIDMap = Map<RE::FormID, T>;

namespace stl
{
	using namespace SKSE::stl;

	template <class F, class T>
	void write_vfunc()
	{
		REL::Relocation<std::uintptr_t> vtbl{ F::VTABLE[0] };
		T::func = vtbl.write_vfunc(T::size, T::thunk);
	}

	template <class T>
	void write_thunk_call(std::uintptr_t a_src)
	{
		auto& trampoline = SKSE::GetTrampoline();
		SKSE::AllocTrampoline(14);

		T::func = trampoline.write_call<5>(a_src, T::thunk);
	}
}

#	ifdef SKYRIM_AE
#		define OFFSET(se, ae) ae
#		define OFFSET_3(se, ae, vr) ae
#	elif SKYRIMVR
#		define OFFSET(se, ae) se
#		define OFFSET_3(se, ae, vr) vr
#	else
#		define OFFSET(se, ae) se
#		define OFFSET_3(se, ae, vr) se
#	endif

#	include "Util.h"
#	include "Version.h"
#else

#	include <ranges>
#	include <set>
#	include <string>
#	include <string_view>
#	include <unordered_map>
#	include <unordered_set>
#	include <variant>

#	include "common/IPrefix.h"
#	include "nvse/prefix.h"
#	include "nvse/GameAPI.h"
#	include "nvse/GameData.h"
#	include "nvse/GameObjects.h"
#	include "nvse/GameScript.h"
#	include "nvse/PluginAPI.h"
#	include "nvse/SafeWrite.h"
#	include "nvse/Utilities.h"

#	include "lib/distribution.hpp"
#	include "lib/numeric.hpp"
#	include "lib/rng.hpp"
#	include "lib/simpleINI.hpp"
#	include "lib/string.hpp"

#	include "srell.hpp"

#	include <ankerl/unordered_dense.h>

#	include "Util.h"
#	include "Version.h"

namespace string = clib_util::string;
namespace dist = clib_util::distribution;
namespace numeric = clib_util::numeric;

using SeedRNG = clib_util::RNG;

template <class T>
using MinMax = std::pair<T, T>;
template <class T>
using RelData = std::pair<bool, MinMax<T>>;  //relative vs absolute

using FormIDStr = std::variant<std::uint32_t, std::string>;

// using FormIDSet = std::unordered_set<std::uint32_t>;
// using FormIDOrSet = std::variant<std::uint32_t, FormIDSet>;

template <class T>
using SwapMap = std::unordered_map<std::uint32_t, T>;

namespace BOS
{
	template <class K, class D>
	using Map = ankerl::unordered_dense::map<K, D>;
	template <class T>
	using Set = ankerl::unordered_dense::set<T>;
}
template <class T>
using OrderedSet = std::set<T>;

using FormIDSet = BOS::Set<RE::FormID>;
using FormIDOrSet = std::variant<RE::FormID, FormIDSet>;
using FormIDOrderedSet = OrderedSet<RE::FormID>;

template <class T>
using FormIDMap = BOS::Map<RE::FormID, T>;

#	include "Defs.h"

#endif