-- Defaults will be based on platform in the future.
-- Currently only supports buildoptions.
project_defaults = {

  buildoptions_windows = {
    "/IGNORE:C4577",
  },

  flags = {
    "EnableSSE2",
    "ExtraWarnings",
    "FloatFast",
    -- "NoExceptions", -- deprecated premake5
    -- "NoRTTI", -- deprecated premake5
  },

  defines = {
    --"MATH_USE_SIMD",
  },

  defines_windows = {
    -- Crazy Window.h stuff
    "WIN32_LEAN_AND_MEAN=1",
    "NOMINMAX",

    -- MSVS Tweaks
    "_HAS_EXCEPTIONS=0",
    "_CRT_NO_WARNINGS",
    "_ITERATOR_DEBUG_LEVEL=0",
  },

--   exceptions = false,
--   rtti = false,
}



-- This is used by the individual projects, but it needs to be defined first.
projects = {}
