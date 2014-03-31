#pragma once

#include "../../Incarnate.h"


namespace matchn {


class World;


struct ContainerFSM :
    public msm::front::state_machine_def< ContainerFSM >
{
    ContainerFSM( Incarnate* );

    virtual ~ContainerFSM();


    /**
    * События при запуске и остановке машины.
    */
    template < class E, class F >
    void on_entry( E const&, F& );

    template < class E, class F >
    void on_exit( E const&, F& );


    /**
    * Состояния машины.
    *
    * # Состояния должны быть согласованы с Container::state().
    */
    struct Attention;
    struct Change;
    struct Drop;
    struct Same;
    struct Select;
    struct Wait;


    /**
    * Начальное состояние.
    */
    typedef Wait initial_state;


    /**
    * Воспринимаемые события (Event).
    */
    struct AttentionE {};
    struct CancelE {};
    struct PulseE {};
    struct SameE {};
    struct SelectE {};


    /**
    * Предпринимаемые действия (Action).
    */
    // attention
    void startAttentionA( const AttentionE& );

    // drop
    void startDropA( const PulseE& );
    void continueDropA( const PulseE& );
    void stopDropA( const PulseE& );

    // change
    void startChangeA( const SelectE& );
    void continueChangeA( const PulseE& );
    void stopChangeA( const PulseE& );

    // same
    void startSameA( const AttentionE& );
    void startSameA( const SameE& );
    void startSameA( const SelectE& );
    void continueSameA( const PulseE& );

    // select
    void startSelectA( const SelectE& );
    void continueSelectA( const PulseE& );

    // wait
    void startWaitA( const CancelE& );


    /**
    * Условные переходы (Guard).
    */
    // change
    bool firstSelectG( const SelectE& );
    bool secondSelectG( const SelectE& );
    bool completeChangeG( const PulseE& );
    bool continueChangeG( const PulseE& );

    // drop
    bool yesDropG( const PulseE& );
    bool noNeedDropG( const AttentionE& );
    bool noNeedDropG( const PulseE& );
    bool noNeedDropG( const SameE& );
    bool completeDropG( const PulseE& );
    bool continueDropG( const PulseE& );

    // same
    bool sameTimeOverG( const PulseE& );
    bool continueSameG( const PulseE& );


    /**
    * Таблица переходов.
    *
    * # Состояния в таблице переходов должны быть
    *   согласованы с Container::state().
    *
    * #! Больше 30-ти состояний - компилятор не справляется. И распорки
    *    в stdafx.h не помогают.
    *
    * @todo Попробовать разместить TT в куче > http://boost.org/doc/libs/1_53_0/libs/msm/doc/HTML/examples/FsmAsPtr.cpp
    */
    typedef ContainerFSM  self;
    struct transition_table : mpl::vector<
        //      (Current)    Event         (Next)       Action                      Guard
        //     +------------+-------------+------------+---------------------------+---------------------------+
        a_row < Attention,   SameE,        Same,        &self::startSameA >,
        //     +------------+-------------+------------+---------------------------+---------------------------+
          //row < Change,      PulseE,       Change,      &self::continueChangeA,     &self::continueChangeG >,
          row < Change,      PulseE,       Wait,        &self::stopChangeA,         &self::completeChangeG >,
        //     +------------+-------------+------------+---------------------------+---------------------------+
         //_row < Drop,        AttentionE,   Drop >,
          row < Drop,        PulseE,       Same,        &self::continueSameA,       &self::continueSameG >,
          row < Drop,        PulseE,       Drop,        &self::continueDropA,       &self::continueDropG >,
          row < Drop,        PulseE,       Wait,        &self::stopDropA,           &self::completeDropG >,
         _row < Drop,        SameE,        Drop >,
         _row < Drop,        SelectE,      Drop >,
        //     +------------+-------------+------------+---------------------------+---------------------------+
        a_row < Same,        AttentionE,   Same,        &self::startSameA >,
         _row < Same,        CancelE,      Wait >,
          row < Same,        PulseE,       Drop,        &self::startDropA,          &self::yesDropG >,
        g_row < Same,        PulseE,       Wait,                                    &self::sameTimeOverG >,
          row < Same,        PulseE,       Same,        &self::continueSameA,       &self::continueSameG >,
         _row < Same,        SameE,        Same >,
          row < Same,        SelectE,      Change,      &self::startChangeA,        &self::secondSelectG >,
          row < Same,        SelectE,      Select,      &self::startSelectA,        &self::firstSelectG >,
        //     +------------+-------------+------------+---------------------------+---------------------------+
         _row < Select,      AttentionE,   Select >,
        a_row < Select,      CancelE,      Wait,        &self::startWaitA >,
          row < Select,      PulseE,       Drop,        &self::startDropA,          &self::yesDropG >,
        a_row < Select,      PulseE,       Select,      &self::continueSelectA >,
         _row < Select,      SameE,        Select >,
        a_row < Select,      SelectE,      Wait,        &self::startSameA >,
        //     +------------+-------------+------------+---------------------------+---------------------------+
          row < Wait,        AttentionE,   Attention,   &self::startAttentionA,     &self::noNeedDropG >,
          row < Wait,        PulseE,       Drop,        &self::startDropA,          &self::yesDropG >,
          row < Wait,        SameE,        Same,        &self::startSameA,          &self::noNeedDropG >,
        a_row < Wait,        SelectE,      Select,      &self::startSelectA >
        //     +------------+-------------+------------+---------------------------+---------------------------+
    > {};


    /**
    * Ответ по умолчанию для неопределённого перехода.
    */
    template < class F, class E >
    inline void no_transition( E const& e,  F&,  int state ) {
        CONSOLE <<
            "Для состояния " << state << " не определён переход"
            " по событию '" << typeid( e ).name() << "'."
            "\n@see transition_table" <<
            std::endl;
    }




private:
    /**
    * @return Ниже есть контейнер или сам контейнер находится в нижнем ряду.
    */
    bool presentAboveOrLastRow() const;




    /**
    * @return Ниже все ячейки заполнены контейнерами или сам контейнер
    *         находится в нижнем ряду.
    */
    bool filledAllCellAbove() const;




public:
    /**
    * Ссылка на сам элемент.
    */
    Incarnate*  mIncarnate;

    /**
    * Переменные для отработки состояний.
    */
    typelib::coord2Int_t  stopVisualCoord;
    int  stopTimeSame;
};




struct ContainerFSM::Attention : public msm::front::state<> {
    template < class E, class F >
    inline void on_entry( E const&, F& fsm ) {
        //CONSOLE << "ContainerFSM::Attention::on_entry " << *fsm.mIncarnate << std::endl;
    }

