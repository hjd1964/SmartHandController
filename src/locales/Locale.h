// -----------------------------------------------------------------------------------
// Locale
#pragma once

#include "../Constants.h"
#include "../locales/Locales.h"
#include "../../Config.h"

// see Strings_xx.h for individual locale translations

// ISO639-1 language codes (these control the inclusion of individual Strings_xx.h files as specified in Config.h)
#if DISPLAY_LANGUAGE == L_de
  #include "Strings_de.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_en
  #include "Strings_en.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_us // not ISO639-1 but might be useful
  #include "Strings_en.h"
  #define UNITS IMPERIAL
#endif
#if DISPLAY_LANGUAGE == L_es
  #include "Strings_es.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_fr
  #include "Strings_fr.h"
  #define UNITS METRIC
#endif
