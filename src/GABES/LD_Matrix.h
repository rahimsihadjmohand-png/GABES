#pragma once
#include "framework.h"
#include "OOC_File.h"
#include <fstream>

// LD : Stands for Large and Dense Matrix

namespace BEM_3D
{
	// ============ Declaration and Implementation of the LD_Column class ==========================
	template <typename T> 
	struct LD_Column
	{
	public:
		LD_Column(const LD_Column& rhs);
		LD_Column(bool bOOC, unsigned N, T* Aj);
		virtual ~LD_Column();

		LD_Column& operator = (const LD_Column& rhs);
		T operator[] (unsigned i)const;
		const T* data()const { return m_Aj; }
		T* data() { return m_Aj; }


	private:
		bool m_bOOC; // This boolean tells if the column is stored Out Of Core or not
		T* m_Aj;     
		unsigned m_N; 
	};
	
	template <typename T>
	LD_Column<T>::LD_Column(const LD_Column<T>& rhs)
		: m_bOOC(rhs.m_bOOC)
		, m_N(rhs.m_N)
	{
		if (m_bOOC)
		{
			// Deep copy (OOC owns its data)
			m_Aj = new T[m_N];
			CopyMemory(m_Aj, rhs.m_Aj, sizeof(T) * m_N);
		}
		else
		{
			// Shallow copy (RAM data owned by LD_Matrix)
			m_Aj = rhs.m_Aj;
		}
	}


	template <typename T>
	LD_Column<T>::LD_Column(bool bOOC, unsigned N, T* Aj)
		: m_bOOC(bOOC)
		, m_N(N)
		, m_Aj(Aj)
	{

	}

	template <typename T>
	LD_Column<T>::~LD_Column()
	{
		// The column is freed only if it is originated From Disk file
		if (m_bOOC)
		{
			delete[] m_Aj;
			m_Aj = nullptr;
		}
	}

	template <typename T>
	LD_Column<T>& LD_Column<T>::operator = (const LD_Column<T>& rhs)
	{
		if (this == &rhs)
			return *this;

		if (m_bOOC)
			delete[] m_Aj;

		m_bOOC = rhs.m_bOOC;
		m_N = rhs.m_N;

		if (m_bOOC)
		{
			m_Aj = new T[m_N];
			CopyMemory(m_Aj, rhs.m_Aj, sizeof(T) * m_N);
		}
		else
		{
			m_Aj = rhs.m_Aj; // shallow copy
		}

		return *this;
	}


	template <typename T>
	T LD_Column<T>::operator[] (unsigned i)const
	{
		return m_Aj[i];
	}

	// ==============================================================================


	//================ Declaration and Implementation of the LD_Matrix class ========================
	template <typename T>
	class LD_Matrix
	{
	public:
		LD_Matrix(unsigned maxRamAlloc, LPCTSTR szFileName, double* pCurrAdv = nullptr);
		LD_Matrix(unsigned N, unsigned M, unsigned maxRamAlloc, LPCTSTR szFileName, double* pCurrAdv = nullptr);
		virtual ~LD_Matrix();

		LD_Matrix(const LD_Matrix&) = delete;
		LD_Matrix& operator=(const LD_Matrix&) = delete;


		// Initializes the matrix
		void SetRamQuota(unsigned maxRamAlloc);
		void Initialize(unsigned N, unsigned M);  // N, M: the matrix dimensions

		// Make the matrix all Zeros, the matrix is assumed initialized!! 
		void ZeroMatrix();

		// Clean helper routine
		void CleanRamStorage();

		// Keep in Mind that m_File must be opened before any operations on Input/Output!!

		// Getters 
		LD_Column<T> operator[] (unsigned j)const; // It returns a pointer to the column of index j
		// IMPORTANT!!! : if the column is stored in RAM keep it allocated
		//                otherwise this overloaded operator returns an allocated pointer
		//                containing data copied from the Disk file, in this case we have to
		//                free this memory

		const T Get(unsigned j, unsigned i) const;

		unsigned GetMaxRamAlloc()const { return m_MaxRamAlloc; }

		// Setters
		void Set(unsigned j, unsigned i, T Val);
		void SetColumn(unsigned j, const LD_Column<T>& Aj);
		void SetColumn(unsigned j, T* Aj);
		void IncrementColumn(unsigned j, const LD_Column<T>& Aj);
		void IncrementColumn(unsigned j, T* Aj);
		void SetChunck(unsigned j_beg, unsigned n, T** Chunck);
		void Add(unsigned j, unsigned i, T Val);
		void Subtract(unsigned j, unsigned i, T Val);

