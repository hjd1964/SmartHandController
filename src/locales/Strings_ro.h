// -----------------------------------------------------------------------------------
// Locale Romanian (without diacritics), ISO ISO639-1 language code "ro"
//
// Note: SHC fonts default to u8g2_font_helvR10_tf / 6x13_tf (basic ASCII).  To use
// Romanian diacritics (a-breve, a-circumflex, i-circumflex, s-comma, t-comma) swap
// to extended variants below (e.g. u8g2_font_helvR10_te / 6x13_te).
#pragma once

// Fonts, see https://github.com/olikraus/u8g2/wiki/fntlistall for the complete list
#define LF_CATALOGS u8g2_font_6x13_tf       //  9 pixel height
#define LF_GREEK u8g2_font_unifont_t_greek  // 10 pixel height
#define LF_STANDARD u8g2_font_helvR10_tf    // 11 pixel height
#define LF_LARGE u8g2_font_helvR12_tf       // 12 pixel height
#define LF_PASSWORD u8g2_font_t0_17_mf      // 11 pixel height monospaced

// General
#define L_OK "OK"
#define L_ON "Pornit"
#define L_OFF "Oprit"
#define L_YES "Da"
#define L_NO "Nu"
#define L_NOW "Acum"
#define L_SETV "Set"
#define L_ALL "Toate"
#define L_USE "Foloseste"
#define L_SET_STATE "Set Stare"
#define L_VALUE "Valoare"
#define L_WITHIN "In"
#define L_DISABLE "Dezactiv"
#define L_CANCELED "Anulat"
#define L_CURRENT "Curent"
#define L_FAILED "Esuat"
#define L_DEGREE "grad"
#define L_ARCSEC "arc-sec"
#define L_MICRON_PER_C "um/C"
#define L_DAYS "Zile"
#define L_SHOW_EDIT "- Editare -"
#define L_SHOW_KNOWN "- Arata Cunoscute -"
#define L_UPDATING "Actualizare"
#define L_SCANNING "Scanare"
#define L_PLEASE_WAIT "Asteptati"
#define L_CONFIRM "Sigur?"
#define L_CLEAR "Sterge"
#define L_MEMORY "Memorie"
#define L_OVERWRITE "Suprascrie"
#define L_PASSWORD "Parola"
#define L_DHCP_LINE1 "Dezact. DHCP poate"
#define L_DHCP_LINE2 "afecta functionarea!"
#define L_POWERING "Pornire"

// common abbreviations
#define L_TELESCOPE "Telesc" // telescope
#define L_SEC_ABV "sec"     // seconds
#define L_MIN_ABV "min"     // minutes
#define L_HRS_ABV "ore"     // hours
#define L_OFFSET_ABV "Ofs"  // offset
#define L_TZ_ABV "Zona"     // time-zone
#define L_RA "RA"           // Right Ascension
#define L_DE "De"           // Declination
#define L_DEC "Dec"
#define L_AZ "Az"           // Azimuth
#define L_ALT "Alt"         // Altitude

// --------------------- menu, alignment -------------------

#define L_ALGN_RESUME "Reia Aliniere"
#define L_ALGN_SHOW_CORR "Arata Cor"
#define L_ALGN_CLEAR_CORR "Sterge Cor"
#define L_ALGN_RESET_HOME "Reset Origine"
#define L_ALIGNMENT "Aliniere"
#define L_ALGN_STAR_ALIGN "Aliniere Stea"
#define L_ALGN_SHOW_MODEL "Arata Model"
#define L_ALGN_CLEAR_MODEL "Sterge Model"
#define L_ALGN_RESET_HOME "Reset Origine"

#define L_ALGN_RESUME0 "Resetare"
#define L_ALGN_RESUME1 "Mutare la Origine"
#define L_ALGN_RESUME2 "Data/Ora"
#define L_ALGN_RESUME3 "Nesetate!"
#define L_ALGN_RESUME4 "Rata Ghidare"
#define L_ALGN_RESUME5 "Set 48X"

