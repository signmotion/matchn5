#include "../../../include/stdafx.h"
#include "../../../include/ge/container/Container.h"
#include "../../../include/ge/world/World.h"


namespace matchn {


ContainerFSM::ContainerFSM(
    Incarnate*  ie
) :
    mIncarnate( ie )
{
}




ContainerFSM::~ContainerFSM() {
}




template < class E, class F >
void ContainerFSM::on_entry( E const&, F& ) {
    CONSOLE << "start ContainerFSM " << *mIncarnate << std::endl;
}


template < class E, class F >
void ContainerFSM::on_exit( E const&, F& ) {
    CONSOLE << "stop ContainerFSM " << *mIncarnate << std::endl << std::endl;
}




void
ContainerFSM::startAttentionA( const AttentionE& e ) {
    //CONSOLE << "ContainerFSM::startAttentionA " << *mIncarnate << std::endl;

    // переведЄм однотипные элементы в состо€ние Attention
    std::set< Container* >  hr;
    auto elc = static_cast< Container* >( mIncarnate );
    hr.insert( elc );
    const nameElement_t& name = elc->name();
    elc->world()->fsm()->harvestAllNeighbour< Container >( &hr, name, elc );
    for (auto itr = hr.begin(); itr != hr.end(); ++itr) {
        const auto& el = *itr;
        el->fsm()->process_event( SameE() );
    }
}




void
ContainerFSM::startChangeA( const SelectE& e ) {
    //CONSOLE << "ContainerFSM::startChangeA " << *mIncarnate << std::endl;

    // подготавливаем данные дл€ обмена контейнеров местами
    /* - @todo ...
    const auto& lc = mIncarnate->logicCoord();
    const auto& fsm = mIncarnate->world()->fsm();
    const auto ss = fsm->state< Container, Select >();
    DASSERT( (ss.size() == 2)
        && "ќжидали получить 2 выбранных контейнера." );

    const size_t place = (lc == ss[ 0 ]->logicCoord()) ? 1 : 0;
    stopVisualCoord = ss[ place ]->visualCoord();
    */
}




void
ContainerFSM::continueChangeA( const PulseE& e ) {
    //CONSOLE << "ContainerFSM::continueChangeA " << *mIncarnate << std::endl;

    const auto& current = mIncarnate->visualCoord();
    const auto direction = stopVisualCoord - current;
    const auto shift = direction / VISUAL_STEP_CHANGE + 1;
    const auto next = current + shift;
    mIncarnate->visualCoord( next );
}




void
ContainerFSM::stopChangeA( const PulseE& e ) {
    //CONSOLE << "ContainerFSM::stopChangeA " << *mIncarnate << std::endl;

    // вычисл€ем и фиксируем логическую координату контейнера
    const auto& vc = mIncarnate->visualCoord();
    const auto& lc =
        mIncarnate->world()->fsm()->logicByVisualCoordContainer( vc );
    mIncarnate->logicCoord( lc );
}




void
ContainerFSM::startDropA( const PulseE& e ) {
    //CONSOLE << "ContainerFSM::startDropA " << *mIncarnate << std::endl;

    // подготавливаем данные дл€ падени€
    const auto& lc = mIncarnate->logicCoord();
    stopVisualCoord =
        mIncarnate->world()->fsm()->visualByLogicCoordContainer( lc );
    const int height = mIncarnate->needVisualSize().y;
    stopVisualCoord.y += height;
}




void
ContainerFSM::continueDropA( const PulseE& e ) {
    //CONSOLE << "ContainerFSM::continueDropA " << *mIncarnate << std::endl;

    const auto& vc = mIncarnate->visualCoord();
    const int height = mIncarnate->needVisualSize().y;
    const int shiftY = height / VISUAL_STEP_DROP + 1;
    mIncarnate->visualCoord( typelib::coord2Int_t( vc.x,  vc.y + shiftY ) );
}




void
ContainerFSM::stopDropA( const PulseE& e ) {
    //CONSOLE << "ContainerFSM::stopDropA " << *mIncarnate << std::endl;

    // фиксируем логическую координату контейнера
    const auto& lc = mIncarnate->logicCoord();
    const typelib::coord2Int_t  nlc( lc.x,  lc.y - 1 );
    mIncarnate->logicCoord( nlc );

    // фиксируем визуальную координату контейнера
    const auto& vc =
        mIncarnate->world()->fsm()->visualByLogicCoordContainer( lc );
    mIncarnate->visualCoord( vc );
}




void
ContainerFSM::startSameA( const AttentionE& e ) {
    //CONSOLE << "ContainerFSM::startSameA " << *mIncarnate << std::endl;

    stopTimeSame = mIncarnate->world()->currentTime() + VISUAL_TIMEOUT_SAME;
}




void
ContainerFSM::startSameA( const SameE& e ) {
    startSameA( AttentionE() );
}




void
ContainerFSM::startSameA( const SelectE& e ) {
    startSameA( AttentionE() );
}




void
ContainerFSM::continueSameA( const PulseE& e ) {
    //CONSOLE << "ContainerFSM::continueSameA " << *mIncarnate << std::endl;

    const auto rotation = mIncarnate->visualRotation();
    mIncarnate->visualRotation( rotation + 1 );
}




void
ContainerFSM::startSelectA( const SelectE& e ) {
    //CONSOLE << "ContainerFSM::startSelectA " << *mIncarnate << std::endl;
}




void
ContainerFSM::continueSelectA( const PulseE& e ) {
    //CONSOLE << "ContainerFSM::continueSelectA " << *mIncarnate << std::endl;

    const auto rotation = mIncarnate->visualRotation();
    mIncarnate->visualRotation( rotation - 1 );
}




void
ContainerFSM::startWaitA( const CancelE& e ) {
    //CONSOLE << "ContainerFSM::startWaitA " << *mIncarnate << std::endl;
}




bool ContainerFSM::firstSelectG( const SelectE& ) {
    //CONSOLE << "ContainerFSM::firstSelectG " << *mIncarnate << std::endl;

    // выбран первый контейнер
    /* - @todo ...
    const auto& fsm = mIncarnate->world()->fsm();
    const auto countState = fsm->state< Container, Select >();
    return (countState.size() == 1);
    */
    return true;
}




bool ContainerFSM::secondSelectG( const SelectE& ) {
    //CONSOLE << "ContainerFSM::secondSelectG " << *mIncarnate << std::endl;

    // выбран второй контейнер
    /* - @todo ...
    const auto& fsm = mIncarnate->world()->fsm();
    const auto countState = fsm->state< Container, Select >();
    return (countState.size() == 2);
    */
    return false;
}




bool ContainerFSM::completeChangeG( const PulseE& e ) {
    //CONSOLE << "ContainerFSM::completeChangeG " << *mIncarnate << std::endl;

    return !continueChangeG( e );
}




bool ContainerFSM::continueChangeG( const PulseE& ) {
    //CONSOLE << "ContainerFSM::continueChangeG " << *mIncarnate << std::endl;

    const auto& current = mIncarnate->visualCoord();
    const auto direction = stopVisualCoord - current;
    const auto shift = direction / VISUAL_STEP_CHANGE + 1;
    const auto next = current + shift;
    return ( (direction.x > 0) && (next.x < stopVisualCoord.x)
          && (direction.y > 0) && (next.y < stopVisualCoord.y) )
       || (  (direction.x < 0) && (next.x < stopVisualCoord.x)
          && (direction.y < 0) && (next.y < stopVisualCoord.y) );
}




bool
ContainerFSM::yesDropG( const PulseE& e ) {
    return !presentAboveOrLastRow();
}




bool
ContainerFSM::noNeedDropG( const AttentionE& e ) {
    return filledAllCellAbove();
}




bool
ContainerFSM::noNeedDropG( const PulseE& e ) {
    return filledAllCellAbove();
}




bool
ContainerFSM::noNeedDropG( const SameE& e ) {
    return filledAllCellAbove();
}




bool
ContainerFSM::completeDropG( const PulseE& e ) {
    //CONSOLE << "ContainerFSM::completeDropG " << *mIncarnate << std::endl;

    const auto& vc = mIncarnate->visualCoord();
    return (vc.y >= stopVisualCoord.y);
}




bool
ContainerFSM::continueDropG( const PulseE& e ) {
    //CONSOLE << "ContainerFSM::continueDropG " << *mIncarnate << std::endl;

    const auto& vc = mIncarnate->visualCoord();
    return (vc.y < stopVisualCoord.y);
}




bool
ContainerFSM::continueSameG( const PulseE& e ) {
    //CONSOLE << "ContainerFSM::continueSameG " << *mIncarnate << std::endl;

    // врем€ вышло?
    return !sameTimeOverG( e );
}




bool
ContainerFSM::sameTimeOverG( const PulseE& e ) {
    //CONSOLE << "ContainerFSM::sameTimeOverG " << *mIncarnate << std::endl;

    // врем€ вышло?
    const auto ct = mIncarnate->world()->currentTime();
    return (ct > stopTimeSame);
}




bool
ContainerFSM::presentAboveOrLastRow() const {
    ASSERT( mIncarnate );

    //CONSOLE << "ContainerFSM::presentAboveOrLastRow " << *mIncarnate << std::endl;

    const auto& lc = mIncarnate->logicCoord();
    auto& fsm = mIncarnate->world()->fsm();
    if (lc.y == fsm->minCoord().y) {
        // нижний контейнер не падает
        return true;
    }

    // смотрим наличие контейнера ниже
    const auto shift = fsm->isc( 3 );
    const auto above = lc + shift;
    const auto& el = fsm->element< Container >( above );

    return el;
}




bool
ContainerFSM::filledAllCellAbove() const {
    ASSERT( mIncarnate );

    //CONSOLE << "ContainerFSM::absentEmptyCellAbove " << *mIncarnate << std::endl;

    const auto& lc = mIncarnate->logicCoord();
    auto& fsm = mIncarnate->world()->fsm();
    if (lc.y == fsm->minCoord().y) {
        // это нижний контейнер
        return true;
    }

    // смотрим наличие контейнеров ниже
    const auto shift = fsm->isc( 3 );
    for (auto above = lc + shift;  fsm->inside( above );  above += shift ) {
        const auto& el = fsm->element< Container >( above );
        if ( !el ) {
            // пуста€ €чейка
            return false;
        }
    }

    // все €чейки под контейнером заполнены
    return true;
}







size_t Container::countAvailable_ = 0;




Container::Container(
    std::shared_ptr< World >     w,
    const nameElement_t&         name,
    const typelib::coord2Int_t&  logicCoord
) :
    // контейнеры нарисуем после фона
    Incarnate( w, name, logicCoord, 1 ),
    // # ѕолна€ инициализаци€ машины состо€ний проходит в valueOf().
    mFSM( new FSM( nullptr ) )
{
}




std::shared_ptr< Container >
Container::valueOf(
    std::shared_ptr< World >     world,
    const nameElement_t&         name,
    const typelib::coord2Int_t&  logicCoord
) {
    auto r = std::shared_ptr< Container >( new Container( world, name, logicCoord ) );
    r->fsm()->mIncarnate = r.get();
    r->fsm()->start();
    return r;
}




Container::~Container() {
}




std::shared_ptr< const Container::FSM >
Container::fsm() const {
    return mFSM;
}


std::shared_ptr< Container::FSM >
Container::fsm() {
    return mFSM;
}




nameState_t const&  Container::state() const {

    // # —осто€ни€ должны быть согласованы с ContainerFSM.
    static const size_t N = 6;
    static const std::string stateName[ N ] = {
        static_cast< nameState_t >( ContainerFSM::Attention() ),
        static_cast< nameState_t >( ContainerFSM::Change() ),
        static_cast< nameState_t >( ContainerFSM::Drop() ),
        static_cast< nameState_t >( ContainerFSM::Same() ),
        static_cast< nameState_t >( ContainerFSM::Select() ),
        static_cast< nameState_t >( ContainerFSM::Wait() )
    };

    const size_t q = mFSM->current_state()[ 0 ];
    ASSERT( (q < N)
        && "«апрос неизвестного состо€ни€." );

    return stateName[ q ];
}




void Container::draw( std::shared_ptr< World >  w ) const {
    // #todo дл€ контейнера существует много состо€ний
    const nameState_t& ns = state();
    drawSprite( w->visual()->get_gc(), needVisualSize(), "container", name(), ns );
}




typelib::size2Int_t
Container::needVisualSize() const {
    // контейнер должен помещатьс€ в сетке мира N x M
    const auto vs = 
        typelib::size2_t( WINDOW_WIDTH, WINDOW_HEIGHT ) /
        static_cast< typelib::size2_t >( ELEMENT_SPRITE_SIZE ) /
        typelib::size2_t( world()->fsm()->N, world()->fsm()->M ) * 0.9;

    return static_cast< typelib::size2_t >( ELEMENT_SPRITE_SIZE ) * vs;
}




nameElement_t
Container::nextRandom() {

    static boost::mt19937  gen;
    // # ѕоследним всегда идЄт бонусный элемент: его надо заслужить, собира€
    //   вместе первые. ѕоэтому: -2.
    const size_t ne = countAvailable();
    ASSERT( (ne >= 2)
        && "¬ папке 'media/element' должно быть больше одного элемента." );
    static boost::uniform_int<>   uni( 'a', 'a' + ne - 2 );
    static boost::variate_generator< boost::mt19937&, boost::uniform_int<> >
        random( gen, uni );

    const auto ch = static_cast< char >( random() );
    return std::string( 1, ch );
}




size_t
Container::countAvailable() {
    if (countAvailable_ == 0) {
        std::set< std::string >  folder;
        typelib::file::listFolder( &folder, PATH_MEDIA + "/container" );
        countAvailable_ = folder.size();
        ASSERT( (countAvailable_ > 0)
            && "¬ папке 'media/container' не обнаружено ни одного элемента." );
    }

    return countAvailable_;
}


} // matchn
