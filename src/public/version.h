/*
*	OXWARE developed by oxiKKK
*	Copyright (c) 2023
* 
*	This program is licensed under the MIT license. By downloading, copying, 
*	installing or using this software you agree to this license.
*
*	License Agreement
*
*	Permission is hereby granted, free of charge, to any person obtaining a 
*	copy of this software and associated documentation files (the "Software"), 
*	to deal in the Software without restriction, including without limitation 
*	the rights to use, copy, modify, merge, publish, distribute, sublicense, 
*	and/or sell copies of the Software, and to permit persons to whom the 
*	Software is furnished to do so, subject to the following conditions:
*
*	The above copyright notice and this permission notice shall be included 
*	in all copies or substantial portions of the Software. 
*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
*	OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
*	THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
*	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
*	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
*	IN THE SOFTWARE.
*/

#ifndef VERSION_H
#define VERSION_H
#pragma once

// https://semver.org/
// MAJOR version when you make incompatible API changes, a lot of stuff changed/added
// MINOR version when you add functionality, bigger improvements
// PATCH version when you make bug fixes, small improvements

#define OXVER_MAJOR		1
#define OXVER_MINOR		0
#define OXVER_PATCH		1
#define OXVER_STRING	"1.0.1" // don't forget to change, too!

#if defined(_DEBUG)
#define OX_BUILD "debug"
#elif defined(_RETAIL)
#define OX_BUILD "retail"
#else
#define OX_BUILD "release"
#endif

#define OX_COMPILE_TIMESTAMP __TIME__ " | " __DATE__

#endif // VERSION_H