#define L_ALGN_REFINE_MSG1 "Setup & aliniere"
#define L_ALGN_REFINE_MSG2 "3+ stele."
#define L_ALGN_REFINE_MSG3 "Goto stea aprins"
#define L_ALGN_REFINE_MSG4 "langa NCP/SCP cu"
#define L_ALGN_REFINE_MSG5 "Dec intre 50 si 80"
#define L_ALGN_REFINE_MSG6 "grade N/S."
#define L_ALGN_REFINE_MSG7 "Apasa DA mai jos."
#define L_ALGN_REFINE_MSG8 "Folos. ajustari PA"
#define L_ALGN_REFINE_MSG9 "ca sa centrezi"
#define L_ALGN_REFINE_MSG10 "din nou steaua."
#define L_ALGN_REFINE_MSG11 "Optional reali-"
#define L_ALGN_REFINE_MSG12 "niaza montura."
#define L_ALGN_REFINE_MSG13 "Rafinare PA?"
#define L_ALGN_REFINE_MSG14 "Centreaza din nou"
#define L_ALGN_REFINE_MSG15 "cu controale PA"

// ----------------------- menu, main ----------------------

// feature key menu
#define L_FKEY_GUIDE_RATE "Rata Ghidare"
#define L_FKEY_PULSE_GUIDE_RATE "Rata Ghid. Puls"
#define L_FKEY_UTILITY_LIGHT "Lumina Utilitara"
#define L_FKEY_RETICLE "Reticul"
#define L_FKEY_FOCUSER "Focuser"
#define L_FKEY_ROTATOR "Rotor"
#define L_FKEY_FEATURE_KEYS "Taste Functii"

// main menu root
#define L_MM_GOTO "Goto"
#define L_MM_SYNC "Sinc"
#define L_MM_ALIGN "Aliniere"
#define L_MM_PARKING "Parcare"
#define L_MM_TRACKING "Urmarire"
#define L_MM_PEC "PEC"
#define L_MM_SETTINGS "Setari"
#define L_MM_MAIN_MENU "Meniu Principal"

// park menu
#define L_PARK "Parc."
#define L_SETPARK "Set-Parc."
#define L_UNPARK "Deparc."
#define L_PARKING "Parcare"
#define L_UNPARKING "Deparcare"

// tracking menu
#define L_TRK_START "Start"
#define L_TRK_STOP "Stop"
#define L_TRK_SIDEREAL "Sideral"
#define L_TRK_SOLAR "Solar"
#define L_TRK_LUNAR "Lunar"
#define L_TRK_RESET "Reset Rata"
#define L_TRK_FASTER "Rata +0.02Hz"
#define L_TRK_SLOWER "Rata -0.02Hz"
#define L_TRACKING "Urmarire"
#define L_TRK_CF "Comp Total"
#define L_TRK_CR "Comp Refr"
#define L_TRK_CO "Comp Oprit"
#define L_TRK_CS "Comp 1 Axa"
#define L_TRK_CD "Comp 2 Axe"

// PEC menu
#define L_PEC_PLAY "Redare"
#define L_PEC_STOP "Stop"
#define L_PEC_RECORD "Inreg."
#define L_PEC_WRITENV "Scrie in NV"
#define L_PEC "PEC"
#define L_PEC_PLAYING "Redare"
#define L_PEC_STOPPED "Oprit"
#define L_PEC_RECORDING "Inregistreaza"

// ---------------------- menu, mount ----------------------

// root menu
#define L_MOUNT_SPEED "Viteza Goto"
#define L_MOUNT_BL "Backlash"
#define L_MOUNT_LIMITS "Limite"
#define L_MOUNT_PIER "Latura Pivot"
#define L_MOUNT_CONFIG "Configurare"

#define L_MOUNT_FASTEST "Maxim"
#define L_MOUNT_FASTER "Rapid"
#define L_MOUNT_DEFAULT_SPEED "Viteza Implicita"
#define L_MOUNT_SLOWER "Incet"
#define L_MOUNT_SLOWEST "Minim"
#define L_MOUNT_SPEED "Viteza Goto"

#define L_MOUNT_BL "Backlash"
#define L_MOUNT_LIMIT_H "Orizont"
#define L_MOUNT_LIMIT_O "Zenit"
#define L_MOUNT_LIMIT_ME "Meridian E"
#define L_MOUNT_LIMIT_MW "Meridian V"

#define L_MOUNT_LIMITS "Limite"
#define L_MOUNT_LIMIT_HORIZON "Limita Orizont"
#define L_MOUNT_LIMIT_OVERHEAD "Limita Zenit"
#define L_MOUNT_LIMIT_MERIDIAN_EAST "Lim. Merid. E"
#define L_MOUNT_LIMIT_MERIDIAN_WEST "Lim. Merid. V"
#define L_MOUNT_PPS "Latura Pivot Pref"
#define L_PPS_EAST "Est"
#define L_PPS_WEST "Vest"
#define L_PPS_BEST "Optim"
#define L_PPS_AUTO "Auto"

// -------------------- menu, settings ---------------------

