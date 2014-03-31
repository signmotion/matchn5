#include "../../../include/stdafx.h"
#include "../../../include/ge/background/Background.h"
#include "../../../include/ge/container/Container.h"
#include "../../../include/ge/world/World.h"


namespace matchn {


WorldFSM::WorldFSM(
    size_t K,
    size_t N,
    size_t M
) :
    K( K ), N( N ), M( M )
{
    ASSERT( (K >= 2 )
        && "Количество собираемых вместе элементов должно быть 2 или больше." );
    ASSERT( ( (N >= 3) && (M >= 3) )
        && "Размер поля не может быть меньше чем 3 x 3." );
    ASSERT( ( ((N % 2) != 0) && ((M % 2) != 0) )
        && "Размеры поля должны быть нечётными." );
}




WorldFSM::~WorldFSM() {
}




template < class E, class F >
void WorldFSM::on_entry( E const&, F& ) {
    CONSOLE << "start WorldFSM" << std::endl;
}

template < class E, class F >
void WorldFSM::on_exit( E const&, F& ) {
    CONSOLE << "stop WorldFSM" << std::endl << std::endl;
}




bool
WorldFSM::presentFreeCellG( const PulseE& e ) {
    //CONSOLE << "WorldFSM::presentFreeCellG" << std::endl;

    // проверяем наличие контейнеров в верхних ячейках
    typelib::coord2Int_t  cc( 0, maxCoord().y );
    for (cc.x = minCoord().x; cc.x <= maxCoord().x; ++cc.x) {
        // @test
        //if (cc.x != minCoord().x) { continue; }

        const std::shared_ptr< Incarnate > container =
            element< Container >( cc );
        if ( !container ) {
            // место по этой координате свободно
            return true;
        }
    } // for (cc.x = ...

    return false;
}


    
    
template < class F, class E >
void
WorldFSM::no_transition( E const& e,  F&,  int state ) {
    CONSOLE <<
        "Для состояния " << state << " не определён переход"
        " по событию '" << typeid( e ).name() << "'."
        "\n@see transition_table" <<
        std::endl;
}




typelib::coord2Int_t
WorldFSM::visualByLogicCoordContainer(
    const typelib::coord2Int_t&  lc
) const {
    const auto vss =
        typelib::size2_t( WINDOW_WIDTH, WINDOW_HEIGHT ) /
        typelib::size2_t( N, M ) * 0.9;
    const auto vnlc =
        static_cast< typelib::coord2_t >( lc ) *
        static_cast< typelib::coord2_t >( vss );
    const auto c = typelib::coord2Int_t(
        vnlc.x + CENTER_WINDOW_WIDTH,
       -vnlc.y + CENTER_WINDOW_HEIGHT
    );
    return c;
}




typelib::coord2Int_t
WorldFSM::logicByVisualCoordContainer(
    const typelib::coord2Int_t&  vc
) const {
    const auto lnvc = typelib::coord2_t(
        vc.x - CENTER_WINDOW_WIDTH,
      -(vc.y - CENTER_WINDOW_HEIGHT)
    );
    const auto vss =
        typelib::size2_t( WINDOW_WIDTH, WINDOW_HEIGHT ) /
        typelib::size2_t( N, M ) * 0.9;
    const auto c = lnvc / static_cast< typelib::coord2_t >( vss );
    return typelib::coord2Int_t(
        boost::math::round( c.x ),
        boost::math::round( c.y )
    );
}




template< class T >
std::shared_ptr< Incarnate >
WorldFSM::element( const typelib::coord2Int_t&  logicCoord ) {

    for (auto itr = mIncarnateSet.begin(); itr != mIncarnateSet.end(); ++itr) {
        if (typeid( T ) == typeid( **itr )) {
            const auto& lc = ( *itr )->logicCoord();
            if (lc == logicCoord) {
                return *itr;
            }
        }
    }

    return nullptr;
}



/*
template< class T, class S >
std::vector< std::shared_ptr< Incarnate > >
WorldFSM::state() const {

    std::vector< std::shared_ptr< Incarnate > >  r;

    // @todo ...

    return r;
}
*/



template < class E, class F >
void
WorldFSM::IncarnateContainer::on_entry( E const&, F& fsm ) {
    CONSOLE << "WorldFSM::IncarnateContainer::on_entry" << std::endl;

    // находим 1-ю пустую ячейку в верхнем ряду
    typelib::coord2Int_t  cc( 0, fsm.maxCoord().y );
    for (cc.x = fsm.minCoord().x; cc.x <= fsm.maxCoord().x; ++cc.x) {
        const std::shared_ptr< Incarnate > container =
            fsm.element< Container >( cc );
        if ( !container ) {
            // место по этой координате свободно
            break;
        }
    } // for (cc.x = ...
    ASSERT( (cc.x <= fsm.maxCoord().x)
        && "Охрана метода должна гарантировать наличие как минимум"
           " 1 свободной ячейки в верхнем ряду." );

    // воплощаем случ. контейнер в этой ячейке
    const nameElement_t name = Container::nextRandom();
    auto elu = Container::valueOf( fsm.mWorld.lock(), name, cc );
    auto els = std::shared_ptr< Container >( std::move( elu ) );
    auto elsi = std::static_pointer_cast< Incarnate >( els );
    fsm.mIncarnateSet.push_back( elsi );

    // # Элементы расположены в порядке отрисовки.
    // @todo optimize Добавлять сразу в нужное место.
    fsm.mIncarnateSet.sort( [] (
        const std::shared_ptr< Incarnate >&  a,
        const std::shared_ptr< Incarnate >&  b
    ) {
        return (a->visualOrder() < b->visualOrder());
    } );
}




template < class E, class F >
void
WorldFSM::IncarnateContainer::on_exit( E const&, F& fsm ) {
    CONSOLE << "WorldFSM::IncarnateContainer::on_exit" << std::endl;
}




template < class E, class F >
void
WorldFSM::Wait::on_entry( E const&, F& fsm ) {
    CONSOLE << "WorldFSM::Wait::on_entry" << std::endl;
}




template < class E, class F >
void
WorldFSM::Wait::on_exit( E const&, F& fsm ) {
    CONSOLE << "WorldFSM::Wait::on_exit" << std::endl;
}




template< typename T >
void
WorldFSM::harvestAllNeighbour(
    std::set< T* >*  r,
    const nameElement_t&  name,
    const T*  el
) {
    ASSERT( r );
    ASSERT( !name.empty() );
    ASSERT( el );

    // @see typelib::DynamicMapContent2D::isc()
    // # Учитываем диагонали (углы).
    for (size_t k = 1; k <= 8; ++k) {
        const typelib::coord2Int_t lc = el->logicCoord() + isc( k );
        if ( inside( lc ) ) {
            const auto i = ic( lc );
            ASSERT( (i < volume())
                && "Получен индекс элемента за пределами игрового мира." );
            auto other = element< T >( lc );
            // однотипные элементы
            if ( other && (other->name() == name) ) {
                auto otherCast = static_cast< Container* >( other.get() );
                const bool inserted = r->insert( otherCast ).second;
                if ( inserted ) {
                    // передадим нового соседа на отработку сюда же
                    harvestAllNeighbour( r, name, otherCast );
                }
            }
        } // if
    } // for ...
}


template
void
WorldFSM::harvestAllNeighbour(
    std::set< Container* >*,
    const nameElement_t&,
    const Container*
);








World::World(
    size_t K, size_t N, size_t M
) :
    // для World порядок отрисовки не важен
    Incarnate( nullptr, ".",  typelib::coord2Int_t::ZERO(), 0 ),
    mFSM( new FSM( K, N, M ) ),
    mCurrentTime( 0 )
{
    // *графический мир*
    // @source Example "SpritesRTS" from http://clanlib.org
    // @source Example "TileMap" from http://clanlib.org
    {
        // инициализируем ClanLib
	    CL_SetupCore setup_core;
	    CL_SetupDisplay setup_display;
	    #ifdef USE_SOFTWARE_RENDERER
		    CL_SetupSWRender setup_swrender;
	    #endif
	    #ifdef USE_OPENGL_1
		    CL_SetupGL1 setup_gl1;
	    #endif
	    #ifdef USE_OPENGL_2
		    CL_SetupGL setup_gl;
	    #endif
	    //CL_SetupSound setup_sound;

        std::stringstream  ss;
        ss << "Match" << K << "    " << N << " x " << M;
        CL_DisplayWindowDescription  desc( ss.str() );
	    desc.set_size( CL_Size( WINDOW_WIDTH, WINDOW_HEIGHT ),  false );
	    desc.set_allow_resize( false );
	    mVisualWorld =
            std::unique_ptr< CL_DisplayWindow >( new CL_DisplayWindow( desc ) );
    }


    // *мир звуков*
    // @todo mAudiuWorld = ...


    // подключаем слушателей событий от игрока
    {
        mSlotMouseDown =
            mVisualWorld->get_ic().get_mouse().sig_key_down()
            .connect( this, &World::onMouseDown );
        mSlotMouseMove =
            mVisualWorld->get_ic().get_mouse().sig_pointer_move()
            .connect( this, &World::onMouseMove );
	    mSlotKeyboardDown =
            mVisualWorld->get_ic().get_keyboard().sig_key_down()
            .connect( this, &World::onKeyboardDown );
        mSlotWindowClose =
            mVisualWorld->sig_window_close()
            .connect( this, &World::onWindowClose );
    }


    //mVisualWorld->hide_cursor();
}




std::shared_ptr< World >
World::valueOf( size_t K, size_t N, size_t M ) {
    return std::shared_ptr< World >( new World( K, N, M ) );
}




World::~World() {
    mFSM->stop();
}




std::shared_ptr< const World::FSM >
World::fsm() const {
    return mFSM;
}


std::shared_ptr< World::FSM >
World::fsm() {
    return mFSM;
}




void
World::draw( std::shared_ptr< World >  w ) const {
    auto& is = incarnateSet();
    for (auto itr = is.begin(); itr != is.end(); ++itr) {
        ( *itr )->draw( w );
    }
}




WorldFSM::incarnateSet_t const&
World::incarnateSet() const {
    return mFSM->mIncarnateSet;
}




WorldFSM::incarnateSet_t&
World::incarnateSet() {
    return mFSM->mIncarnateSet;
}

    
    
    
void
World::go() {

    // рассказываем машине состояний об этом мире и запускаем её
    mFSM->mWorld = shared();
    mFSM->start();

    // воплощаем обязательные элементы
    //incarnateBackground( "fable" );


    // показываем
    auto gc = mVisualWorld->get_gc();
    auto ic = mVisualWorld->get_ic();
    CL_Font  font( gc, "Tahoma", 16 );
    CL_BlendMode  blendTransparent;
    int lastTime = CL_System::get_time();
    for (auto keyboard = ic.get_keyboard();
         !keyboard.get_keycode( CL_KEY_ESCAPE );
    ) {
        // @source http://clanlib.org/wiki/MainDocs:Timing
		mCurrentTime = CL_System::get_time();
		int deltaTime = mCurrentTime - lastTime;
        // ждём: окно приложения могут перетаскивать
		if (deltaTime > 1000) { deltaTime = 1000; }
        // 'deltaTimeMs' можно использовать для корректного тайминга
		//const float deltaTimeMs = static_cast< float >( deltaTime );
        lastTime = mCurrentTime;


#ifdef _DEBUG
        // для FPS
        // @source http://clanlib.org/docs/clanlib-0.8.0/Overview/tipsandtricks.html
        const size_t fps = World::calcFPS( deltaTime );
#endif


        gc.clear( CL_Colorf::aquamarine );

        //gc.set_map_mode( cl_map_2d_upper_left );
        //gc.set_blend_mode( blendTransparent );


        // отправляем машинам события
        // @todo optimize Разнести по потокам.
        {
            mFSM->process_event( WorldFSM::PulseE() );
            auto& is = incarnateSet();
            for (auto itr = is.begin(); itr != is.end(); ++itr) {
                const auto& el = *itr;
                const auto& name = typeid( *el ).name();
                if (typeid( Container ) == typeid( *el )) {
                    auto elc = std::static_pointer_cast< Container >( el );
                    elc->fsm()->process_event( ContainerFSM::PulseE() );
                }
            }
        }


        draw( this->shared() );


#ifdef _DEBUG
        // покажем информацию для отладки
        {
            std::ostringstream ss;
            ss << fps;
            //const auto s = CL_StringHelp::int_to_text( fps );
            font.draw_text( gc, 10, 20, ss.str(), CL_Colorf::lightcoral );
        }
#endif

        mVisualWorld->flip( 1 );
        CL_KeepAlive::process();

		mCurrentTime = CL_System::get_time();
        // 10 ms (100 hz)
		static const int mainLoopRate = 10;
		const int timeToSleep = mainLoopRate - (mCurrentTime - lastTime);
		if (timeToSleep > 0) {
            CL_System::sleep( timeToSleep );
        }

    } // for (auto keyboard = ic.get_keyboard(); ...

}




void
World::onMouseDown( const CL_InputEvent& ie, const CL_InputState& is ) {

    boost::mutex::scoped_lock  lock( mtxMouseAction );

    const auto attentionPoint =
        typelib::coord2Int_t( ie.mouse_pos.x, ie.mouse_pos.y );
    const auto lc = fsm()->logicByVisualCoordContainer( attentionPoint );
    auto& it = fsm()->element< Container >( lc );
    if ( it ) {
        auto elc = std::static_pointer_cast< Container >( it );
        elc->fsm()->process_event( ContainerFSM::SelectE() );
    }
}




void
World::onMouseMove( const CL_InputEvent& ie, const CL_InputState& is ) {

    boost::mutex::scoped_lock  lock( mtxMouseAction );

    const auto attentionPoint =
        typelib::coord2Int_t( ie.mouse_pos.x, ie.mouse_pos.y );
    const auto lc = fsm()->logicByVisualCoordContainer( attentionPoint );
    auto& it = fsm()->element< Container >( lc );
    if ( it ) {
        auto elc = std::static_pointer_cast< Container >( it );
        elc->fsm()->process_event( ContainerFSM::AttentionE() );
    }
}




void
World::onKeyboardDown( const CL_InputEvent& ie, const CL_InputState& is ) {
}




void
World::onWindowClose() {
}




size_t
World::calcFPS( size_t frameTime ) {

	static size_t fpsResult  = 0;
	static size_t fpsCounter = 0;
	static size_t totalTime  = 0;
	
	totalTime += frameTime;
	if (totalTime >= 1000) {
		fpsResult = fpsCounter + 1;
		fpsCounter = totalTime = 0;
	}
	++fpsCounter;

	return fpsResult;
}


} // matchn