		// AHMED A-Matrix method
		void cmpbl(unsigned b1, unsigned n1, unsigned b2, unsigned n2, T* data);
		T scale(unsigned b1, unsigned n1, unsigned b2, unsigned n2);


		// File Management functions
		void OpenFile(bool bWriteAccess);
		void CloseFile();
		void Print(LPCTSTR szFileName)const;



	private:
		unsigned m_N;            // the Row Dimension (the number of rows)
		unsigned m_M;            // the Column Dimension (the number of columns)
		unsigned m_MaxRamAlloc;  // The Maximum permitted storage in RAM expressed in Megabytes
		unsigned m_Offset_OOC;   // The Column index where the out of core storage begins
		T** m_A;                 // The Portion of the Matrix allocated on the Heap
		OOC_File  m_File;        // The File Where the OOC portion is stored 

	public:
		double* m_pCurrentAdv;   // This is the adrress to the current advance in Model class 
		                         // (It will be used inside the AHMED Library for setting up the progression state)
	};



	template <typename T>
	LD_Matrix<T>::LD_Matrix(unsigned maxRamAlloc, LPCTSTR szFileName, double* pCurrAdv)
		: m_N(0)
		, m_M(0)
		, m_MaxRamAlloc(maxRamAlloc)
		, m_Offset_OOC(0)
		, m_A(nullptr)
		, m_File(szFileName)
		, m_pCurrentAdv(pCurrAdv)
	{
		
	}




	template <typename T>
	LD_Matrix<T>::LD_Matrix(unsigned N, unsigned M,  unsigned maxRamAlloc, LPCTSTR szFileName, double* pCurrAdv)
		: m_N(N)
		, m_M(M)
		, m_MaxRamAlloc(maxRamAlloc)
		, m_File(szFileName)
		, m_pCurrentAdv(pCurrAdv)
	{
		// Calculate the offset index in out of core storage
		uint64_t tempOffset = (uint64_t)m_MaxRamAlloc * 1024 * 1024 / (m_N * sizeof(T));
		m_Offset_OOC = (unsigned)(tempOffset);

		// Allocate Memory for Inner core storage and preset its contents to 0
		unsigned nRamCols = min(m_M, m_Offset_OOC);
		if (nRamCols != 0)
		{
			m_A = new T * [nRamCols];
			for (unsigned j = 0; j < nRamCols; j++)
			{
				m_A[j] = new T[m_N];
				ZeroMemory(m_A[j], sizeof(T) * m_N);
			}
		}

		// Create the file and pre-set its contents to 0
		if (m_M > m_Offset_OOC)
		{
			unsigned nCols = m_M - m_Offset_OOC;
			uint64_t dwSize = nCols * m_N * sizeof(T);
			m_File.Init(dwSize);
		}		
	}

	

	template <typename T>
	LD_Matrix<T>::~LD_Matrix()
	{
		CleanRamStorage();
	}


	template <typename T>
	void LD_Matrix<T>::SetRamQuota(unsigned maxRamAlloc)
	{
		m_MaxRamAlloc = maxRamAlloc;
	}


	template <typename T>
	void LD_Matrix<T>::Initialize(unsigned N, unsigned M)
	{
		// Clean RAM Storage
		CleanRamStorage();

		m_N = N;
		m_M = M;

		// Calculate the offset index in out of core storage
		uint64_t tempOffset = (uint64_t)m_MaxRamAlloc * 1024 * 1024 / (m_N * sizeof(T));
		m_Offset_OOC = (unsigned)(tempOffset);

		// Allocate Memory for Inner core storage and preset its contents to 0
		unsigned nRamCols = min(m_M, m_Offset_OOC);
		if (nRamCols != 0)
		{
			m_A = new T * [nRamCols];
			for (unsigned j = 0; j < nRamCols; j++)
			{
				m_A[j] = new T[m_N];
				ZeroMemory(m_A[j], sizeof(T) * m_N);
			}
		}
		

		// Create the file and pre-set its contents to 0
		if (m_M > m_Offset_OOC)
		{
			unsigned nCols = m_M - m_Offset_OOC;
			uint64_t dwSize = nCols * m_N * sizeof(T);
			m_File.Init(dwSize);
		}
	}


