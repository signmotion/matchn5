#pragma once

#include "../../Incarnate.h"


namespace matchn {


class World;


struct WorldFSM :
    public msm::front::state_machine_def< WorldFSM >
{
    WorldFSM( size_t K, size_t N, size_t M );

    virtual ~WorldFSM();


    /**
    * ������� ��� ������� � ��������� ������.
    */
    template < class E, class F >
    void on_entry( E const&, F& );

    template < class E, class F >
    void on_exit( E const&, F& );


    /**
    * ��������� ������.
    */
    struct IncarnateContainer;
    struct Wait;


    /**
    * ��������� ���������.
    */
    typedef Wait initial_state;


    /**
    * �������������� ������� (Event).
    */
    struct PulseE {};


    /**
    * ��������������� �������� (Action).
    */
    // .


    /**
    * �������� �������� (Guard).
    */
    bool presentFreeCellG( const PulseE& );


    /**
    * ������� ���������.
    */
    typedef WorldFSM  self;
    struct transition_table : mpl::vector<
        //      Current state        Event         Next state           Action                Guard
        //     +--------------------+-------------+--------------------+---------------------+---------------------------+
        g_row < Wait,                PulseE,       IncarnateContainer,                        &self::presentFreeCellG >,
        //     +--------------------+-------------+--------------------+---------------------+---------------------------+
         _row < IncarnateContainer,  msmf::none,   Wait >
        //     +--------------------+-------------+--------------------+---------------------+---------------------------+
    > {};


    /**
    * ����� �� ��������� ��� �������������� ��������.
    */
    template < class F, class E >
    void no_transition( E const& e,  F&,  int state );




    /**
    * @return ���������� �����. ���������� �� ����������.
    */
    typelib::coord2Int_t visualByLogicCoordContainer(
        const typelib::coord2Int_t&  lc
    ) const;




    /**
    * @return ���������� �����. ���������� �� ����������.
    */
    typelib::coord2Int_t logicByVisualCoordContainer(
        const typelib::coord2Int_t&  vc
    ) const;




    /**
    * @return ������� ���� T � �������� ������ ���� ��� nullptr,
    *         ���� ������ �������� ���.
    */
    template< class T >
    std::shared_ptr< Incarnate > element(
        const typelib::coord2Int_t&  logicCoord
    );



    /**
    * @return �������� ���� T �� ��������� S.
    */
    //template< class T, class S >
    //std::vector< std::shared_ptr< Incarnate > >  state() const;




    // @todo ������� � ��������� ����� typelib.
    // @see typelib::DynamicMapContent2D
    inline typelib::size2Int_t size() const {
        return typelib::size2Int_t( N, M );
    }




    inline std::size_t volume() const {
        return N * M;
    }




    inline typelib::coord2Int_t minCoord() const {
        return -maxCoord();
    }


    inline typelib::coord2Int_t maxCoord() const {
        return static_cast< typelib::coord2Int_t >( (size() - 1) / 2 );
    }




    inline bool inside( const typelib::coord2Int_t& c ) const {
        const auto mi = minCoord();
        const auto ma = maxCoord();
        return (c.x >= mi.x) && (c.x <= ma.x)
            && (c.y >= mi.y) && (c.y <= ma.y);
    }


    inline bool inside( int x, int y ) const {
        return inside( typelib::coord2Int_t( x, y ) );
    }




    inline size_t ic( const typelib::coord2Int_t& c ) const {
        return ic( c.x, c.y );
    }


    inline size_t ic( int x, int y ) const {
        const auto max = maxCoord();
        const size_t i = (
            static_cast< std::size_t >(x + max.x)
          + static_cast< std::size_t >(y + max.y) * N
        );
        return i;
    }




    inline typelib::coord2Int_t ci( size_t i ) const {
        const size_t y = i / N;
        const size_t x = i - y * N;
        const typelib::coord2Int_t  c(
            static_cast< int >( x ) - maxCoord().x,
            static_cast< int >( y ) - maxCoord().y
        );

        ASSERT( inside( c )
            && "����������� 2D ���������� ����� �� ��������� �������� �����." );

        return c;
    }




    static inline typelib::coord2Int_t isc( size_t cell ) {
        using namespace typelib;
        ASSERT( (cell < 9)
            && "������� �������� ������ �� ��������� 2D-������������." );
        const static coord2Int_t a[ 9 ] = {
            coord2Int_t(  0,   0 ),
            coord2Int_t(  0,  +1 ),
            coord2Int_t( +1,   0 ),
            coord2Int_t(  0,  -1 ),
            coord2Int_t( -1,   0 ),
            coord2Int_t( +1,  +1 ),
            coord2Int_t( +1,  -1 ),
            coord2Int_t( -1,  -1 ),
            coord2Int_t( -1,  +1 )
        };

        return a[ cell ];
    }




    /**
    * �������� � 'r' ��� ���������� �������� � ������ 'name'.
    *
    * # � ������ ���������� ������ ������ 'el'.
    */
    template< typename T >
    void harvestAllNeighbour(
        std::set< T* >*  r,
        const nameElement_t&  name,
        const T*  el
    );





public:
    std::weak_ptr< World >  mWorld;


    /**
    * ���-�� ���������� ������ ��������� � ������ ����.
    *
    * # ������ ��������� �����������, �� ������ gett'���.
    */
    const size_t  K;
    const size_t  N;
    const size_t  M;

    
    /**
    * ����������� � ���� ��������.
    * # ��� World (���� ������� �������) ��� ����� ������.
    * # �������� ����������� � ������� ���������.
    */
    typedef std::list< std::shared_ptr< Incarnate > >  incarnateSet_t;
    incarnateSet_t  mIncarnateSet;
};




struct WorldFSM::IncarnateContainer : public msm::front::state<> {
    template < class E, class F >
    void on_entry( E const&, F& );

