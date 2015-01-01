
#include <windows.h>
#include <winioctl.h>
#include <string.h>

#include "types.h"
#include "gensandbox.h"

int gensandbox_mouse_act() {
	POINT position1, position2;
	GetCursorPos(&position1);
	Sleep(2000); /* Sleep time */
	GetCursorPos(&position2);
	if ((position1.x == position2.x) && (position1.y == position2.y)) {
		/* No mouse activity during the sleep */
		return TRUE;
	}
	else {
		/* Mouse activity during the sleep */
		return FALSE;
	}
}

int gensandbox_username() {
	char username[200];
	int i;
	DWORD usersize = sizeof(username);
	GetUserName(username, &usersize);
	for (i = 0; i < strlen(username); i++) { /* case-insensitive */
		username[i] = toupper(username[i]);
	}
	if (strstr(username, "SANDBOX") != NULL) {
		return TRUE;
	}
	if (strstr(username, "VIRUS") != NULL) {
		return TRUE;
	}
	if (strstr(username, "MALWARE") != NULL) {
		return TRUE;
	}
	return FALSE;
}

int gensandbox_path() {
	char path[500];
	int i;
	DWORD pathsize = sizeof(path);
	GetModuleFileName(NULL, path, pathsize);
	for (i = 0; i < strlen(path); i++) { /* case-insensitive */
		path[i] = toupper(path[i]);
	}
	if (strstr(path, "\\SAMPLE") != NULL) {
		return TRUE;
	}
	if (strstr(path, "\\VIRUS") != NULL) {
		return TRUE;
	}
	if (strstr(path, "SANDBOX") != NULL) {
		return TRUE;
	}
	return FALSE;
}

int gensandbox_drive_size() {
	HANDLE drive;
	BOOL result;
	GET_LENGTH_INFORMATION size;
	DWORD lpBytesReturned;

	drive = CreateFile("\\\\.\\PhysicalDrive0", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (drive == INVALID_HANDLE_VALUE) {
		// Someone is playing tricks. Or not enough privileges.
		CloseHandle(drive);
		return FALSE;
	}
	result = DeviceIoControl(drive, IOCTL_DISK_GET_LENGTH_INFO, NULL, 0, &size,
	sizeof(GET_LENGTH_INFORMATION), &lpBytesReturned, NULL);
	CloseHandle(drive);
	if (result != 0) {
		if (size.Length.QuadPart / 1073741824 <= 60) /* <= 60 GB */
		return TRUE;
	}
	return FALSE;
}

int gensandbox_drive_size2() {
	ULARGE_INTEGER total_bytes;

	if (GetDiskFreeSpaceExA("C:\\", NULL, &total_bytes, NULL))
	{
		if (total_bytes.QuadPart / 1073741824 <= 60) /* <= 60 GB */
		return TRUE;
	}
	return FALSE;
}

int gensandbox_sleep_patched() {
	DWORD time1;

	time1 = GetTickCount();
	Sleep(500);
	if ((GetTickCount() - time1) > 450 ) return FALSE;
	else return TRUE;
}