	template <typename T>
	void LD_Matrix<T>::ZeroMatrix()
	{
		T* Zeros = new T[m_N];
		ZeroMemory(Zeros, sizeof(T) * m_N);

		// Zero all the columns
        #pragma omp parallel for
		for (int j = 0; j < m_M; j++)
			SetColumn(j, Zeros);
		
		delete[] Zeros;
	}


	template <typename T>
	void LD_Matrix<T>::CleanRamStorage()
	{
		unsigned nRamCols = min(m_M, m_Offset_OOC);
		if (m_A != nullptr)
		{
			for (unsigned j = 0; j < nRamCols; j++)
				if (m_A[j] != nullptr)
				{
					delete[] m_A[j];
					m_A[j] = nullptr;
				}
			
			delete[] m_A;
			m_A = nullptr;
		}
		
	}


	template <typename T>
	LD_Column<T> LD_Matrix<T>::operator[] (unsigned j)const
	{
		bool bOOC = false;
		T* Aj = nullptr;

		if (j < m_Offset_OOC)
		{
			bOOC = false;
			Aj = m_A[j];
		}
		else
		{
			bOOC = true;

			// Extract data from file
			Aj = new T[m_N];

			unsigned dj = j - m_Offset_OOC;
			DWORD dwColumnSize = static_cast<DWORD>(m_N * sizeof(T));
			uint64_t offset = static_cast<uint64_t>(dwColumnSize * dj);

			m_File.ReadAt(offset, Aj, dwColumnSize);
		}

		return LD_Column<T>(bOOC, m_N, Aj);
	}


	

	template <typename T>
	const T LD_Matrix<T>::Get(unsigned j, unsigned i) const
	{
		T Val = 0.0;

		if (j < m_Offset_OOC)
			Val = m_A[j][i];
		else
		{
			unsigned dj = j - m_Offset_OOC;
			DWORD dwColumnSize = static_cast<DWORD>(m_N * sizeof(T));
			DWORD dwRowOffset = static_cast<DWORD>(i * sizeof(T));
			uint64_t offset = static_cast<uint64_t>(dwColumnSize * dj + dwRowOffset);

			m_File.ReadAt(offset, &Val, sizeof(T));
		}

		return Val;
	}

	template <typename T>
	void LD_Matrix<T>::Set(unsigned j, unsigned i, T Val)
	{
		if (j < m_Offset_OOC)
			m_A[j][i] = Val;
		else
		{
			unsigned dj = j - m_Offset_OOC;
			DWORD dwColumnSize = static_cast<DWORD>(m_N * sizeof(T));
			DWORD dwRowOffset = static_cast<DWORD>(i * sizeof(T));
			uint64_t offset = static_cast<uint64_t>(dwColumnSize * dj + dwRowOffset);

			m_File.WriteAt(offset, &Val, sizeof(T));
		}
	}

	template <typename T>
	void LD_Matrix<T>::Add(unsigned j, unsigned i, T Val)
	{
		if (j < m_Offset_OOC)
			m_A[j][i] += Val;
		else
		{
			unsigned dj = j - m_Offset_OOC;
			DWORD dwColumnSize = static_cast<DWORD>(m_N * sizeof(T));
			DWORD dwRowOffset = static_cast<DWORD>(i * sizeof(T));
			uint64_t offset = static_cast<uint64_t>(dwColumnSize * dj + dwRowOffset);

			T NewVal = 0;
			m_File.ReadAt(offset, &NewVal, sizeof(T));

			NewVal += Val;

			m_File.WriteAt(offset, &NewVal, sizeof(T));
		}
	}

	template <typename T>
	void LD_Matrix<T>::Subtract(unsigned j, unsigned i, T Val)
	{
		if (j < m_Offset_OOC)
			m_A[j][i] -= Val;
		else
		{
			unsigned dj = j - m_Offset_OOC;
			DWORD dwColumnSize = static_cast<DWORD>(m_N * sizeof(T));
			DWORD dwRowOffset = static_cast<DWORD>(i * sizeof(T));
			uint64_t offset = static_cast<uint64_t>(dwColumnSize * dj + dwRowOffset);

			T NewVal = 0;
			m_File.ReadAt(offset, &NewVal, sizeof(T));

			NewVal -= Val;

			m_File.WriteAt(offset, &NewVal, sizeof(T));
		}
	}

	template <typename T>
	void LD_Matrix<T>::SetColumn(unsigned j, const LD_Column<T>& Aj)
	{
		DWORD dwColumnSize = static_cast<DWORD>(m_N * sizeof(T));

		if (j < m_Offset_OOC)
			CopyMemory(m_A[j], Aj.data(), dwColumnSize);
		else
		{
			unsigned dj = j - m_Offset_OOC;
			uint64_t offset = static_cast<uint64_t>(dwColumnSize * dj);

			m_File.WriteAt(offset, Aj.data(), dwColumnSize);
		}
	}


