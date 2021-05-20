// -----------------------------------------------------------------------------------
// CatalogTypes, data types for catalogs
#pragma once

// maximum number of catalogs supported
// there is a matching array in subMenuSyncGoto() that also needs adjustment if this is increased
#define MaxCatalogs 64

// ----------------------------------------------------------
// Do not change anything in the structs or arrays below, since they
// have to be in sync with the extraction scripts.

// Struct for catalog header
typedef struct {
  const char           Title[32];
  const char*          Prefix;
  const unsigned short NumObjects;
  const void*          Objects;
  const char*          ObjectNames;
  const char*          ObjectSubIds;
  CAT_TYPES            CatalogType;
  int                  Epoch;
  long                 Index;
} catalog_t;

#pragma pack(1)

// Struct for Deep Space Objects (Messier, Herschel, ..etc.)
typedef struct {
  const unsigned char  Has_name: 1;
  const unsigned char  Cons: 7;
  const unsigned char  Obj_type: 7;
  const unsigned char  Has_subId: 1;
  const unsigned short Obj_id;
  const signed   short Mag;
  const float          RA;
  const float          DE;
} dso_t; // 14 bytes/rec
typedef struct {
  const unsigned char  Has_name: 1;
  const unsigned char  Cons: 7;
  const unsigned char  Obj_type: 7;
  const unsigned char  Has_subId: 1;
  const unsigned short Obj_id;
  const unsigned char  Mag;
  const unsigned short RA;
  const signed   short DE;
} dso_comp_t; // compact, 9 bytes/rec
typedef struct {
  const unsigned char  Has_name: 1;
  const unsigned char  Cons: 7;
  const unsigned char  Obj_type: 7;
  const unsigned char  Has_subId: 1;
  const unsigned char  Mag;
  const unsigned short RA;
  const signed   short DE;
} dso_vcomp_t; // very compact, 7 bytes/rec

// Struct for general star catalog
typedef struct {
  const unsigned long  Has_name: 1;
  const unsigned long  Cons: 7;
  const unsigned long  BayerFlam: 8; // 0 to 23 are coded Bayer, 24=Invalid, 25 to 255 are the Flamsteed# (for 1 to 231.)
  const unsigned long  Has_subId: 1;
  const unsigned long  Obj_id: 15;   // to 32767
  const signed   short Mag;
  const float          RA;
  const float          DE;
} gen_star_t; // 14 bytes/rec
typedef struct {
  const unsigned char  Has_name: 1;
  const unsigned char  Cons: 7;
  const unsigned char  BayerFlam: 7; // 0 to 23 are coded Bayer, 24=Invalid, 25 to 127 are the Flamsteed# (for 1 to 103.)
  const unsigned char  Has_subId: 1;
  const unsigned char  Mag;
  const unsigned short RA;
  const signed   short DE;
} gen_star_vcomp_t; // very compact, 7 bytes/rec

// Struct for double star catalog
typedef struct {
  const unsigned long  Has_name: 1;
  const unsigned long  Cons: 7;
  const unsigned long  BayerFlam: 8; // 0 to 23 are coded Bayer, 24=Invalid, 25 to 255 are the Flamsteed# (for 1 to 231.)
  const unsigned long  Has_subId: 1;
  const unsigned long  Obj_id: 15;   // to 32767

  const unsigned int   Sep: 14;      // separation of components 0.0 to 999.8" (0 to 9998), 999.9 (9999) = Unknown
  const unsigned int   PA: 9;        // position angle in degrees, -1 = Unknown
  const signed short   Mag2;

  const signed short   Mag;
  const float          RA;
  const float          DE;
} dbl_star_t; // 18 bytes per record
typedef struct {
  const unsigned long  Has_name: 1;
  const unsigned long  Cons: 7;
  const unsigned long  BayerFlam: 8; // 0 to 23 are coded Bayer, 24=Invalid, 25 to 255 are the Flamsteed# (for 1 to 231.)
  const unsigned long  Has_subId: 1;
  const unsigned long  Obj_id: 15;   // to 32767

  const unsigned long  Sep: 14;      // separation of components 0.0 to 999.8" (0 to 9998), 999.9 (9999) = Unknown
  const unsigned long  PA: 9;        // position angle in degrees, 361 = Unknown
  const unsigned long  Mag2: 8;

  const unsigned char  Mag;
  const unsigned short RA;
  const signed   short DE;
} dbl_star_comp_t; // compact, 13 bytes per record

// Struct for variable star catalog
typedef struct {
  const unsigned long  Has_name: 1;
  const unsigned long  Cons: 7;
  const unsigned long  BayerFlam: 8; // 0 to 23 are coded Bayer, 24=Invalid, 25 to 255 are the Flamsteed# (for 1 to 231.)
  const unsigned long  Has_subId: 1;
  const unsigned long  Obj_id: 15;   // to 32767

  const unsigned int   Period: 15;   // Period 0.00 to 9.99 days (0 to 999) period 10.0 to 3186.5 days (1000 to 32765), 32766 = Irregular, 32767 = Unknown
  const signed short   Mag2;

  const signed short   Mag;
  const float          RA;
  const float          DE;
} var_star_t; // 18 bytes per record
typedef struct {
  const unsigned long  Has_name: 1;
  const unsigned long  Cons: 7;
  const unsigned long  BayerFlam: 8; // 0 to 23 are coded Bayer, 24=Invalid, 25 to 255 are the Flamsteed# (for 1 to 231.)
  const unsigned long  Has_subId: 1;
  const unsigned long  Obj_id: 15;   // to 32767

  const unsigned int   Period: 15;   // Period 0.00 to 9.99 days (0 to 999) period 10.0 to 3186.5 days (1000 to 32765), 32766 = Irregular, 32767 = Unknown
  const unsigned char  Mag2;

  const unsigned char  Mag;
  const unsigned short RA;
  const signed   short DE;
} var_star_comp_t; // compact, 12 bytes per record

#pragma pack()
