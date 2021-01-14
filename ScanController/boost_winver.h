#pragma once

// For WINNT instead WINDOWS for ASIO support
#ifdef _WIN64

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif

#endif