	template <typename T>
	void LD_Matrix<T>::SetColumn(unsigned j, T* Aj)
	{
		DWORD dwColumnSize = static_cast<DWORD>(m_N * sizeof(T));

		if (j < m_Offset_OOC)
			CopyMemory(m_A[j], Aj, dwColumnSize);
		else
		{
			unsigned dj = j - m_Offset_OOC;
			uint64_t offset = static_cast<uint64_t>(dwColumnSize * dj);

			m_File.WriteAt(offset, Aj, dwColumnSize);
		}
	}

	template <typename T>
	void LD_Matrix<T>::IncrementColumn(unsigned j, const LD_Column<T>& Aj)
	{
		DWORD dwColumnSize = static_cast<DWORD>(m_N * sizeof(T));

		if (j < m_Offset_OOC)
		{
            #pragma omp parallel for
			for (int i = 0; i < m_N; i++)
				m_A[j][i] += Aj[i];			
		}			
		else
		{
			unsigned dj = j - m_Offset_OOC;
			uint64_t offset = static_cast<uint64_t>(dwColumnSize * dj);

			// Extract the Column
			T* Col = new T[m_N];
			m_File.ReadAt(offset, Col, dwColumnSize);

            #pragma omp parallel for
			for (int i = 0; i < m_N; i++)
				Col[i] += Aj[i];

			m_File.WriteAt(offset, Col, dwColumnSize);

			delete[] Col;
		}
	}


	template <typename T>
	void LD_Matrix<T>::IncrementColumn(unsigned j, T* Aj)
	{
		DWORD dwColumnSize = static_cast<DWORD>(m_N * sizeof(T));

		if (j < m_Offset_OOC)
		{
            #pragma omp parallel for
			for (int i = 0; i < m_N; i++)
				m_A[j][i] += Aj[i];
		}
		else
		{
			unsigned dj = j - m_Offset_OOC;
			uint64_t offset = static_cast<uint64_t>(dwColumnSize * dj);

			// Extract the Column
			T* Col = new T[m_N];
			m_File.ReadAt(offset, Col, dwColumnSize);

            #pragma omp parallel for
			for (int i = 0; i < m_N; i++)
				Col[i] += Aj[i];

			m_File.WriteAt(offset, Col, dwColumnSize);

			delete[] Col;
		}
	}


	template <typename T>
	void LD_Matrix<T>::SetChunck(unsigned j_beg, unsigned n, T** Chunck)
	{
		unsigned j_end = j_beg + n - 1;
		DWORD nColSize = m_N * sizeof(T);

		if (j_end < m_Offset_OOC) // case 1
		{
            #pragma omp parallel for
			for (int j = j_beg; j <= j_end; j++)
			{
				unsigned j_ = j - j_beg;

				CopyMemory(m_A[j], Chunck[j_], nColSize);
			}
		}
		else if ((j_beg < m_Offset_OOC) && (j_end >= m_Offset_OOC)) // case2
		{
			// Copy the RAM part
            #pragma omp parallel for
			for (int j = j_beg; j < m_Offset_OOC; j++)
			{
				unsigned j_ = j - j_beg;

				CopyMemory(m_A[j], Chunck[j_], nColSize);
			}

			// Copy the OOC part
			{
				DWORD nBytes = (j_end - m_Offset_OOC + 1) * nColSize;
				BYTE* tempBuffer = new BYTE[nBytes];

                #pragma omp parallel for
				for (int j = m_Offset_OOC; j <= j_end; ++j)
				{
					unsigned j_ = j - j_beg;

					unsigned ByteOffset = (j - m_Offset_OOC) * nColSize;

					CopyMemory(tempBuffer + ByteOffset, Chunck[j_], nColSize);
				}
				// The offset in File is 0
				uint64_t offset = 0ULL;


				// Wirte the entire chunck to the file
				m_File.WriteAt(offset, tempBuffer, nBytes);

				// Delete the temporary buffer
				delete[] tempBuffer;
			}			
			
		}
		else if (j_beg >= m_Offset_OOC)
		{
			// Copy the OOC part
			DWORD nBytes = n * nColSize;
			BYTE* tempBuffer = new BYTE[nBytes];

            #pragma omp parallel for
			for (int j = 0; j < n; ++j)
			{
				unsigned ByteOffset = j * nColSize;

				CopyMemory(tempBuffer + ByteOffset, Chunck[j], nColSize);
			}
			// calculate the offset in File
			unsigned dj = j_beg - m_Offset_OOC;
			uint64_t offset = static_cast<uint64_t>((dj * m_N) * sizeof(T));


			// Wirte the entire chunck to the file
			m_File.WriteAt(offset, tempBuffer, nBytes);

			// Delete the temporary buffer
			delete[] tempBuffer;
		}
	}



