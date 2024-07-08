#pragma once

#include<memory>
#include<functional>


#define FD_BIND_EVENT_FD(fn) std::bind(&fn, this, std::placeholders::_1)
#define BIT(x) (1u << x)

namespace ForgeDelta {

}

