#pragma once

#include <ranges>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>

#include "nvse/GameAPI.h"
#include "nvse/GameData.h"
#include "nvse/GameObjects.h"
#include "nvse/GameScript.h"
#include "nvse/PluginAPI.h"
#include "nvse/SafeWrite.h"
#include "nvse/Utilities.h"

#include "lib/distribution.hpp"
#include "lib/numeric.hpp"
#include "lib/rng.hpp"
#include "lib/simpleINI.hpp"
#include "lib/string.hpp"

#include "srell.hpp"

namespace string = clib_util::string;
namespace dist = clib_util::distribution;
namespace numeric = clib_util::numeric;

using SeedRNG = clib_util::RNG;

template <class T>
using MinMax = std::pair<T, T>;
template <class T>
using RelData = std::pair<bool, MinMax<T>>;  //relative vs absolute

using FormIDStr = std::variant<std::uint32_t, std::string>;

#if 0
using FormIDSet = std::unordered_set<std::uint32_t>;
#endif
using FormIDOrSet = std::variant<std::uint32_t, FormIDSet>;

template <class T>
using SwapMap = std::unordered_map<std::uint32_t, T>;