    template < class E, class F >
    void on_exit( E const&, F& );
};




struct WorldFSM::Wait : public msm::front::state<> {
    template < class E, class F >
    void on_entry( E const&, F& );

    template < class E, class F >
    void on_exit( E const&, F& );
};








/**
* ����� ��� ������ � ������� �����.
* ������ � ������ ��� ���� ����: ����������, ����������, ����� � ��..
*/
class World :
    public Incarnate
{
public:
    /**
    * ������ ��������� ��� ����� �������� (������).
    */
    typedef msm::back::state_machine< WorldFSM >  FSM;




protected:
    /**
    * @exception CL_Exception
    */
    World( size_t K, size_t N, size_t M );




public:
    // ��� ������������� this->shared_from_this()
    static std::shared_ptr< World > valueOf(
        size_t K,
        size_t N,
        size_t M
    );




    virtual ~World();




    /**
    * @return ������ ���������.
    */
    std::shared_ptr< const FSM >  fsm() const;
    std::shared_ptr< FSM >  fsm();




    /**
    * @virtual VIncarnate
    */
    virtual void draw( std::shared_ptr< World > ) const;




    /**
    * @virtual VIncarnate
    */
    virtual inline typelib::size2Int_t needVisualSize() const {
        // # ������ ���� �� �����: �������� ���� ��� ����������.
        return typelib::size2Int_t( WINDOW_WIDTH, WINDOW_HEIGHT );
    }




    /**
    * @return ���� ����.
    */
    inline const CL_DisplayWindow* visual() const {
        return mVisualWorld.get();
    }

    inline CL_DisplayWindow* visual() {
        return mVisualWorld.get();
    }




    WorldFSM::incarnateSet_t const& incarnateSet() const;

    WorldFSM::incarnateSet_t& incarnateSet();




    /**
    * �������� ���.
    *
    * @return ������� ����.
    */
	void go();




    /**
    * @return ������� ����� � ����, ��.
    */
    inline int currentTime() const {
        return mCurrentTime;
    }




    /**
    * @return ���������� ��� ��������� shared_ptr �� ���� ������.
    */
    inline std::shared_ptr< const World > shared() const {
        return std::static_pointer_cast< const World >( this->shared_from_this() );
    }

    inline std::shared_ptr< World > shared() {
        return std::static_pointer_cast< World >( this->shared_from_this() );
    }




private:
    /**
    * ��������� ������� ����������� ����.
    */
	void onMouseDown( const CL_InputEvent&, const CL_InputState& );
	void onMouseMove( const CL_InputEvent&, const CL_InputState& );
	void onKeyboardDown( const CL_InputEvent&, const CL_InputState& );
	void onWindowClose();




    /**
    * @return ����� � ������� (Frame Per Second).
    */
    static size_t World::calcFPS( size_t frameTime );




private:
    /**
    * ������ ��������� ��� ����� ��������.
    */
    std::shared_ptr< FSM >  mFSM;


    /**
    * ������������ ���.
    */
    std::unique_ptr< CL_DisplayWindow >  mVisualWorld;


    /**
    * ������� �� ������.
    */
    CL_Slot  mSlotMouseDown;
    CL_Slot  mSlotMouseMove;
    CL_Slot  mSlotKeyboardDown;
    CL_Slot  mSlotWindowClose;


    /**
    * ������� �����, ��.
    * ����� �������������� ��� ����������� ��������.
    */
    int mCurrentTime;


    /**
    * ��� safe-�������.
    */
    mutable boost::mutex  mtxMouseAction;
};


} // matchn
