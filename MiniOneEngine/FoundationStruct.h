#pragma once
#include "Vector.h"

namespace mini
{
	//消息的响应事件
	struct event
	{
		BOOL   bProcess;   //消息是否已经处理过了
		INT64  bringtime;  //消息的产生时间
		POINT  point;     //鼠标位置
		UINT   Messtype;   //消息类型 
		WPARAM wParam;
		LPARAM lParam;
	};

	//响应的消息
	enum MSG_TYPE
	{
		LEFTDOWN = 1 << 1,					//WM_LBUTTONDOWN
		LEFTUP = 1 << 2,					//WM_LBUTTONUP
		RIGHTDOWN = 1 << 3,					//WM_RIGHTBUTTONDOWN
		RIGHTUP = 1 << 4,					//WM_RIGHTBUTTONUP
		MOUSEMOVE = 1 << 5,					//WM_MOUSEMOVE
		MOUSEOUT = 1 << 6,					//自定义消息,表示移出控件范围发生什么事情
		MOUSEWHEEL = 1 << 7,					//WM_MOUSEWHEEL
		LEFTDBLCLK = 1 << 8,					//WM_LBUTTONDBLCLK
		RIGHTDBLCLK = 1 << 9,					//WM_RBUTTONDBLCLK
		WMCHAR = 1 << 10,                  //WM_CHAR
		WMKEYDOWN = 1 << 11,
		MAXMESSAGE = 1 << 12,
		MBUTTONDOWN = 1 << 13,
		MBUTTONUP = 1 << 14,
		GETFOCUS = 1 << 15,
		LOSEFOCUS = 1 << 16,
		WMKEYUP = 1 << 17,

		AllMsg = LEFTDOWN | LEFTUP | RIGHTDOWN | RIGHTUP |
		MOUSEMOVE | MOUSEOUT | MOUSEWHEEL | LEFTDBLCLK |
		RIGHTDBLCLK | WMCHAR | WMKEYDOWN | WMKEYUP | MBUTTONDOWN | MBUTTONUP
		| GETFOCUS | LOSEFOCUS,
	};


	//////////////////////////////////////////////////////////////////////////
	template< typename T >
	struct T_Point2
	{
		T_Point2(std::initializer_list<T> vec)
		{
			INT32 i = 0;
			for (auto item : vec)
			{
				m[i++] = item;
				if (i >= 2)
				{
					break;
				}
			}
		}
		T_Point2()
		{
			x = y = 0;
		}
		T_Point2(T _x, T _y)
		{
			x = _x;
			y = _y;
		}
		T_Point2(T* value)
		{
			m[0] = value[0];
			m[1] = value[1];
		}

		T_Point2& operator + (const T_Point2 &vec) const
		{
			return T_Point2(x + vec.x, y + vec.y);
		}

		T_Point2& operator - (const T_Point2 &vec) const
		{
			return T_Point2(x - vec.x, y - vec.y);
		}

		T_Point2 operator * (float f) const
		{
			return T_Point2(x * f, y * f);
		}

		T_Point2 operator / (float f) const
		{
			float fInv = 1.0f / f;
		    return T_Point2(x * fInv, y * fInv);
		}

		friend T_Point2 operator * (float f, const T_Point2& vec)
		{
			return T_Point2(f * vec.x, f * vec.y);
		}

		union
		{
			struct
			{
				T x, y;
			};
			T m[2];
		};

	};
	typedef T_Point2<float> CPoint2F;
	typedef T_Point2<INT32> CPoint2I;


	//////////////////////////////////////////////////////////////////////////
	template< typename T >
	struct T_Point3
	{
		T_Point3(std::initializer_list<T> vec)
		{
			INT32 i = 0;
			for (auto item : vec)
			{
				m[i++] = item;
				if (i >= 3)
				{
					break;
				}
			}
		}
		T_Point3()
		{
			x = y = z = 0;
		}
		T_Point3(T _x, T _y, T _z)
		{
			x = _x;
			y = _y;
			z = _z;
		}
		T_Point3(T* value)
		{
			m[0] = value[0];
			m[1] = value[1];
			m[2] = value[2];
		}

		T_Point3& operator + (const T_Point3 &vec) const
		{
			return T_Point3(x + vec.x, y + vec.y, z + vec.z);
		}

