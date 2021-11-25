#include "Status.h"
#include "../cmd/Cmd.h"

bool Status::featureSelectByOrder(int order) {
  int found = 0;
  for (int n = 0; n < 8; n++) {
    featureSelect(n + 1);
    if (featurePurpose() > 0) found++;
    if (order == found) return true;
  }
  return false;
}

bool Status::featureScan() {
  static bool scan_features = true;

  if (scan_features) {
    char out[40], present[40], cmd[40];
    _featureValid = true;

    if (onStep.Get(":GXY0#", present) == CR_VALUE_GET) {

      if (strlen(present) != 9) { featureClearAll(); return false; }

      VLF("MSG: Aux Feature, scan start");
      for (uint8_t i = 0; i < 8; i++) {
        if (present[i] == '0') continue;

        char *purpose_str = NULL;

        sprintf(cmd, ":GXY%d#", i + 1);
        if ((onStep.Get(cmd, out) != CR_VALUE_GET)) _featureValid = false;
        if (!_featureValid) { featureClearAll(); return false; }

        if (strlen(out) > 1) {
          purpose_str = strstr(out,",");
          if (purpose_str) {
            purpose_str[0] = 0;
            purpose_str++;
          } else _featureValid = false;
          char *name_str = out; if (!name_str) _featureValid = false;

          if (!_featureValid) { featureClearAll(); return false; }

          if (strlen(name_str) > 10) name_str[11] = 0;
          strcpy(feature[i].name, name_str);
          VF("MSG: Aux Feature, name="); VL(feature[i].name);
          if (purpose_str) feature[i].purpose = atoi(purpose_str);
          VF("MSG: Aux Feature, purpose="); VL(feature[i].purpose);

          _featureFound = true;
        }
      }
      VLF("MSG: Aux Feature, scan done");
    }
    scan_features = false;
    return true;
  } else return false;
}

// update auxiliary feature state
// \param index aux feature# 1 to 8, or 0 for all
bool Status::featureUpdate(int index) {
  for (uint8_t i = 0; i < 8; i++) {
    char *value1_str = NULL;
    char *value2_str = NULL;
    char *value3_str = NULL;
    char *value4_str = NULL;
    char out[40], cmd[40];

    if (index == 0 || ((index == i + 1) && (feature[i].purpose == SWITCH || feature[i].purpose == ANALOG_OUTPUT || feature[i].purpose == DEW_HEATER || feature[i].purpose == INTERVALOMETER))) {
      sprintf(cmd, ":GXX%d#", i + 1);
      if ((onStep.Get(cmd, out) != CR_VALUE_GET)) _featureValid = false;
      if (!_featureValid) {
        for (uint8_t j = 0; j < 8; j++) feature[j].purpose = 0;
        return false;
      }

      value2_str = strstr(out, ",");
      if (value2_str) {
        value2_str[0] = 0;
        value2_str++;
        value3_str = strstr(value2_str, ",");
        if (value3_str) {
          value3_str[0] = 0;
          value3_str++;
          value4_str = strstr(value3_str, ",");
          if (value4_str) {
            value4_str[0] = 0;
            value4_str++;
          }
        }
      }
      value1_str = out; if (!value1_str) _featureValid = false;

      if (_featureValid) {
        if (value1_str) feature[i].value1 = atoi(value1_str);
        if (value2_str) feature[i].value2 = atof(value2_str);
        if (value3_str) feature[i].value3 = atof(value3_str);
        if (value4_str) feature[i].value4 = atof(value4_str);
      }
    }
  }
  return true;
}

void Status::featureClearAll() {
  VLF("MSG: Aux Feature, clearing all");
  for (uint8_t j = 0; j < 8; j++) feature[j].purpose = 0;
  _featureFound = false;
}