#pragma once

#include "configure.h"
#include "struct.h"


namespace matchn {


class World;


/**
* ������� ����, � ������� ����� �����������������.
* GE - ����. �� GameElement.
*/
class GE {
public:
    explicit GE( std::shared_ptr< World > );




    virtual ~GE();




    /**
    * @return UID ����� ��������.
    */
    inline uidElement_t uid() const {
        return mUID;
    }



    /*
    * #! ������� ���� World ���������� nullptr.
    */
    std::shared_ptr< World >  world() const;
    std::shared_ptr< World >  world();




    /**
    * @return ������� ����������.
    */
    inline bool live() const {
        return !mDeleted;
    }




    /**
    * �������� ������� �� ��������.
    */
    inline void unlive() {
        mDeleted = true;;
    }




    inline bool operator==( const GE& b ) const {
        return (mUID == b.uid());
    }




private:
    /**
    * @return ����. ��������� UID ��� ��������.
    */
    static inline uidElement_t nextUID() {
        ++nextFreeUID;
        return nextFreeUID;
    }




private:
    std::weak_ptr< World >  mWorld;

    /**
    * UID ��������.
    */
    const uidElement_t  mUID;

    /**
    * �������: ������� ���� ����� �� ����.
    */
    bool  mDeleted;


    /**
    * ������� ��� ������� UID ���������.
    */
    static uidElement_t  nextFreeUID;
};


} // matchn
