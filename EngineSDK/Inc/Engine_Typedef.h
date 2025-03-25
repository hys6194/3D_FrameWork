#ifndef Engine_Typedef_h__
#define Engine_Typedef_h__

namespace Engine
{
	typedef		bool						_bool;

	typedef		signed char					_byte;
	typedef		unsigned char				_ubyte;
	typedef		char						_char;

	typedef		wchar_t						_tchar;
	
	typedef		signed short				_short;
	typedef		unsigned short				_ushort;

	typedef		signed int					_int;
	typedef		unsigned int				_uint;

	typedef		signed long					_long;
	typedef		unsigned long				_ulong;

	typedef		float						_float;
	typedef		double						_double;

	typedef		wstring						_wstring;
	typedef		string						_string;

	typedef		XMFLOAT2					_float2;	// 저장용
	typedef		XMFLOAT3					_float3;	// 저장용
	typedef		XMFLOAT4					_float4;	// 저장용
	typedef		XMVECTOR					_vector;	// 연산용
	typedef		FXMVECTOR					_fvector;
	typedef		GXMVECTOR					_gvector;
	typedef		HXMVECTOR					_hvector;
	typedef		CXMVECTOR					_cvector;

	typedef		XMFLOAT4X4					_float4x4;	// 저장용
	typedef		XMMATRIX					_matrix;	// 연산용
	typedef		FXMMATRIX					_fmatrix;	// 인라인 함수에서 매개변수로 전달할 때 최적회 되어 있음
	typedef		CXMMATRIX					_cmatrix;	// 함수 인자로 사용할 때 유용



}

#endif // Engine_Typedef_h__
