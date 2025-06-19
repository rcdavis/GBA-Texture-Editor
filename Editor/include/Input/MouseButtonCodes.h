#pragma once

#include "Core.h"

using MouseCode = uint32;

namespace Mouse {
	enum : MouseCode {
		Button1,
		Button2,
		Button3,
		Button4,
		Button5,
		Button6,
		Button7,
		Button8,

		ButtonLast = Button8,
		ButtonLeft = Button1,
		ButtonRight = Button2,
		ButtonMiddle = Button3
	};
}