// root menu
#define L_SET_DATE_TIME "Data/Ora"
#define L_SET_SITE "Locatie"
#define L_SET_FOCUSER "Focuser"
#define L_SET_FOCUSER1 "Focuser1"
#define L_SET_FOCUSER2 "Focuser2"
#define L_SET_ROTATOR "Rotor"
#define L_SET_DISPLAY "Afisaj"
#define L_SET_BUZ "Sonerie"
#define L_SET_MERIDIAN_FLIP "Flip Meridian"
#define L_SET_CONFIG "Configurare"
#define L_SET_VERSION "Firmware"
#define L_SETTINGS "Setari"

// date/time
#define L_SET_LOCAL_DATE "Data Locala"
#define L_SET_LOCAL_PM "Ora Locala PM"
#define L_SET_LOCAL_DST "Ora Locala DST"

// display
#define L_SET_DISP_OFF "Oprire"
#define L_SET_DISP_CONT "Contrast"
#define L_SET_DISP_DIM_TO "Atenuare"
#define L_SET_DISP_BLANK_TO "Stingere"
#define L_DISPLAY "Afisaj"
#define L_SET_DISP_MSG1 "Apasa o tasta"
#define L_SET_DISP_MSG2 "pentru pornire"
#define L_SET_DISP_MIN "Min"
#define L_SET_DISP_LOW "Slab"
#define L_SET_DISP_HIGH "Mare"
#define L_SET_DISP_MAX "Max"
#define L_SET_DISP_CONTRAST "Set Contrast"
#define L_SET_BUZZER "Sonerie?"

// meridian flips
#define L_SET_MF_AUTO "Automat"
#define L_SET_MF_PAUSE "Pauza la Origine"
#define L_SET_MF "Flip Meridian"
#define L_SET_MF_AF "Flip Auto?"
#define L_SET_MF_PF "Flip cu Pauza?"

// site
#define L_SET_SITE_SELECT "Selectie Locatie"
#define L_SET_SITE_LAT "Latitudine"
#define L_SET_SITE_LONG "Longitudine"
#define L_SET_SITE_UTC "Decalaj UTC"
#define L_SET_SITE_TITLE "Meniu Locatie"
#define L_SET_SITE_NUM "Locatie"
#define L_SET_SITE_NUM_TITLE "Meniu Locatii"

// rotator
#define L_ROT_RET_HOME "Mergi Acasa"
#define L_ROT_AT_HOME "Acasa"
#define L_ROT_DEROT "De-rotire"
#define L_ROT_PA "Mergi la PA"
#define L_ROT_REV "Inverseaza"
#define L_ROTATOR "Rotor"
#define L_ROTATE "Rot"
#define L_ROT_AT_HOME_ZERO "Acasa/Zero?"
#define L_ROT_REVERSE "Inverseaza"

// focuser
#define L_FOC_RET_HOME "Mergi Acasa"
#define L_FOC_AT_HOME  "Acasa"
#define L_FOCUSER "Focuser"
#define L_FOCUS "Foc"
#define L_AUX_FEATURE "Func Aux"
#define L_FOC_AT_HALF "Cursa la 1/2?"
#define L_FOC_TC "Comp. Temp.?"
#define L_FOC_TC_COEF "TC Coef."
#define L_FOC_TC_DEADBAND "TC Deadband"
#define L_FOC_TC_DB_UNITS "micron(i)"
#define L_FOC_BACKLASH "Backlash"
#define L_FOC_BL_UNITS "micron(i)"

// dew heater
#define L_AF_DEW_HEATER_ZERO "Pct Roua, max pwr"
#define L_AF_DEW_HEATER_SPAN "Pct Roua, 0% pwr"

// intervalometer
#define L_AF_IV_COUNT "Nr Exp."
#define L_AF_IV_EXPOS "Durata Exp."
#define L_AF_IV_DELAY "Pauza Exp."

// -------------------- menu, sync/goto --------------------

// root menu
#define L_SG_SOLSYS "Sistem Solar"
#define L_SG_HERE "Aici"
#define L_SG_USER "Utilizator"
#define L_SG_FILTERS "Filtre \xa5"
#define L_SG_COORDS "Coordonate"
#define L_SG_HOME "Acasa"
#define L_SG_SYNC "Sinc"
#define L_SG_GOTO "Goto"
#define L_SG_SPIRAL "Cautare Spirala"
#define L_SG_LAST "Ultima"
#define L_SG_GOTO_LAST "Goto Ultima"
#define L_SG_TARGET "Tinta"

