#include"MonitorInstance.h"

MonitorInstance::MonitorInstance() {
	HWND buffer = 0;
	handle = MonitorFromWindow(buffer, MONITOR_DEFAULTTOPRIMARY);
	info = { sizeof(MONITORINFO) };

	if (!GetMonitorInfo(handle, &info)) {
		return;
	}
}