#pragma once


// CGizmoWnd
#define ID_GIZMO_WND 23012026

class CGizmoWnd : public CWnd
{
	DECLARE_DYNAMIC(CGizmoWnd)

public:
	CGizmoWnd();
	virtual ~CGizmoWnd();

	BOOL CreateGizmoWnd(CWnd* pParent);
	void RenderFrame(float XRotAng, float YRotAng);
	void InitializeD3D();

	void SetWndSize(int Width, int Height) 
	{
		m_Width = Width;
		m_Height = Height;
	}


private:
	void CleanUpD3D();
	void SetGeometryPipeline(float XRotAng, float YRotAng);
	IDirect3DDevice9* m_pD3ddev;
	ID3DXMesh* m_pMeshXAxis;
	ID3DXMesh* m_pMeshYAxis;
	ID3DXMesh* m_pMeshZAxis;
	const float m_Z;
	int m_Width;
	int m_Height;

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


