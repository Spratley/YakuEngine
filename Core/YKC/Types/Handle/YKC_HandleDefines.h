#pragma once

#define YK_HANDLE_PAGE_SIZE 32

#define YK_DECLARE_HANDLE(DataType) \
using DataType##Handle = YK_HandleBase<DataType, YK_HANDLE_PAGE_SIZE>; \
using DataType##HandledStorage = YK_HandledStorage<DataType, YK_HANDLE_PAGE_SIZE>