// SPDX-License-Identifier: MIT
//
// Copyright 2024 dgz
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the “Software”), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

/// @file compiler.h Define various abstractions around compiler features for
/// easier readability and use.

#pragma once

// clang-format off

/// @brief This branch is unlikely to be executed.
///
/// Improves compiler optimization decisions; ignored when profile-guided
/// optimization is used.
#define unlikely(x)	__builtin_expect(!!(x), 0)

/// @brief This branch is likely to be executed.
///
/// Improves compiler optimization decisions; ignored when profile-guided
/// optimization is used.
#define likely(x)	__builtin_expect(!!(x), 1)

#define ATTRIB_FMT(type, idx, first) __attribute__((format(type, idx, first)))

// clang-format on
