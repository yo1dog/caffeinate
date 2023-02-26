# Caffeinate

A super simple utility for keeping your PC awake.

<img src="coffee.ico" width="48" />

All it does is:
```c++
SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED);
Sleep(...);
```

The computer is prevented from sleeping for as long as the program remains running. The display/monitor is still allowed to turn off. `SetThreadExecutionState` docs [here](https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-setthreadexecutionstate).


## Download

Download from the latest GitHub release (runs on 32 and 64bit):
 - [caffeinate.exe](https://github.com/yo1dog/caffeinate/releases/latest/download/caffeinate.exe)

**Note:** You may have to "unblock" the program after downloading: Right-click the exe file → Properties → Check "Unblock" at bottom right.


## Usage

```
caffeinate.exe [duration] [--help]
```

The program will remain open and prevent sleep for the given duration, or forever if a duration is not given.
Use `SIGINT` (ctrl+c), `SIGHUP`, `SIGTERM`, etc. to kill at any time.

- Duration may be a whole number with an optional unit, or the word `forever`.
- If duration is not given, `forever` is used.
- Valid units: `s` (seconds), `m` (minutes), `h` (hours)
- If a unit is not given, seconds is used.

Examples:

```bash
caffeinate.exe         # Forever
caffeinate.exe forever # Forever
caffeinate.exe 30      # 30 seconds
caffeinate.exe 30s     # 30 seconds
caffeinate.exe 120s    # 2 minutes
caffeinate.exe 2m      # 2 minutes
caffeinate.exe 36h     # 36 hours
```

## WSL

**Optional:** Here is a small bash script for managing Caffeinate from WSL. Calling this script will kill all Caffeinate instances and start a new one in the background, thus ensuring only one is running and allowing you to reset or end the caffeinated duration.

 - [caff](caff)

Please modify to set the correct `caffeinate.exe` path and default duration as desired.

The interface nearly identical to `caffeinate.exe`, but the script may default the duration to a value other than `forever`. Also, if a duration of `0` is given, all Caffeinate instances are killed but a new one is not started.
