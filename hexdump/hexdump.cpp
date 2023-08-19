// Copyright 2023 (c) Secoh
//
// This program exports binary file to text formatted as classical hex dump: each byte is represented as uppercase
// hexademical value, values separated by space, each line represents "width" bytes.
// Notable application of hexdump is comparison of binary files by standard diff, when files are similar.
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
#include <fstream>
#include <vector>

void hx(int t)
{
    std::cout << "0123456789ABCDEF"[t & 0x0F];
}

void pr(const std::vector<int>& d, int w, int n)
{
    if (!n) return;

    for (int i=0; i<w; i++)
        if (i >= n)
        {
            std::cout << "   ";
        }
        else
        {
            hx(d[i] / 16);
            hx(d[i]);
            std::cout << " ";
        }

    for (int i = 0; i < n; i++) std::cout << (char)((d[i] >= 0x20 && d[i] < 0x7F) ? d[i] : '.');

    std::cout << "\n";
}

int main(int argn, char *argc[])
{
    if (argn < 2)
    {
        std::cout << "HD file [width]\nDump the file in hex bytes.\nWidth default is 16.\n";
        return -1;
    }

    std::ifstream f(argc[1], std::fstream::binary);
    if (!f.is_open())
    {
        std::cerr << "Cannot open input file.\n";
        return -1;
    }

    int w = (argn > 2 ? atoi(argc[2]) : 16);
    if (w<1)
    {
        std::cerr << "Invalid width.\n";
        return -1;
    }

    std::vector<int> d(w);
    int i = 0;
    char c;

    while (f.get(c))
    {
        if (i >= w)
        {
            pr(d, w, w);
            i = 0;
        }

        d[i++] = (unsigned char)c;
    }
    pr(d, w, i);

    return 0;
}

