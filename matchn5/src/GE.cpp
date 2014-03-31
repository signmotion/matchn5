#include "../include/stdafx.h"
#include "../include/GE.h"
#include "../include/ge/world/World.h"


namespace matchn {


uidElement_t GE::nextFreeUID = 0;




GE::GE(
    std::shared_ptr< World >  w
) :
    mWorld( w ),
    mUID( nextUID() ),
    mDeleted( false )
{
    /* - ��� - ���� ������� ����. ��� ���� ��������� �������� nullptr.
    ASSERT( !mWorld.expired()
        && "������� ����������� ���." );
    */
    ASSERT( ( !w || ( mWorld.lock() && mWorld.lock()->visual() ) )
        && "���������� ��� ������ ���� ���������������." );
}




GE::~GE() {
}




std::shared_ptr< World >
GE::world() const {
    return mWorld.lock();
}




std::shared_ptr< World >
GE::world() {
    return mWorld.lock();
}


} // matchn
