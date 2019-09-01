#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <tchar.h>
#include "DevicePlatform.h"

#ifndef V_RETURN
#define V_RETURN(x)					{ hr = x; if( FAILED(hr) ) { return hr; } }
#endif
namespace mini
{
	namespace foundation
	{
		inline std::list<std::string> split(std::string str, std::string pattern)
		{
			std::string::size_type pos;
			std::list<std::string> result;
			str += pattern;//扩展字符串以方便操作
			int size = str.size();

			for (int i = 0; i < size; i++)
			{
				pos = str.find(pattern, i);
				if (pos != std::string::npos)
				{
					std::string s = str.substr(i, pos - i);
					result.push_back(s);
					i = pos + pattern.size() - 1;
				}
			}
			return result;
		}

		//随机返回一个浮点数
		inline float GetRandomFloat(float lowBound, float highBound)
		{
			if (lowBound >= highBound) // bad input
				return lowBound;

			// get random float in [0, 1] interval
			float f = (rand() % 10000) * 0.0001f;

			// return float in [lowBound, highBound] interval. 
			return (f * (highBound - lowBound)) + lowBound;
		}

		//随机的返回一个向量
		inline void GetRandomVector(CPoint3F* out, CPoint3F* min, CPoint3F* max)
		{
			out->x = GetRandomFloat(min->x, max->x);
			out->y = GetRandomFloat(min->y, max->y);
			out->z = GetRandomFloat(min->z, max->z);
		}

		inline DWORD FtoDw(float f)
		{
			return *((DWORD*)&f);
		}

		inline bool strtowstr(IN const std::string& str, OUT std::wstring& wstr)
		{
			wstr.clear();
			wstr.resize(str.length());
			WCHAR* pBuf = const_cast<WCHAR*>(wstr.c_str());
			INT32 nwLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), (INT32)str.length(), NULL, 0);
			MultiByteToWideChar(CP_ACP, 0, str.c_str(), (INT32)str.length(), pBuf, nwLen);
			return true;
		}

		inline bool wstrtostr(IN const std::wstring& wstr, OUT std::string& str)
		{
			int iSize;
			char pszMultiByte[512];
			iSize = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
			WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, pszMultiByte, iSize, NULL, NULL);
			str = pszMultiByte;
			return true;
		}

		inline void ComputeTriNormal(CPoint3F& v1, CPoint3F& v2, CPoint3F& v3, CPoint3F& normalVec)
		{
			D3DXVECTOR3 tmpV1;
			tmpV1.x = v1.x - v2.x;
			tmpV1.y = v1.y - v2.y;
			tmpV1.z = v1.z - v2.z;
			D3DXVECTOR3 tmpV2;
			tmpV2.x = v1.x - v3.x;
			tmpV2.y = v1.y - v3.y;
			tmpV2.z = v1.z - v3.z;
			D3DXVECTOR3 normal;
			D3DXVec3Cross(&normal, &tmpV1, &tmpV2);
			D3DXVec3Normalize(&normal, &normal);
			normalVec = { normal.x, normal.y, normal.z };
		}

		template <typename T>
		inline void Clamp(OUT T& out, IN T p1, IN T p2)
		{
			if (out < p1)
			{
				out = p1;
			}
			else if (out > p2)
			{
				out = p2;
			}
		}


		template<class T >
		T Clip(T minvalue, T maxvalue, T in)
		{
			if (in >= minvalue && in <= maxvalue)
			{
				return in;
			}
			else if (in < minvalue)
			{
				return minvalue;
			}
			return maxvalue;
		}

		inline FLOAT angle_to_radian(FLOAT value)
		{
			if (value > 360.0f)
			{
				value -= 360.0f;
			}
			else if (value < 0.0f)
			{
				value += 360.0f;
			}
			return value * (float)D3DX_PI / 180.0f;
		}

		inline FLOAT radian_to_angle(FLOAT value)
		{
			return value * 180.0f / (float)D3DX_PI;
		}

		inline void getPath(TCHAR *path, TCHAR *szbuffer)
		{
			for (size_t i = _tcslen(szbuffer); i > 0; --i)
			{
				if ('/' == szbuffer[i])
				{
					_tcsncpy_s(path, i + 1, szbuffer, i + 1);
					path[i + 1] = '\0';
					break;
				}
			}
		}

		//距离公式
		inline FLOAT getLength(CPoint3F &v0, CPoint3F &v1)
		{
			return (v0.x - v1.x) * (v0.x - v1.x) +
				(v0.y - v1.y) * (v0.y - v1.y) +
				(v0.z - v1.z) * (v0.z - v1.z);
		}

		//距离公式开方
		inline FLOAT getLengthsqr(CPoint3F &v0, CPoint3F &v1)
		{
			return sqrt(getLength(v0, v1));
		}

		inline bool InPlace(RECT const& _rect, POINT const& _pt)
		{
			return (
				_pt.x >= _rect.left &&
				_pt.x < _rect.left + _rect.right &&			//right == 宽度
				_pt.y >= _rect.top	&&
				_pt.y < _rect.top + _rect.bottom			//bottom == 高度
				);
		}

		template<typename T>
		class IsClassT
		{
		private:
			typedef char One;
			typedef struct
			{
				char a[2];
			} Two;
			template<typename C>
			static One test(int T::*);
			template<typename C>
			static Two test(...);
		public:
			enum { Yes = sizeof(test<T>(0)) == 1 };
			enum { No = !Yes };
		};

		template< typename T >
		bool CheckClass()
		{
			if (IsClassT<T>::Yes)
			{
				return true;
			}
			return false;
		};

		template<typename T>
		T stringToType(const std::string& strConent)
		{
			std::istringstream iss(strConent);
			T num;
			iss >> num;
			return num;
		}

		template<typename T>
		std::vector<T> splitString(std::string str, const std::string& pattern) //noexcept
		{
			std::vector<T> result;
			result.reserve(4);

			std::string::size_type pos = 0;
			str += pattern;
			std::string::size_type size = str.size();
			for (std::string::size_type i = 0; i < size; ++i)
			{
				pos = str.find(pattern, i);
				if (pos < size)
				{
					T tempValue = stringToType<T>(str.substr(i, pos - i));
					result.push_back(tempValue);
					i = pos + pattern.size() - 1;
				}
			}
			return result;
		}


		extern const std::string getExterName(const std::string& strName);
	}
}