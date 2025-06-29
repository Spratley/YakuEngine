#pragma once

#define CRTP_CALL(ChildType) static_cast<ChildType*>(this)