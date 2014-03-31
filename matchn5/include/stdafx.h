#pragma once

// ����� ����������� ����� ������� � ����������� ��������� ������������ ��������, ��� �������
#pragma warning( disable : 4503 )

// ���������� ��������� �������, ������� ���� �������� ��� ���������� � deprecated
// @see http://lists.boost.org/Archives/boost/2006/08/109338.php 
//#pragma warning( disable : 4996 )


// (!) ��������� �������������� ������� �������� ������ ���� ���. �����
//     ���������� � ��� ��� �������������� �����������.

// ����������� ������� �������������� "pcl::search::Search<PointT>::radiusSearch",
// ���������� ������ ����������� �� ��������� ���������������, ����� ���������
// ���������� ������ ��������������� const ��� volatile
//#pragma warning( disable : 4373 )

// �������������� double � int, �������� ������ ������
//#pragma warning( disable : 4244 )


// �������� ����������, �������� ������� ���� �� ��������� Windows
#define WIN32_LEAN_AND_MEAN


// ���������� ��������� ������� �� ������� ������ Windows
// (��������� ��� boost::asio 1.53)
#define _WIN32_WINNT 0x501



// �������� ������� ������ � WinDef.h
#define NOMINMAX

// ���������� �������������� ���������
#define _USE_MATH_DEFINES



// ��� ������ ��������� �������� (Boost MSM)
// @see http://boost.org/doc/libs/1_53_0/libs/msm/doc/HTML/ch05.html
#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_VECTOR_SIZE 50
//#define BOOST_MPL_LIMIT_MAP_SIZE 50
#define FUSION_MAX_VECTOR_SIZE 50


#include <assert.h>
#include <algorithm>
#include <climits>
#include <cstdio>
#include <ctime>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <istream>
#include <list>
#include <map>
#include <math.h>
#include <memory>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include <windows.h>

#include <boost/assign.hpp>
#include <boost/bimap.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/function.hpp>
#include <boost/functional/hash.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/math/special_functions/round.hpp>
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/numeric/interval.hpp>
#include <boost/thread.hpp>
#include <boost/unordered_map.hpp>


#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/sound.h>

// Choose the target renderer
//#define USE_OPENGL_2
//#define USE_OPENGL_1
#define USE_SOFTWARE_RENDERER

#ifdef USE_SOFTWARE_RENDERER
#include <ClanLib/swrender.h>
#endif

#ifdef USE_OPENGL_1
#include <ClanLib/gl1.h>
#endif

#ifdef USE_OPENGL_2
#include <ClanLib/gl.h>
#endif


#include <typelib/typelib.h>


#include "Console.h"




namespace msm  = boost::msm;
namespace msmf = boost::msm::front;
namespace mpl  = boost::mpl;
