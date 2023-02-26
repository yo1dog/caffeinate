#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

DWORD parseInputDur(char *);
char *trimwhitespace(char *);

int main(int argc, char *argv[]) {
  DWORD sleepMS = parseInputDur(argc > 1? argv[1] : NULL);
  
  EXECUTION_STATE state = SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED);
  if (state == NULL) {
    cout << "ERROR: Unable to set thread execution state." << endl;
    exit(1);
  }
  
  Sleep(sleepMS);
  
  cout << "Caffeine has worn off." << endl;
  exit(0);
}



DWORD maxSecs  = LONG_MAX /    1000;
DWORD maxMins  = LONG_MAX /   60000;
DWORD maxHours = LONG_MAX / 3600000;

DWORD parseInputDur(char *durInput) {
  if (durInput != NULL) {
    durInput = trimwhitespace(durInput);
  }
  if (
    durInput == NULL ||
    strlen(durInput) == 0 ||
    strcmp(durInput, "forever") == 0
  ) {
    cout << "Caffeinated forever." << endl;
    return INFINITE;
  }
  if (
    strcmp(durInput, "help"  ) == 0 ||
    strcmp(durInput, "-help" ) == 0 ||
    strcmp(durInput, "--help") == 0
  ) {
    cout << "Usage: caffeinate.exe [duration]" << endl;
    cout << "- Duration may be a whole number with an optional unit, or the word \"forever\"." << endl;
    cout << "- If duration is not given, \"forever\" is used." << endl;
    cout << "- Valid units: s (seconds), m (minutes), h (hours)." << endl;
    cout << "- If a unit is not given, seconds is used." << endl;
    cout << endl;
    cout << "Examples: " << endl;
    cout << "caffeinate.exe         # Forever" << endl;
    cout << "caffeinate.exe forever # Forever" << endl;
    cout << "caffeinate.exe 30      # 30 seconds" << endl;
    cout << "caffeinate.exe 30s     # 30 seconds" << endl;
    cout << "caffeinate.exe 120s    # 2 minutes" << endl;
    cout << "caffeinate.exe 2m      # 2 minutes" << endl;
    cout << "caffeinate.exe 36h     # 36 hours" << endl;
    exit(0);
  }
  
  char *durSuffix = NULL;
  long durValue;
  errno = 0;
  durValue = strtol(durInput, &durSuffix, 10);
  if (
    errno != 0 ||
    durSuffix == durInput || // Check if no number was found.
    durValue < 0 ||
    durValue > maxSecs ||
    *durSuffix == '.'
  ) {
    cout << "Input duration must be a whole number >= 0 and <= " << maxSecs << "." << endl;
    exit(1);
  }
  
  durSuffix = trimwhitespace(durSuffix);
  if (strlen(durSuffix) > 1) {
    cout << "Unrecongized duration suffix \"" << durSuffix << "\". Must be one of: \"s\", \"m\", \"h\"" << endl;
    exit(1);
  }
  char durUnit = *durSuffix;
  
  DWORD sleepMS;
  if (durUnit == 's' || durUnit == 'S' || durUnit == 0) {
    if (durValue > maxSecs) {
      cout << "Can not specify more than " << maxSecs << " seconds." << endl;
      exit(1);
    }
    sleepMS = durValue * 1000;
  }
  else if (durUnit == 'm' || durUnit == 'M') {
    if (durValue > maxMins) {
      cout << "Can not specify more than " << maxMins << " minutes." << endl;
      exit(1);
    }
    sleepMS = durValue * 60000;
  }
  else if (durUnit == 'h' || durUnit == 'H') {
    if (durValue > maxHours) {
      cout << "Can not specify more than " << maxHours << " hours." << endl;
      exit(1);
    }
    sleepMS = durValue * 3600000;
  }
  else {
    cout << "Unrecongized duration suffix \"" << durSuffix << "\". Must be one of: \"s\", \"m\", \"h\"" << endl;
    exit(1);
  }
  
  DWORD seconds;
  DWORD minutes;
  DWORD hours;
  seconds = sleepMS / 1000;
  minutes = seconds / 60;
  seconds = seconds % 60;
  hours   = minutes / 60;
  minutes = minutes % 60;
  
  cout << "Caffeinated for";
  if (sleepMS == 0) {
    cout << " 0 seconds";
  }
  else {
    if (hours == 1) cout << " 1 hour";
    else if (hours > 1) cout << " " << hours << " hours";
    if (minutes == 1) cout << " 1 minute";
    else if (minutes > 1) cout << " " << minutes << " minutes";
    if (seconds == 1) cout << " 1 second";
    else if (seconds > 1) cout << " " << seconds << " seconds";
  }
  cout << "." << endl;
  
  return sleepMS;
}

// https://stackoverflow.com/a/122721/2544290
char *trimwhitespace(char *str) {
  char *end;
  
  // Trim leading space
  while(isspace((unsigned char)*str)) str++;
  
  if(*str == 0)  // All spaces?
    return str;
  
  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;
  
  // Write new null terminator character
  end[1] = '\0';
  
  return str;
}