// return home or reset at home
#define L_SG_HOME1 "Goto Acasa"
#define L_SG_HOME2 "Reset la"
#define L_SG_HOME3 "Goto"
#define L_SG_HOME4 "Pozitie Origine"
#define L_SG_NO_OBJECT "Fara Obiect"
#define L_SG_NO_INIT "Neinitiat"

// solsys
#define L_SG_SUN "Soare"
#define L_SG_MER "Mercur"
#define L_SG_VEN "Venus"
#define L_SG_MAR "Marte"
#define L_SG_JUP "Jupiter"
#define L_SG_SAT "Saturn"
#define L_SG_URA "Uranus"
#define L_SG_NEP "Neptun"
#define L_SG_MOON "Luna"
#define L_SG_SSOL "Sinc Sist Solar"
#define L_SG_GSOL "Goto Sist Solar"
#define L_SG_SOL_WARN1 "Indicare spre Soare"
#define L_SG_SOL_WARN2 "este periculoasa"
#define L_SG_GSUN "Goto Soare"

// user catalogs
#define L_SG_USER_MSG1 "Selectie Cat Util"
#define L_SG_USER_MSG2 "Fara Cataloage"
#define L_SG_SYNC_USER "Sinc Utilizator"
#define L_SG_GOTO_USER "Goto Utilizator"
#define L_SG_SYNC_USER_ITEM "Sinc Item Util"
#define L_SG_GOTO_USER_ITEM "Goto Item Util"

// --------------------- menu, filters ---------------------

// root menu
#define L_SG_FILT_RESET "Reset filtre"
#define L_SG_FILT_HOR "Peste Orizont"
#define L_SG_FILT_CON "Constelatie"
#define L_SG_FILT_TYP "Tip"
#define L_SG_FILT_MAG "Magnitudine"
#define L_SG_FILT_NEAR "In apropiere"
#define L_SG_FILT_VAR_MAX "Var* Per. max"
#define L_SG_FILT_DBL_MIN "Dbl* Sep. min"
#define L_SG_FILT_DBL_MAX "Dbl* Sep. max"
#define L_SG_FILT_ALLOW "Filtre permise"
#define L_SG_FILT_MSG1 "Filtre"
#define L_SG_FILT_MSG2 "Reset"
#define L_SG_FILT_MSG3 "Doar Peste"
#define L_SG_FILTER "Filtru"

// filter constellation
#define L_SG_FILT_BY_CON "Filtru Constel"

// filter type
#define L_SG_FILT_BY_TYPE "Filtru Tip"

// filter magn
#define L_SG_FILT_BY_MAG "Filtru Magnitudine"

// filter nearby
#define L_SG_FILT_BY_NEAR "Filtru Apropiate"

// filter dbl min sep
#define L_SG_FILT_BY_SEP_MIN "Filtru Dbl* Sep."
#define L_SG_SEP_MIN_MSG1 "Sep min trebuie"
#define L_SG_SEP_MIN_MSG2 "sa fie < Sep max."

// filter dbl max sep
#define L_SG_FILT_BY_SEP_MAX "Filtru Dbl* Sep."
#define L_SG_SEP_MAX_MSG1 "Sep max trebuie"
#define L_SG_SEP_MAX_MSG2 "sa fie > Sep min."

// filter var max per
#define L_SG_FILT_BY_PER_MAX "Filtru Var* Per."

