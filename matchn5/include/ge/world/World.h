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
    * События при запуске и остановке машины.
    */
    template < class E, class F >
    void on_entry( E const&, F& );

    template < class E, class F >
    void on_exit( E const&, F& );


    /**
    * Состояния машины.
    */
    struct IncarnateContainer;
    struct Wait;


    /**
    * Начальное состояние.
    */
    typedef Wait initial_state;


    /**
    * Воспринимаемые события (Event).
    */
    struct PulseE {};


    /**
    * Предпринимаемые действия (Action).
    */
    // .


    /**
    * Условные переходы (Guard).
    */
    bool presentFreeCellG( const PulseE& );


    /**
    * Таблица переходов.
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
    * Ответ по умолчанию для неопределённого перехода.
    */
    template < class F, class E >
    void no_transition( E const& e,  F&,  int state );




    /**
    * @return Визуальные коорд. контейнера по логическим.
    */
    typelib::coord2Int_t visualByLogicCoordContainer(
        const typelib::coord2Int_t&  lc
    ) const;




    /**
    * @return Логические коорд. контейнера по визуальным.
    */
    typelib::coord2Int_t logicByVisualCoordContainer(
        const typelib::coord2Int_t&  vc
    ) const;




    /**
    * @return Элемент вида T в заданной ячейке мира или nullptr,
    *         если такого элемента нет.
    */
    template< class T >
    std::shared_ptr< Incarnate > element(
        const typelib::coord2Int_t&  logicCoord
    );



    /**
    * @return Элементы вида T со статусами S.
    */
    //template< class T, class S >
    //std::vector< std::shared_ptr< Incarnate > >  state() const;




    // @todo Вынести в отдельный класс typelib.
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
            && "Вычисленная 2D координата лежит за пределами заданной карты." );

        return c;
    }




    static inline typelib::coord2Int_t isc( size_t cell ) {
        using namespace typelib;
        ASSERT( (cell < 9)
            && "Попытка получить доступ за пределами 2D-пространства." );
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
    * Собирает в 'r' все однотипные элементы с именем 'name'.
    *
    * # В список включаются только соседи 'el'.
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
    * Кол-во собираемых вместе элементов и размер мира.
    *
    * # Делаем открытыми константами, не плодим gett'еры.
    */
    const size_t  K;
    const size_t  N;
    const size_t  M;

    
    /**
    * Воплощённые в мире элементы.
    * # Сам World (тоже игровой элемент) вне этого списка.
    * # Элементы расположены в порядке отрисовки.
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
* Класс для работы с игровым миром.
* Создаёт и хранит все миры игры: физический, визуальный, аудио и пр..
*/
class World :
    public Incarnate
{
public:
    /**
    * Машина состояний для этого элемента (бекенд).
    */
    typedef msm::back::state_machine< WorldFSM >  FSM;




protected:
    /**
    * @exception CL_Exception
    */
    World( size_t K, size_t N, size_t M );




public:
    // Для инициализации this->shared_from_this()
    static std::shared_ptr< World > valueOf(
        size_t K,
        size_t N,
        size_t M
    );




    virtual ~World();




    /**
    * @return Машина состояний.
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
        // # Размер мира не важен: рисуются лишь его содержимое.
        return typelib::size2Int_t( WINDOW_WIDTH, WINDOW_HEIGHT );
    }




    /**
    * @return Миры мира.
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
    * Оживляет мир.
    *
    * @return Команда мира.
    */
	void go();




    /**
    * @return Текущее время в мире, мс.
    */
    inline int currentTime() const {
        return mCurrentTime;
    }




    /**
    * @return Сокращение для получения shared_ptr на этот объект.
    */
    inline std::shared_ptr< const World > shared() const {
        return std::static_pointer_cast< const World >( this->shared_from_this() );
    }

    inline std::shared_ptr< World > shared() {
        return std::static_pointer_cast< World >( this->shared_from_this() );
    }




private:
    /**
    * Слушатели событий визуального мира.
    */
	void onMouseDown( const CL_InputEvent&, const CL_InputState& );
	void onMouseMove( const CL_InputEvent&, const CL_InputState& );
	void onKeyboardDown( const CL_InputEvent&, const CL_InputState& );
	void onWindowClose();




    /**
    * @return Кадры в секунду (Frame Per Second).
    */
    static size_t World::calcFPS( size_t frameTime );




private:
    /**
    * Машина состояний для этого элемента.
    */
    std::shared_ptr< FSM >  mFSM;


    /**
    * Отображаемый мир.
    */
    std::unique_ptr< CL_DisplayWindow >  mVisualWorld;


    /**
    * События от игрока.
    */
    CL_Slot  mSlotMouseDown;
    CL_Slot  mSlotMouseMove;
    CL_Slot  mSlotKeyboardDown;
    CL_Slot  mSlotWindowClose;


    /**
    * Текущее время, мс.
    * Может использоваться для корректного тайминга.
    */
    int mCurrentTime;


    /**
    * Для safe-потоков.
    */
    mutable boost::mutex  mtxMouseAction;
};


} // matchn
