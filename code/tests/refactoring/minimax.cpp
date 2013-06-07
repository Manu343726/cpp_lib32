/* 
 * File:   minimax.cpp
 * Author: Manu343726
 *
 * Created on 7 de junio de 2013, 19:15
 */

/*******************************************************************************
 * cpp_lib32 project. C++ port of the dx_lib32 project.                         *
 *                                                                              *
 * Copyright (C) 2012 - 2013, Manuel Sánchez Pérez                              *                     
 *                                                                              *
 * This file is part of cpp_lib32 project.                                      *
 *                                                                              *
 * cpp_lib32 project is free software: you can redistribute it and/or modify    *
 * it under the terms of the GNU Lesser General Public License as published by  *
 * the Free Software Foundation, version 2 of the License.                      *
 *                                                                              *
 * cpp_lib32 is distributed in the hope that it will be useful,                 *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               * 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                *
 * GNU Lesser General Public License for more details.                          *
 *                                                                              *
 * You should have received a copy of the GNU Lesser General Public License     *
 * along with cpp_lib32 project. If not, see <http://www.gnu.org/licenses/>.    *
 *******************************************************************************/

#include "dl32TestConfig.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_REFACTORING_TMP_MINIMAX

#include <iostream>

#include "dl32MetaprogrammingLibrary.h"

using namespace std;

enum TileCoords
{
    UPLEFT    = 0,
    UP        = 1,
    UPRIGHT   = 2,
    LEFT      = 3,
    CENTER    = 4,
    RIGHT     = 5,
    DOWNLEFT  = 6,
    DOWN      = 7,
    DOWNRIGHT = 8
};

enum TileValue
{
	PLAYER1,
	PLAYER2,
	EMPTY_TILE
};

template<TileValue value>
using Tile = dl32ValueWrapper<TileValue,value>;

template<TileValue... values> //Se supone que siempre vas a escribir nueve tiles
using Board = dl32TypeList<Tile<values>...>;

template<TileValue PLAYER> 
struct GetOpponent: public dl32ValueWrapper<TileValue , PLAYER == PLAYER1 ? PLAYER2 : PLAYER1> {}; 

template<typename BOARD , TileValue PLAYER>
struct PlayerWins
{
private:
    template<TileValue UPLEFT , TileValue UP , TileValue UPRIGHT , TileValue LEFT , TileValue CENTER , TileValue RIGHT , TileValue DOWNLEFT , TileValue DOWN , TileValue DOWNRIGHT>
    struct _wins<Board<UPLEFT,UP,UPRIGHT,LEFT,CENTER,RIGHT,DOWNLEFT,DOWN,DOWNRIGHT>> : public dl32BoolWrapper< 
    
                        ( UPLEFT == PLAYER && LEFT == PLAYER && DOWNLEFT == PLAYER )    /* left column */    ||
                        ( UP == PLAYER && CENTER == PLAYER && DOWN == PLAYER )          /* center column */  ||
                        ( UPRIGHT == PLAYER && RIGHT == PLAYER && DOWNRIGHT == PLAYER ) /* right column */   ||

                        ( UPLEFT == PLAYER && UP == PLAYER && UPRIGHT == PLAYER )       /* up row */         ||
                        ( LEFT == PLAYER && CENTER == PLAYER && RIGHT == PLAYER )       /* center row */     ||
                        ( DOWNLEFT == PLAYER && DOWN == PLAYER && DOWNRIGHT == PLAYER ) /* down row */       ||

                        ( UPLEFT == PLAYER && CENTER == PLAYER && DOWNRIGHT == PLAYER ) /* first diagonal */ ||
                        ( DOWNLEFT == PLAYER && CENTER == PLAYER && UPRIGHT == PLAYER ) /* second diagonal */
                                                                                                              > {};
                                                                                                              
public:
    static const bool result = _wins<BOARD>::value;
};

template<typename BOARD>
struct LeafBoard : public dl32BoolWrapper<PlayerWins<BOARD,PLAYER1>::result || PlayerWins<BOARD,PLAYER2>::result> {};

template<typename BOARD , typename LAST_PLAYER>
class GenerateNextBoards
{
private:
    
    template<typename GENERATED_BOARDS , typename CURRENT_BOARD , typename... ORIGINAL_BOARD_PROCESSED_TILES>
    struct _generate;
    
    template<typename... GENERATED_BOARDS , typename CURRENT_TILE , typename... NEXT_TILES , typename... ORIGINAL_BOARD_PROCESSED_TILES>
    struct _generate<dl32TypeList<GENERATED_BOARDS...> , dl32TypeList<CURRENT_TILE,NEXT_TILES...>,ORIGINAL_BOARD_PROCESSED_TILES...>
    {
        using generated_board = typename dl32tmp_if<CURRENT_TILE::value == EMPTY_TILE , // Player can play with this tile?
                                                    dl32TypeList<ORIGINAL_BOARD_PROCESSED_TILES...,LAST_PLAYER,NEXT_TILES...>, //YES (Play with it, puts its mark)
                                                    dl32TypeList<ORIGINAL_BOARD_PROCESSED_TILES...,CURRENT_TILE,NEXT_TILES...> //NO (Not plays with it, leave it as is)
                                                   >::type;
                
        using result = typename _generate<dl32TypeList<GENERATED_BOARDS...,generated_board> , dl32TypeList<NEXT_TILES...> , ORIGINAL_BOARD_PROCESSED_TILES...,CURRENT_TILE>::result;
    };
    
public:
    using result = typename _generate<dl32EmptyTypeList,BOARD>::result;
};

template<typename INIT_BOARD , TileValue PLAYER , unsigned int MAX_DEPTH = 2> 
class MINIMAX
{
private:
    
    template<unsigned int LEVEL , typename CURRENT_BOARD , TileValue CURRENT_PLAYER>
    struct _minimax
    {
        template<typename NEXT_BOARDS>
        struct _try_next_boards;
        
        template<typename CURRENT , typename... NEXTS>
        struct _try_next_boards<dl32TypeList<CURRENT,NEXTS...>>
        {
            static const bool player_wins_with_this_board = PlayerWins<CURRENT,PLAYER>::result;
            static const bool keep_trying = (PLAYER != CURRENT_PLAYER ? player_wins_with_this_board : !player_wins_with_this_board); //Tu oponente no quiere que ganes, así que no va a jugar una jugada que te haga ganar.
            
            using result = typename dl32tmp_if<keep_trying,
                                               typename _try_next_boards<NEXTS...>::result, //Sigue probando con las demás jugadas posibles
                                               typename _minimax<LEVEL+1,typename dl32TypeList<NEXTS...>::front,GetOpponent<CURRENT_PLAYER>::value>::result //El jugador actual juega ésta jugada. Sigue bajando por el arbol...
                                              >::type;
        };
        
        template<typename...>
        struct _try_next_boards<dl32EmptyTypeList>
        {
            //using result = 
        };
        
        static const bool is_leaf = LEVEL >= MAX_DEPTH || LeafBoard<CURRENT_BOARD>::value;
        
        using result = typename dl32tmp_if<is_leaft,CURRENT_BOARD,typename _try_next_boards<typename GenerateNextBoards<CURRENT_BOARD,CURRENT_PLAYER>::result>::result,
    };
};

int main()
{

    return 0;
}

#endif