// ------------------- SmartController.h -----------------------
#define L_WIFI_SELECT "Conexiune OnStep:"
#define L_WIFI_SELECT_EDIT "Editare Conexiune:"
#define L_WIFI_CONNECTION "Pornire WiFi:"
#define L_BT_CONNECTION "Pornire BT:"
#define L_LOOKING "Caut"
#define L_WARNING "Atentie"
#define L_CONNECTION "Conexiune"
#define L_CONNECTING "Conectare"
#define L_WARNING "Atentie"
#define L_COORDINATES "Coordonate"
#define L_OBSERVED_PLACE "Loc Observat."
#define L_WARNING "Atentie"
#define L_REBOOT "REPORNIRE"
#define L_DEVICE "DISPOZITIV"
#define L_STAR "Stea"
#define L_ALIGN_MSG1 "Selecteaza o Stea"
#define L_ALIGN_MSG2 "Din lista urmatoare"
#define L_ABORTED "Abandonat"
#define L_LOST_MSG "Pierdut"
#define L_SLEW_MSG1 "Miscare spre tinta"
#define L_SLEW_MSG2 "anulata"
#define L_FKEY_GUIDE_DN "Ghid Lent"
#define L_FKEY_GUIDE_UP "Ghid Rapid"
#define L_FKEY_PGUIDE_DN "PGhid Lent"
#define L_FKEY_PGUIDE_UP "PGhid Rapid"
#define L_FKEY_LAMP_DN "Util Slab"
#define L_FKEY_LAMP_UP "Util Aprins"
#define L_FKEY_RETI_DN "Retic Slab"
#define L_FKEY_RETI_UP "Retic Aprins"
#define L_SUCCESS "Reusit"
#define L_ADD_STAR "Adauga Stea"
#define L_SUCCESS "Reusit"
#define L_FAILED "Esuat"
#define L_SLEWING_TO_STAR "Miscare la Stea"
#define L_RECENTER_STAR "Recentreaza Stea"
#define L_SELECT_STAR "Selecteaza Stea"
#define L_CMD_NOTOK_1 "Comanda LX200"
#define L_CMD_NOTOK_2 "a esuat!"
#define L_CMD_SETVF_1 "Setare Valoare"
#define L_CMD_SETVF_2 "a esuat!"
#define L_CMD_GETVF_1 "Citire Valoare"
#define L_CMD_GETVF_2 "a esuat!"
#define L_CMD_SETTG_1 "Setare Tinta"
#define L_CMD_SETTG_2 "a esuat!"
#define L_CMD_OBJSE_1 "Niciun Obiect"
#define L_CMD_OBJSE_2 "Selectat!"
#define L_CMD_TGHOR_1 "Tinta este"
#define L_CMD_TGHOR_2 "Sub Orizont!"
#define L_CMD_TGOVH_1 "Tinta este"
#define L_CMD_TGOVH_2 "Peste Limita!"
#define L_CMD_STNBF_1 "Telescopul"
#define L_CMD_STNBF_2 "in standby!"
#define L_CMD_PARKF_1 "Telescopul"
#define L_CMD_PARKF_2 "este Parcat!"
#define L_CMD_GOGOF_1 "Goto deja"
#define L_CMD_GOGOF_2 "in curs!"
#define L_CMD_LIMTF_1 "Tinta"
#define L_CMD_LIMTF_2 "in afara lim.!"
#define L_CMD_HARDF_1 "Telescop"
#define L_CMD_HARDF_2 "defect h/w!"
#define L_CMD_GOMOF_1 "Telescop"
#define L_CMD_GOMOF_2 "in miscare!"
#define L_CMD_UNSPF_1 "Goto eroare"
#define L_CMD_UNSPF_2 "necunoscuta!"
#define L_CMD_ERROR   "Eroare"
#define L_CMD_ISAOK_1 "Comanda LX200"
#define L_CMD_ISAOK_2 "Executata!"
#define L_CMD_SETOK_1 "Valoare"
#define L_CMD_SETOK_2 "Setata!"
#define L_CMD_GETOK_1 "Valoare"
#define L_CMD_GETOK_2 "Citita!"
#define L_CMD_SNCOK_1 "Telescop"
#define L_CMD_SNCOK_2 "Sincronizat!"
#define L_CMD_GOTOK_1 "Mergi la"
#define L_CMD_GOTOK_2 "Tinta"

// ----------------------- u8g2_ext_catalog.cpp -------------------------

#define L_CAT_PER_UNK "Perioada Necun."
#define L_CAT_PER_IRR "Perioada Iregul."
#define L_CAT_PER "Perioada"
#define L_CAT_UNK "Necunoscut"
#define L_CAT_OC "Cluster Deschis"
#define L_CAT_GC "Cluster Globular"
#define L_CAT_PN "Nebuloasa Plan."
#define L_CAT_SG "Galaxie Spirala"
#define L_CAT_EG "Galaxie Eliptica"
#define L_CAT_IG "Galaxie Iregul."
#define L_CAT_KNT "Nod"
#define L_CAT_SNR "Rest SuperNova"
#define L_CAT_GAL "Galaxie"
#define L_CAT_CN "Cluster + Nebul."
#define L_CAT_STR "Stea"
#define L_CAT_PLA "Planeta"
#define L_CAT_CMT "Cometa"
#define L_CAT_AST "Asteroid"

// ----------------------- u8g2_ext_input.cpp -------------------------

#define L_RIGHT_ASC "Asc. Dreapta"
#define L_DECLINATION "Declinatie"
#define L_LOCAL_TIME "Ora Locala"
#define L_LATITUDE "Latitudine"
#define L_LONGITUDE "Longitudine"
