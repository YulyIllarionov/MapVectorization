#include <stdafx.h>
#include <atlbase.h>
#include <vector>

// ----------------------------------------------------
// 1. DEFINE: XXX_YYY
// ----------------------------------------------------
#ifndef RIGHT_MILE_LENGTH
#define RIGHT_MILE_LENGTH  1851.8518519
#endif

// ----------------------------------------------------
// 2. NAMESPACE: XxxYyy
// ----------------------------------------------------
namespace NamedScope
{

	// ----------------------------------------------------
	// 3. ENUM: XXX_YYY
	// ----------------------------------------------------
	enum VOLUME_TYPE
	{
		VT_NORMAL,
		VT_BIG,
		VT_ENORMOUS
	};

}; 

// ----------------------------------------------------
// 4. Inline �������:
//       4.1. ����������: xxx_yyy() 
//       4.2. ��������� �������:    xxxYyy
//       4.3. ��������� ����������: xxxYyy
// ----------------------------------------------------
inline int negate_value(int x)
{
	return -x;
}

double calculate_scale(double lonEnd, double lonBegin, double pixClientWidth, double pixSize) 
{
	double k = 60.0 * RIGHT_MILE_LENGTH;
	double chartScale = k * (lonEnd - lonBegin) / (pixClientWidth * pixSize); 
	return chartScale;
}

// ------------------------------------------------------------
// 5. Raw ���������, ����� ������������ �� �������� � ���� � �������: 
//       5.1. ����������:        xxx_yyy
//       5.2. ����:              xxx_yyy
//       5.3. �������:           kxx_yyy()
// ------------------------------------------------------------
struct bounding_rect
{
	LONG x_left;
	LONG y_top;
	LONG x_right;
	LONG y_bottom;

	LONG get_width() const 
	{ 
		return (x_right - x_left); 
	}

	LONG get_height() const 
	{ 
		return (y_bottom - y_top); 
	}
};
// ���� ��������� �� ����� ������������ �� �������� - ��������� ��� �����

// ------------------------------------------------------------
// 6. ���������� ���������� ����� ������������ ������: IXxxYyy 
// ------------------------------------------------------------
class ISafeVector
{
public:
	virtual size_t GetCount() const = 0; 
	virtual BOOL GetElement(size_t index, DWORD& elem) const = 0;
};

// ------------------------------------------------------------
// 7. �����: 
//       7.1. ����������:        XxxYyy (���� ���������� ���������� XxxYyyImpl)
//       7.2. ����:              _xxxYyy (��������� m_xxxYyy, �� ���� ����� ����, ���� ������)
//       7.3. �������� �������:  XxxYyy()
//       7.4. �������� �������:  xxxYyy()
// ------------------------------------------------------------
class SafeVectorImpl : public ISafeVector
{ 
public: 
	SafeVector(void)  {} 
	~SafeVector(void) {} 

	// ���� ������� - ������ �� ����� �������
	size_t GetCount() const 
	{ 
		return privateGetCount(); 
	}

	BOOL GetElement(size_t index, DWORD& elem) const 
	{ 
		return getElement(index, elem); 
	}

private:
	std::vector<DWORD> _safeVector;

	size_t privateGetCount() const 
	{ 
		return _safeVector.size(); 
	}

	// ��� �������������� ������ - ��������� �� ���������� �����
	BOOL getElement(size_t index, DWORD& elem) const 
	{ 
		return ( (index < _safeVector.size()) ?  (elem = _safeVector[index], TRUE) : FALSE ); 
	}	
}; 

// ------------------------------------------------------------
// 9. ���������� ���������� ����������: g_XxxYyy
// ------------------------------------------------------------
class CAtlAppModule : public CAtlDllModuleT<CAtlAppModule> {};
CAtlAppModule g_AtlAppModule;

// ------------------------------------------------------------
// 10. ������ ������������� ������
// ------------------------------------------------------------
CoProject::CoProject()
	: _notificationState(false)
	, _pRbCallback(0)
	, _bEmpty(true)
{
	InitHeaderCurrent();
	_rbEnabled = false;
}

// ------------------------------------------------------------
// 10. ���������� if, for � �.�.
// ------------------------------------------------------------
int main() 
{
	int i;
	// ������ ������
	for (i = 0; i < 2; i++) 
		printf_s("\n%d\n", i);	// ������� �������� �� ��������� �������

	// ������� ��������:
	for (i = 0; i < 2; i++) 
	{
		printf_s("\n%d\n", i + 3);
		printf_s("\n%d\n", i);
	}
}
