#pragma once

#include <string>
#include <typelib/include/coord2.h>
#include <typelib/include/vector2.h>


namespace matchn {


/**
* ���������� ���������� ���������, ����������� � ����.
*
* @todo extend ���������� �� ���������� ����� � ����� 'media/element'.
*/




/**
* �������������� �������� ����������� � ����������� �� ���������.
*/
static const int VISUAL_STEP_CHANGE  = 10;
static const int VISUAL_STEP_DROP    = 10;
static const int VISUAL_TIMEOUT_SAME = 2000;





/**
* ������ ���� ����, �������� �������� ������������� ��������, ���.
*/
static const int WINDOW_WIDTH  = 800;
static const int WINDOW_HEIGHT = 800;
static const float WINDOW_K_WIDTH_HEIGHT =
    static_cast< float >( WINDOW_WIDTH ) /
    static_cast< float >( WINDOW_HEIGHT );




/**
* �������� ������ ��������� ����������� ���� ������������ �����������, ���.
*/
static const int CENTER_WINDOW_WIDTH  = WINDOW_WIDTH  / 2;
static const int CENTER_WINDOW_HEIGHT = WINDOW_HEIGHT / 2;




/**
* ������������ � ����������� ���������� ����������� ����, ���.
*/
static const int MIN_X_VISUAL_COORD = -WINDOW_WIDTH  / 2;
static const int MAX_X_VISUAL_COORD =  WINDOW_WIDTH  / 2;
static const int MIN_Y_VISUAL_COORD = -WINDOW_HEIGHT / 2;
static const int MAX_Y_VISUAL_COORD =  WINDOW_HEIGHT / 2;



/**
* ������������ ������� ���������� �� ������ ���� (�� ������ �� ����).
*/
static const float MAX_VISUAL_CENTER_DISTANCE =
    typelib::vector2_t( MAX_X_VISUAL_COORD, MAX_Y_VISUAL_COORD ).length();




// # ���� ��� ���������� � ������� �������� ������������ ����������� ���� ����.
// # ����������� ������������ ����
//     - ���������� ���: X - ������, Y - ����
//     - ���������� ���: X - ������, Y - �����




/**
* ���������� ������ ������� ���������, ���.
* �������� �������. ����������� ���� ���� - ��. ����. ����.
*/
static const int ELEMENT_AREA_X = 0;
static const int ELEMENT_AREA_Y = 0;




/**
* ������ ������� �������� (�� ���������).
*/
static const typelib::size2Int_t  ELEMENT_SPRITE_SIZE( 270, 270 );



/**
* ���� � ������ � �������� �������.
*/
static const std::string PATH_MEDIA = "media";




/**
* ��� �������.
*/
//#define _DEBUG_EVENT


#undef ASSERT
#define ASSERT(EXPR)  assert(EXPR);
#define DASSERT(EXPR)  if (!(EXPR)) __debugbreak();


// @see Console
#define CONSOLE  Console::instance()



} // matchn
