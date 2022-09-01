// fstomp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <chrono>

static constexpr int SectorSize = 512;
unsigned char Sector[SectorSize];

static constexpr int Nletters = 'Z' - 'A' + 1;
static constexpr int Rbase = 8192;
static constexpr int Rcr = 410;
static constexpr int Rspan = 2 * Nletters + 10;

unsigned char Alphabet[Rspan];

static constexpr int64_t ReportGranularity = 1024LL * 1024;  // 1 Meg

int wmain(int argn, wchar_t* argc[])
{
    int k = 0;
    for (int i = 0; i < 10; i++) Alphabet[k++] = '0' + i;
    for (int i = 0; i < Nletters; i++) Alphabet[k++] = 'A' + i;
    for (int i = 0; i < Nletters; i++) Alphabet[k++] = 'a' + i;
    if (k != Rspan)
    {
        std::wcerr << L"Internal error\n";
        return -1;
    }

//    srand((unsigned)time(nullptr));

    auto msec = std::chrono::duration_cast<std::chrono::duration<uint64_t, std::milli>>(std::chrono::system_clock::now().time_since_epoch()).count();
    srand((unsigned)(msec & 0x7FFF));

    if (argn < 2)
    {
        std::wcerr << L"Overwrites file with garbage and flushes.\nUsage: fstomp <file_to_destroy>\n";
        return 0;
    }

    const wchar_t *filename = argc[1];

    FILE* f = _wfopen(filename, L"rb");
    if (!f)
    {
        std::wcerr << L"File " << filename << L" doesn\'t exist\n";
        return 0;
    }

    if (_fseeki64(f, 0, SEEK_END))
    {
        std::wcerr << L"File " << filename << L" doesn\'t support SEEK, or is not a file\n";
        return 0;
    }

    int64_t LEN = _ftelli64(f);
    if (LEN <= 0)
    {
        std::wcerr << L"File " << filename << L" has zero length, or is not a file\n";
        return 0;
    }

    fclose(f);

    f = _wfopen(filename, L"r+b");
    if (!f)
    {
        std::wcerr << L"ERROR -- Cannot open file for writing: " << filename << L"\n";
        return 0;
    }

    const wchar_t* slpos = wcsrchr(filename, L'\\');
    std::wstring shortname = (slpos ? slpos+1 : filename);

    int64_t Count = (LEN + SectorSize-1) / SectorSize;
    int64_t ReportMark = ReportGranularity;

    for (int64_t N = 0; N < Count; N++)
    {
        int64_t progress = N * SectorSize;
        if (progress >= ReportMark)
        {
            std::wcout << shortname << L" " << std::to_wstring(progress) << L"\r" << std::flush;
            ReportMark += ReportGranularity;
        }

        for (int i = 0; i < SectorSize; i++)
        {
            if (i < SectorSize - 1 && (rand() % Rbase) < Rcr)
            {
                Sector[i++] = '\r';
                Sector[i] = '\n';
            }
            else
            {
                Sector[i] = Alphabet[rand() % Rspan];
            }
        }

        if (fwrite(Sector, 1, SectorSize, f) != SectorSize || fflush(f) != 0)
        {
            std::wcerr << L"ERROR -- Cannot write to file " << filename << L"\n";
            return 0;
        }
    }

    std::wcout << shortname << L" " << std::to_wstring(Count*SectorSize) << std::endl;

    fclose(f);

    return 0;
}

