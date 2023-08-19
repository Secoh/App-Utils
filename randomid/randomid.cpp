// Copyright 2023 (c) Secoh
//
// This program generates random text pieces of specified length using letters [0-9], [A-Z].
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details. You should have received a copy of the GNU General Public License along with this program.  If not, see
// http://www.gnu.org/licenses/
//

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>

int main(int argn, char *argc[])
{
    if (argn < 2)
    {
        std::cout << "RandomId length [alphabet]\nGenerate random sequence of specified length.\n"
                     "Available characters are 0...9,A...Z, or picked from 2-nd parameter.\n";
        return -1;
    }

    int w = atoi(argc[1]);
    if (w<1)
    {
        std::cerr << "Invalid length.\n";
        return -1;
    }

    std::string A = "";
    if (argn > 2)
    {
        A = argc[2];
    }
    else
    {
        for (unsigned char c='0'; c<='9'; c++) A += c;
        for (unsigned char c='A'; c<='Z'; c++) A += c;
    }

    auto msec = std::chrono::duration_cast<std::chrono::duration<uint64_t, std::milli>>(std::chrono::system_clock::now().time_since_epoch()).count();
    srand((unsigned)(msec & 0x7FFF));

    for (int i=0; i<w; i++) std::cout << A[rand() % A.size()];
    std::cout << "\n";

    return 0;
}
