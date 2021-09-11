//--------------------------------------------
// Copyright (c) 2021 Dove All rights reserved.
//--------------------------------------------
#pragma once

#ifndef WIN
#define LINUX
#endif

#include <stdio.h> 
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#ifdef WIN
#include <ShlObj_core.h>
#endif

#ifdef LINUX
using PATH_STRING = std::string;
inline PATH_STRING get_config_path() {
	std::string path = getenv("HOME");
	path.append("/.dtconfig");
	return path;
}
inline std::string fn_conv_to_utf8(std::string str) { return str; }
#endif

#ifdef WIN
using PATH_STRING = std::wstring;
inline PATH_STRING get_config_path() {
	PWSTR path_user;

	SHGetKnownFolderPath(FOLDERID_Profile, 0, nullptr, &path_user);
	std::wstring path(path_user);

	path.append(L"\\.dtconfig");
	return path;
}

inline std::string fn_conv_to_utf8(std::string str) {
	int size = MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str.c_str(),
								   str.length(), nullptr, 0);
	std::wstring u16_str(size, '\0');
	MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str.c_str(), str.length(),
						&u16_str[0], size);

	int u8_size =
		WideCharToMultiByte(CP_UTF8, 0, u16_str.c_str(), u16_str.length(),
							nullptr, 0, nullptr, nullptr);
	std::string u8_str(u8_size, '\0');
	WideCharToMultiByte(CP_UTF8, 0, u16_str.c_str(), u16_str.length(),
						&u8_str[0], u8_size, nullptr, nullptr);
	return u8_str;
}
#endif