    template < class E, class F >
    inline void on_exit( E const&, F& fsm ) {
        //CONSOLE << "ContainerFSM::Attention::on_exit " << *fsm.mIncarnate << std::endl;
    }

    inline operator nameState_t() const {
        return "attention";
    }
};




struct ContainerFSM::Change : public msm::front::state<> {
    template < class E, class F >
    inline void on_entry( E const&, F& fsm ) {
        //CONSOLE << "ContainerFSM::Change::on_entry " << *fsm.mIncarnate << std::endl;
    }

    template < class E, class F >
    inline void on_exit( E const&, F& fsm ) {
        //CONSOLE << "ContainerFSM::Change::on_exit " << *fsm.mIncarnate << std::endl;
    }

    inline operator nameState_t() const {
        return "change";
    }
};




struct ContainerFSM::Drop : public msm::front::state<> {
    template < class E, class F >
    inline void on_entry( E const&, F& fsm ) {
        //CONSOLE << "ContainerFSM::Drop::on_entry " << *fsm.mIncarnate << std::endl;
    }

    template < class E, class F >
    inline void on_exit( E const&, F& fsm ) {
        //CONSOLE << "ContainerFSM::Drop::on_exit " << *fsm.mIncarnate << std::endl;
    }

    inline operator nameState_t() const {
        return "drop";
    }
};




struct ContainerFSM::Same : public msm::front::state<> {
    template < class E, class F >
    inline void on_entry( E const&, F& fsm ) {
        //CONSOLE << "ContainerFSM::Same::on_entry " << *fsm.mIncarnate << std::endl;
    }

    template < class E, class F >
    inline void on_exit( E const&, F& fsm ) {
        //CONSOLE << "ContainerFSM::Same::on_exit " << *fsm.mIncarnate << std::endl;
    }

    inline operator nameState_t() const {
        return "same";
    }
};




struct ContainerFSM::Select : public msm::front::state<> {
    template < class E, class F >
    inline void on_entry( E const&, F& fsm ) {
        //CONSOLE << "ContainerFSM::Select::on_entry " << *fsm.mIncarnate << std::endl;
    }

    template < class E, class F >
    inline void on_exit( E const&, F& fsm ) {
        //CONSOLE << "ContainerFSM::Select::on_exit " << *fsm.mIncarnate << std::endl;
    }

    inline operator nameState_t() const {
        return "select";
    }
};




struct ContainerFSM::Wait : public msm::front::state<> {
    template < class E, class F >
    inline void on_entry( E const&, F& fsm ) {
        //CONSOLE << "ContainerFSM::Wait::on_entry " << *fsm.mIncarnate << std::endl;
    }

    template < class E, class F >
    inline void on_exit( E const&, F& fsm ) {
        //CONSOLE << "ContainerFSM::Wait::on_exit " << *fsm.mIncarnate << std::endl;
    }

    inline operator nameState_t() const {
        return "wait";
    }
};








/**
* Контейнер (что будем менять местами).
*/
class Container :
    public Incarnate
{
public:
    /**
    * Машина состояний для этого элемента (бекенд).
    */
    typedef msm::back::state_machine< ContainerFSM >  FSM;




protected:
    Container(
        std::shared_ptr< World >,
        const nameElement_t&,
        const typelib::coord2Int_t&  logicCoord
    );




public:
    static std::shared_ptr< Container >  valueOf(
        std::shared_ptr< World >,
        const nameElement_t&,
        const typelib::coord2Int_t&  logicCoord
    );




    virtual ~Container();




    /**
    * @return Машина состояний.
    */
    std::shared_ptr< const FSM >  fsm() const;
    std::shared_ptr< FSM >  fsm();




    /**
    * Текущее состояние машины.
    */
    nameState_t const&  state() const;




    /**
    * @virtual VIncarnate
    */
    virtual void draw( std::shared_ptr< World > ) const;




    /**
    * @virtual VIncarnate
    */
    virtual typelib::size2Int_t needVisualSize() const;




    /**
    * @return Случайный контейнер.
    */
    static nameElement_t nextRandom();




    /**
    * @return Количество доступных контейнеров.
    *
    * # Определяется по содержимому папки 'media/container'.
    */
    static size_t countAvailable();




protected:
    /**
    * Машина состояний для этого элемента.
    */
    std::shared_ptr< FSM >  mFSM;


    /**
    * Кеш.
    */
    static size_t countAvailable_;
};


} // matchn