	template<typename T>
	void LD_Matrix<T>::cmpbl(unsigned b1, unsigned n1, unsigned b2, unsigned n2, T* data)
	{
		// We have 3 cases for the block location
		// Case 1: the block is situated exclusively in RAM Storage
		// Case 2: the block is split between RAM and OOC storages
		// case 3: the block is situated exclusively in OOC storage

		unsigned b2_beg = b2;          // is the start column index of the block
		unsigned b2_end = b2 + n2 - 1; // is the end column index of the block 
		DWORD dwColSize = n1 * sizeof(T);


		if (b2_end < m_Offset_OOC) // case 1
		{
			// AHMED expects block data columnwise
			unsigned idx = 0;
			for (unsigned j = b2_beg; j <= b2_end; ++j)
			{
				CopyMemory(data + idx, m_A[j] + b1, dwColSize);
				idx += n1;
			}
		}
		else if ((b2_beg < m_Offset_OOC) && (b2_end >= m_Offset_OOC)) // case2
		{
			unsigned idx = 0;
			// Read the RAM part
			for (unsigned j = b2_beg; j < m_Offset_OOC; ++j)
			{
				CopyMemory(data + idx, m_A[j] + b1, dwColSize);
				idx += n1;
			}
			// Read the OOC part			
			for (unsigned j = m_Offset_OOC; j <= b2_end; ++j)
			{
				// calculate the offset in File
				unsigned dj = j - m_Offset_OOC;
				uint64_t offset = static_cast<uint64_t>((dj * m_N + b1) * sizeof(T));
				

				m_File.ReadAt(offset, data + idx, dwColSize);
				idx += n1;
			}
		}
		else if (b2_beg >= m_Offset_OOC)
		{
			// AHMED expects block data columnwise
			unsigned idx = 0;
			for (unsigned j = b2_beg; j <= b2_end; ++j) 
			{				
				// calculate the offset in File
				unsigned dj = j - m_Offset_OOC;
				uint64_t offset = static_cast<uint64_t>((dj * m_N + b1) * sizeof(T));

				m_File.ReadAt(offset, data + idx, dwColSize);
				idx += n1;
			}
				
		}		
	}

	template<typename T>
	T LD_Matrix<T>::scale(unsigned b1, unsigned n1, unsigned b2, unsigned n2)
	{
		//T max_val = static_cast<T>(0);

		//T* data = new T[n1 * n2];
		//cmpbl(b1, n1, b2, n2, data);

		//for (unsigned idx = 0; idx < n1 * n2; ++idx) 
		//{
		//	T v = std::abs(data[idx]);
		//	if (v > max_val) 
		//		max_val = v;
		//}

		//delete[] data;

		//// Avoid zero scale (ACA expects non-zero scale; returning 1 may be ok for tiny blocks but prefer small eps).
		//if (max_val == static_cast<T>(0))
		//	return static_cast<T>(1e-16);

		//return max_val;

		return (T)1.0;
	}



	template<typename T>
	void LD_Matrix<T>::OpenFile(bool bWriteAccess)
	{
		if (m_M <= m_Offset_OOC) // No need to open the file!!
			return;

		m_File.Open(bWriteAccess, false);

	}

	template<typename T>
	void LD_Matrix<T>::CloseFile()
	{
		if (m_M <= m_Offset_OOC) // No need to close the file!!
			return;

		m_File.Close();
	}

	template<typename T>
	void LD_Matrix<T>::Print(LPCTSTR szFileName)const
	{
		// Open file for write in text mode
		std::fstream file(szFileName, std::ios::out);

		// Reference the this pointer
		const LD_Matrix& Mat = *this;

		for (unsigned j = 0; j < m_M; j++)
		{
			const LD_Column<T> Col = (*this)[j];

			for (unsigned i = 0; i < m_N; i++)
			{
				file << Col[i] << "\t";
			}
			file << "\n";
		}

		// Close file
		file.close();
	}

};

//===============================================================================