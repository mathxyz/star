#include "liCommonInner.h"

void libeep() {
#ifdef LI_WINDOWS
	Beep(750, 300);
#endif
}

void libeepMusic() {
#ifdef LI_WINDOWS
	Beep(523, 500);
	Beep(587, 500);
	Beep(659, 500);
	Beep(698, 500);
	Beep(784, 500);
	Sleep(500);
	Beep(523, 500);
	Beep(587, 500);
	Beep(659, 500);
	Beep(698, 500);
	Beep(784, 500);
#endif
}