		T_Point3& operator - (const T_Point3 &vec) const
		{
			return T_Point3(x - vec.x, y - vec.y, z - vec.z);
		}


		T_Point3 operator * (float f) const
		{
			return T_Point3(x * f, y * f, z * f);
		}

		T_Point3 operator / (float f) const
		{
			float fInv = 1.0f / f;
			return T_Point3(x * fInv, y * fInv, z & fInv);
		}

		friend T_Point3 operator * (float f, const T_Point3& vec)
		{
			return T_Point3(f * vec.x, f * vec.y, f* vec.z);
		}

		union
		{
			struct
			{
				T x, y, z;
			};
			T m[3];
		};
	};
	typedef T_Point3<float> CPoint3F;
	typedef T_Point3<INT32> CPoint3I;

	template< typename T >
	struct T_Point4
	{
		T_Point4(std::initializer_list<T> vec)
		{
			INT32 i = 0;
			for ( auto item : vec )
			{
				m[i++] = item;
				if (i >= 4)
				{
					break;
				}
			}
		}

		T_Point4()
		{
			x = y = z = w = 0;
		}
		T_Point4(T _x, T _y, T _z, T _w)
		{
			x = _x;
			y = _y;
			z = _z;
			w = _w;
		}
		T_Point4(T* value)
		{
			m[0] = value[0];
			m[1] = value[1];
			m[2] = value[2];
			m[3] = value[3];
		}

		T_Point4& operator + (const T_Point4 &vec) const
		{
			return T_Point4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
		}

		T_Point4& operator - (const T_Point4 &vec) const
		{
			return T_Point4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
		}

		T_Point4 operator * (float f) const
		{
			return T_Point3(x * f, y * f, z * f, w * f);
		}

		T_Point4 operator / (float f) const
		{
			float fInv = 1.0f / f;
			return T_Point4(x * fInv, y * fInv, z * fInv, w * fInv);
		}

		friend T_Point4 operator * (float f, const T_Point4& vec)
		{
			return T_Point4(f * vec.x, f * vec.y, f * vec.z, f*vec.w);
		}
		union
		{
			struct
			{
				T x, y, z, w;
			};
			T m[4];
		};
	};
	typedef T_Point4<float> CPoint4F;
	typedef T_Point4<INT32> CPoint4I;


#pragma warning ( push, 3 )  // Temporarily setting warning level 3
	struct CRectF
	{
		union
		{
			struct
			{
				float left;
				float right;
				float top;
				float bottom;
			};
			struct
			{
				float x;
				float y;
				float z;
				float w;
			};
		};
	};
#pragma warning (pop) 


	struct VertexFormatVTC
	{
#ifdef WIN32
		CPoint3F position;
		CPoint2F uv;
		CPoint4F color;
#else
		union
		{
			struct
			{
				float v[3];
			};
			float x, y, z;
		};
		union
		{
			struct
			{
				float m[4];
			};
			float x, y, z, w;
		};
#endif
	};

	struct VertexFormatVC
	{
#ifdef WIN32
		CPoint3F position;
		CPoint4F color;
#else
		union
		{
			struct
			{
				float v[3];
			};
			float x, y, z;
		};
		union
		{
			struct
			{
				float m[4];
			};
			float x, y, z, w;
		};
#endif
	};


	struct VertexFormatVTN
	{
		CPoint3F	position;
		CPoint2F	uv;
		CPoint3F	normal;
	};

	struct VertexFormatVTNSkin : public VertexFormatVTN
	{
		CPoint4F	BoneIndex;
		CPoint3F	skinWeight;
	};

	struct VertexFormatVTNTB : public VertexFormatVTN
	{
		CPoint3F	tangent;
		CPoint3F	binormal;
	};

	struct VertexFormatVTNTBSkin : public VertexFormatVTN
	{
		CPoint4I	BoneIndex;
		CPoint4F	skinWeight;
	};

	struct INTERSECTION
	{
		DWORD dwFace;                 // mesh face that was intersected
		FLOAT fBary1, fBary2;         // barycentric coords of intersection
		FLOAT fDist;                  // distance from ray origin to intersection
		FLOAT tu, tv;                 // texture coords of intersection
	};
}
