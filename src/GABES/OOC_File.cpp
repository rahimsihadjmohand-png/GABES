#include "pch.h"
#include "OOC_File.h"

using namespace BEM_3D;

OOC_File::OOC_File(LPCTSTR szFileName)
    : m_szFileName(szFileName)
    , m_hFile(INVALID_HANDLE_VALUE)
{
}

void OOC_File::Open(bool bWriteAccess, bool bCreateNew)
{
    DWORD access = bWriteAccess ? (GENERIC_READ | GENERIC_WRITE) : GENERIC_READ;
    DWORD creation = bCreateNew ? CREATE_ALWAYS : OPEN_ALWAYS;

    m_hFile = CreateFileW(
        m_szFileName,
        access,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        creation,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS | FILE_FLAG_OVERLAPPED,
        nullptr
    );
}

void OOC_File::Close()
{
    if (m_hFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hFile);
        m_hFile = INVALID_HANDLE_VALUE;
    }
}

OOC_File::~OOC_File()
{
    Close();
}


void OOC_File::WriteAt(uint64_t offset, const void* buffer, DWORD bytes)
{
    if (m_hFile == INVALID_HANDLE_VALUE || !buffer || bytes == 0)
        return;

    OVERLAPPED ov = {};
    ov.Offset = static_cast<DWORD>(offset & 0xFFFFFFFFULL);
    ov.OffsetHigh = static_cast<DWORD>(offset >> 32);

    const BYTE* src = static_cast<const BYTE*>(buffer);
    DWORD totalWritten = 0;

    while (totalWritten < bytes)
    {
        DWORD toWrite = bytes - totalWritten;
        DWORD writtenNow = 0;

        BOOL ok = WriteFile(m_hFile, src + totalWritten, toWrite, &writtenNow, &ov);

        if (!ok)
        {
            DWORD err = GetLastError();
            if (err == ERROR_IO_PENDING)
            {
                // Wait until the write completes
                ok = GetOverlappedResult(m_hFile, &ov, &writtenNow, TRUE);
            }
        }

        // If still failed, retry (loop continues indefinitely)
        if (!ok || writtenNow == 0)
        {
            // Optional: short sleep to avoid busy-waiting
            //Sleep(1);
            continue;
        }

        totalWritten += writtenNow;

        // Advance overlapped offset
        uint64_t newOffset = offset + totalWritten;
        ov.Offset = static_cast<DWORD>(newOffset & 0xFFFFFFFFULL);
        ov.OffsetHigh = static_cast<DWORD>(newOffset >> 32);
    }
}


void OOC_File::ReadAt(uint64_t offset, void* buffer, DWORD bytes) const
{
    if (m_hFile == INVALID_HANDLE_VALUE || !buffer || bytes == 0)
        return;

    OVERLAPPED ov = {};
    ov.Offset = static_cast<DWORD>(offset & 0xFFFFFFFFULL);
    ov.OffsetHigh = static_cast<DWORD>(offset >> 32);

    BYTE* dst = static_cast<BYTE*>(buffer);
    DWORD totalRead = 0;

    while (totalRead < bytes)
    {
        DWORD toRead = bytes - totalRead;
        DWORD readNow = 0;

        BOOL ok = ReadFile(m_hFile, dst + totalRead, toRead, &readNow, &ov);

        if (!ok)
        {
            DWORD err = GetLastError();
            if (err == ERROR_IO_PENDING)
            {
                ok = GetOverlappedResult(m_hFile, &ov, &readNow, TRUE);
            }
        }

        // Retry until success
        if (!ok || readNow == 0)
        {
            //Sleep(1);  // avoid busy wait
            continue;
        }

        totalRead += readNow;

        // Advance overlapped offset
        uint64_t newOffset = offset + totalRead;
        ov.Offset = static_cast<DWORD>(newOffset & 0xFFFFFFFFULL);
        ov.OffsetHigh = static_cast<DWORD>(newOffset >> 32);
    }
}


// ============================================================
// Init : Pre-allocate file of given size
// ============================================================
void OOC_File::Init(uint64_t nBytes)
{
    Open(true, true);  

    if (nBytes == 0)
    {
        SetEndOfFile(m_hFile);
        Close();
        return;
    }

    OVERLAPPED ov = {};
    ov.Offset = (DWORD)((nBytes - 1) & 0xFFFFFFFF);
    ov.OffsetHigh = (DWORD)(((nBytes - 1) >> 32) & 0xFFFFFFFF);
    ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    BYTE zero = 0;
    DWORD written = 0;

    BOOL ok = WriteFile(
        m_hFile,
        &zero,
        1,
        &written,     // will not be used if async
        &ov
    );

    if (!ok)
    {
        if (GetLastError() == ERROR_IO_PENDING)
        {
            // Wait for completion of the async write
            WaitForSingleObject(ov.hEvent, INFINITE);

            DWORD written2 = 0;
            if (!GetOverlappedResult(m_hFile, &ov, &written2, TRUE) || written2 != 1)
            {
                CloseHandle(ov.hEvent);
                Close();
                return;
            }
        }
        else
        {
            // WriteFile failed immediately
            CloseHandle(ov.hEvent);
            Close();
            return;
        }
    }

    // File size is now nBytes (kernel expanded)

    CloseHandle(ov.hEvent);
    Close();
}
