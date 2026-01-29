#pragma once

namespace BEM_3D
{
    class OOC_File {
    public:
        OOC_File(LPCTSTR szFileName);
        virtual ~OOC_File();
        
        // Open/Close and initiation helpers
        void Open(bool bWriteAccess, bool bCreateNew);
        void Close();
        void Init(uint64_t nBytes);


        // Input/Output helpers
        void WriteAt(uint64_t offset, const void* buffer, DWORD bytes);
        void ReadAt(uint64_t offset, void* buffer, DWORD bytes) const;



    private:
        LPCTSTR m_szFileName;
        HANDLE m_hFile;
    };
};



