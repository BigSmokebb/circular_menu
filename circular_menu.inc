// credits: BigSmoke_bb
/*
#if defined _menu_included
    #endinput
#endif

#define _menu_included

#include <a_samp>
#include <YSI\y_iterate>
#include <YSI\y_hooks>

const 
    MENU_MAX_ITEMS = 500,
    MENU_ITEM_COLOR = 168430310,
    MENU_MAIN_COLOR = -1523963137,
    MENU_MAX_ITEM_PER_PAGE = 12; // 
 

static
    count_tick[MAX_PLAYERS],
    player_menu_id[MAX_PLAYERS],
    Iterator:player_items[MAX_PLAYERS]<MENU_MAX_ITEMS>,
    player_item_list[MAX_PLAYERS][MENU_MAX_ITEMS][ 50 ],
    Float:eMenu_Data_Y[MAX_PLAYERS],Float:eMenu_Data_X[MAX_PLAYERS],
    player_item_color[MAX_PLAYERS] = {MENU_MAIN_COLOR, ...},
    PlayerText:_@m_ptd[MAX_PLAYERS][5] = {PlayerText:-1, ...},
    PlayerText:_@m_box[MAX_PLAYERS][MENU_MAX_ITEM_PER_PAGE] = {PlayerText:-1, ...},
    PlayerText:_@m_item[MAX_PLAYERS][MENU_MAX_ITEM_PER_PAGE] = {PlayerText:-1, ...},
    PlayerText:_@m_BUTTON[MAX_PLAYERS][ 4 ] = {PlayerText:-1, ...};
    //PlayerText:_@m_sub_item[MAX_PLAYERS][MENU_MAX_ITEM_PER_PAGE] = {PlayerText:-1, ...}; ko lam nua

forward OnMenuResponse(playerid, menuid, response, listitem);

stock Menu_Add(playerid, item[ 50 ])
{
    new 
        index = Iter_Free(player_items[playerid]);

    if(-1 < index < MENU_MAX_ITEMS)
    {
        strcat((player_item_list[playerid][index][0] = EOS, player_item_list[playerid][index]), item);
        Iter_Add(player_items[playerid], index);
        return 1;
    }
    return 0;
}

stock Menu_Show(playerid, menuid,  tittle[] = "Menu",  header[] = "Interaction")
{
    if(menuid <= 0) 
        return 0;
    
    PlayerTextDrawSetString(playerid, _@m_ptd[playerid][1], tittle);
    PlayerTextDrawSetString(playerid, _@m_ptd[playerid][3], header);
    PlayerTextDrawColor(playerid, _@m_ptd[playerid][0], player_item_color[playerid]);
    for(new i = 0; i < 5; i ++) 
    {
        PlayerTextDrawShow(playerid, _@m_ptd[playerid][i]);
    }
    Menu_ShowPage(playerid, 0);
    Menu_SelectIndex(playerid, 0);
    player_menu_id[playerid] = menuid;
    return 1;
}

stock Menu_Hide(playerid) 
{
    for(new i = 0; i < 5; i ++) 
    {
        PlayerTextDrawHide(playerid, _@m_ptd[playerid][i]);
    }
    for(new count = 0; count < MENU_MAX_ITEM_PER_PAGE; count ++) 
    {
        if(_@m_item[playerid][count] != PlayerText:INVALID_TEXT_DRAW) 
        {
            PlayerTextDrawDestroy(playerid, _@m_item[playerid][count]);
            _@m_item[playerid][count] = PlayerText:INVALID_TEXT_DRAW;
        }               
        if(_@m_box[playerid][count] != PlayerText:INVALID_TEXT_DRAW) 
        {
            PlayerTextDrawDestroy(playerid, _@m_box[playerid][count]);
            _@m_box[playerid][count] = PlayerText:INVALID_TEXT_DRAW;
        }               
    }
    for(new i = 0; i < 4; i++)
    {
        if(_@m_BUTTON[playerid][ i ] != PlayerText:INVALID_TEXT_DRAW) 
        {
            PlayerTextDrawDestroy(playerid, _@m_BUTTON[playerid][ i ]);
            _@m_BUTTON[playerid][ i ] = PlayerText:INVALID_TEXT_DRAW;
        }
    }
    Iter_Clear(player_items[playerid]);
    SetPVarInt(playerid, "Menu.SelectIndex", -1);
    return 1;
}

stock Menu_SetColor(playerid, color)
{
    player_item_color[playerid] = color;
    return 1;
}

static Menu_SelectIndex(playerid, index, cur_index = -1, bool:show = true) 
{
    if(cur_index != -1) 
    {
        if(_@m_box[playerid][cur_index] != PlayerText:INVALID_TEXT_DRAW) 
        {
            PlayerTextDrawColor(playerid, _@m_box[playerid][cur_index], MENU_ITEM_COLOR);
            if(show) PlayerTextDrawShow(playerid, _@m_box[playerid][cur_index]);
        }   
    }
    if(_@m_box[playerid][index] != PlayerText:INVALID_TEXT_DRAW) 
    {
        SetPVarInt(playerid, "Menu.SelectIndex", index);
        PlayerTextDrawColor(playerid, _@m_box[playerid][index], player_item_color[playerid]);
        if(show) PlayerTextDrawShow(playerid, _@m_box[playerid][index]);
    }

    new 
        str[22];

    count_tick[playerid] = GetTickCount() + 150;
    format(str, sizeof(str), "%d/%d", Menu_CurrentItem(playerid) + 1, Iter_Count(player_items[playerid]));
    PlayerTextDrawSetString(playerid, _@m_ptd[playerid][4], str);
    PlayerTextDrawShow(playerid, _@m_ptd[playerid][4]);     
    return 1;   
}

static Menu_ShowPage(playerid, page = 0) 
{
    for(new j = 0, i = page * MENU_MAX_ITEM_PER_PAGE; i < MENU_MAX_ITEM_PER_PAGE * (page + 1); i ++, j ++) 
    {
        if(Iter_Contains(player_items[playerid], i)) 
        {
            Menu_Create_TD(playerid, j, player_item_list[playerid][i]);
        }
        else 
        {
            if(_@m_item[playerid][j] != PlayerText:-1) 
            {
                PlayerTextDrawDestroy(playerid, _@m_item[playerid][j]);
                _@m_item[playerid][j] = PlayerText:-1;
            }           
            if(_@m_box[playerid][j] != PlayerText:-1) 
            {
                PlayerTextDrawDestroy(playerid, _@m_box[playerid][j]);
                _@m_box[playerid][j] = PlayerText:-1;
            }   
        }
    }
    Create_Buttons_Menu(playerid);
    SetPVarInt(playerid, "Menu.curPage", page);
    return 1;
}


static Menu_CurrentItem(playerid) 
{
    return MENU_MAX_ITEM_PER_PAGE * GetPVarInt(playerid, "Menu.curPage") + GetPVarInt(playerid, "Menu.SelectIndex");
}

stock Menu_GetMaxItemPerPage(playerid, cur_page) 
{
    new 
        count = 0;
    
    for(new i = MENU_MAX_ITEM_PER_PAGE * cur_page; i < MENU_MAX_ITEM_PER_PAGE * (cur_page + 1); i ++) 
    {
        if(Iter_Contains(player_items[playerid], i))
        {
            count++;
        }
    } 
    return count;
}

static Menu_Create_TD(playerid, index,  item[ 50 ]) 
{
    if(_@m_item[playerid][index] != PlayerText:-1) 
    {
        PlayerTextDrawDestroy(playerid, _@m_item[playerid][index]);
    }
    if(_@m_box[playerid][index] != PlayerText:-1) 
    {
        PlayerTextDrawDestroy(playerid, _@m_box[playerid][index]);
    }
    for(new i = 0; i < 4; i++)
    {
        if(_@m_BUTTON[playerid][ i ] != PlayerText:INVALID_TEXT_DRAW) 
        {
            PlayerTextDrawDestroy(playerid, _@m_BUTTON[playerid][ i ]);
            _@m_BUTTON[playerid][ i ] = PlayerText:INVALID_TEXT_DRAW;
        }
    }

    new 
        Float:Y = 200.548095 + (index * 11.300745);

    _@m_box[playerid][index] = CreatePlayerTextDraw(playerid, 294.966522, Y - 1.70 + index * 0.4, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, _@m_box[playerid][index], 110.030433, 11.699993);
    PlayerTextDrawAlignment(playerid, _@m_box[playerid][index], 1);
    PlayerTextDrawColor(playerid, _@m_box[playerid][index], MENU_ITEM_COLOR);
    PlayerTextDrawSetShadow(playerid, _@m_box[playerid][index], 0);
    PlayerTextDrawFont(playerid, _@m_box[playerid][index], 4);
    PlayerTextDrawSetSelectable(playerid, _@m_box[playerid][index], 1);
    PlayerTextDrawShow(playerid, _@m_box[playerid][index]);


    _@m_item[playerid][index] = CreatePlayerTextDraw(playerid, 296.366638 + 0.0, Y + 0.0 + index * 0.4, item);
    PlayerTextDrawLetterSize(playerid, _@m_item[playerid][index], 0.184666, 0.886074);
    PlayerTextDrawSetProportional(playerid, _@m_item[playerid][index], 1);
    PlayerTextDrawAlignment(playerid, _@m_item[playerid][index], 1);
    PlayerTextDrawSetShadow(playerid, _@m_item[playerid][index], 0);
    PlayerTextDrawColor(playerid, _@m_item[playerid][index], -1);
    PlayerTextDrawFont(playerid, _@m_item[playerid][index], 1);
    PlayerTextDrawShow(playerid, _@m_item[playerid][index]);



    eMenu_Data_Y[playerid] = Y - 1.70 + index * 0.4;
    eMenu_Data_X[playerid] = Y + 0.0 + index * 0.4;
   
    return 1;
}
static Create_Buttons_Menu(playerid)
{
    new Float:pepepe =  eMenu_Data_Y[playerid] + 11.70149 ;
    new Float:pipipip =  eMenu_Data_X[playerid] + 11.70149 ;
    _@m_BUTTON[playerid][ 0 ] = CreatePlayerTextDraw(playerid, 294.966522, pepepe , "LD_SPAC:white"); //Siguiente boton
    PlayerTextDrawTextSize(playerid, _@m_BUTTON[playerid][ 0 ], 54.030433, 11.699993);
    PlayerTextDrawAlignment(playerid, _@m_BUTTON[playerid][ 0 ], 1);
    PlayerTextDrawColor(playerid, _@m_BUTTON[playerid][ 0 ], MENU_ITEM_COLOR);
    PlayerTextDrawSetShadow(playerid, _@m_BUTTON[playerid][ 0 ], 0);
    PlayerTextDrawFont(playerid, _@m_BUTTON[playerid][ 0 ], 4);
    PlayerTextDrawSetSelectable(playerid, _@m_BUTTON[playerid][ 0 ], 1);
    PlayerTextDrawShow(playerid, _@m_BUTTON[playerid][ 0 ]);

    _@m_BUTTON[playerid][ 1 ] = CreatePlayerTextDraw(playerid, 296.366638 + 0.0, pipipip , "Siguiente");
    PlayerTextDrawLetterSize(playerid, _@m_BUTTON[playerid][ 1 ], 0.184666, 0.886074);
    PlayerTextDrawSetProportional(playerid, _@m_BUTTON[playerid][ 1 ], 1);
    PlayerTextDrawAlignment(playerid, _@m_BUTTON[playerid][ 1 ], 1);
    PlayerTextDrawSetShadow(playerid, _@m_BUTTON[playerid][ 1 ], 0);
    PlayerTextDrawColor(playerid, _@m_BUTTON[playerid][ 1 ], -1);
    PlayerTextDrawFont(playerid, _@m_BUTTON[playerid][ 1 ], 1);
    PlayerTextDrawShow(playerid, _@m_BUTTON[playerid][ 1 ]);

    _@m_BUTTON[playerid][ 2 ] = CreatePlayerTextDraw(playerid, 294.966522 + 55.0, pepepe , "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, _@m_BUTTON[playerid][ 2 ], 54.030433, 11.699993);
    PlayerTextDrawAlignment(playerid, _@m_BUTTON[playerid][ 2 ], 1);
    PlayerTextDrawColor(playerid, _@m_BUTTON[playerid][ 2 ], MENU_ITEM_COLOR);
    PlayerTextDrawSetShadow(playerid, _@m_BUTTON[playerid][ 2 ], 2);
    PlayerTextDrawFont(playerid, _@m_BUTTON[playerid][ 2 ], 4);
    PlayerTextDrawSetSelectable(playerid, _@m_BUTTON[playerid][ 2 ], 1);
    PlayerTextDrawShow(playerid, _@m_BUTTON[playerid][ 2 ]);

    _@m_BUTTON[playerid][ 3 ] = CreatePlayerTextDraw(playerid, 296.366638 + 0.0 + 55.0, pipipip, "Anterior");
    PlayerTextDrawLetterSize(playerid, _@m_BUTTON[playerid][ 3 ], 0.184666, 0.886074);
    PlayerTextDrawSetProportional(playerid, _@m_BUTTON[playerid][ 3 ], 1);
    PlayerTextDrawAlignment(playerid, _@m_BUTTON[playerid][ 3 ], 1);
    PlayerTextDrawSetShadow(playerid, _@m_BUTTON[playerid][ 3 ], 0);
    PlayerTextDrawColor(playerid, _@m_BUTTON[playerid][ 3 ], -1);
    PlayerTextDrawFont(playerid, _@m_BUTTON[playerid][ 3 ], 1);
    PlayerTextDrawShow(playerid, _@m_BUTTON[playerid][ 3 ]);

    SelectTextDraw(playerid, MENU_ITEM_COLOR);
    return 1;
}
stock Menu_Showing(playerid) 
{
    return GetPVarInt(playerid, "Menu.SelectIndex") == -1 ? 0 : 1;
}

stock Menu_GetMaxPage(playerid) 
{
    new 
        items = Iter_Count(player_items[playerid]);

    return floatround((items % MENU_MAX_ITEM_PER_PAGE == 0) ? items / MENU_MAX_ITEM_PER_PAGE : items / MENU_MAX_ITEM_PER_PAGE + 1);
}

hook OnPlayerConnect(playerid)
{
    if(IsPlayerNPC(playerid)) return 1;
    _@m_ptd[playerid][0] = CreatePlayerTextDraw(playerid, 296.966491, 140.937042, "LD_SPAC:white"); // Main BG
    PlayerTextDrawTextSize(playerid, _@m_ptd[playerid][0], 110.000000, 51.000000);
    PlayerTextDrawColor(playerid, _@m_ptd[playerid][0], player_item_color[playerid]);
    PlayerTextDrawSetShadow(playerid, _@m_ptd[playerid][0], 0);
    PlayerTextDrawFont(playerid, _@m_ptd[playerid][0], 4);

    _@m_ptd[playerid][1] = CreatePlayerTextDraw(playerid, 352.366882, 159.129791, "_"); // Main Tittle
    PlayerTextDrawLetterSize(playerid, _@m_ptd[playerid][1], 0.400000, 1.600000);
    PlayerTextDrawSetProportional(playerid, _@m_ptd[playerid][1], 1);
    PlayerTextDrawAlignment(playerid, _@m_ptd[playerid][1], 2);
    PlayerTextDrawSetShadow(playerid, _@m_ptd[playerid][1], 0);
    PlayerTextDrawColor(playerid, _@m_ptd[playerid][1], -1);
    PlayerTextDrawFont(playerid, _@m_ptd[playerid][1], 1);

    _@m_ptd[playerid][2] = CreatePlayerTextDraw(playerid, 296.966491, 191.840148, "LD_SPAC:white"); // Header BG
    PlayerTextDrawTextSize(playerid, _@m_ptd[playerid][2], 110.000000, 11.000000);
    PlayerTextDrawColor(playerid, _@m_ptd[playerid][2], 16843208);
    PlayerTextDrawAlignment(playerid, _@m_ptd[playerid][2], 1);
    PlayerTextDrawSetShadow(playerid, _@m_ptd[playerid][2], 0);
    PlayerTextDrawFont(playerid, _@m_ptd[playerid][2], 4);

    _@m_ptd[playerid][3] = CreatePlayerTextDraw(playerid, 294.000061, 193.359375, "Interaction"); // Header 
    PlayerTextDrawLetterSize(playerid, _@m_ptd[playerid][3], 0.123999, 0.766222);
    PlayerTextDrawSetProportional(playerid, _@m_ptd[playerid][3], 1);
    PlayerTextDrawAlignment(playerid, _@m_ptd[playerid][3], 1);
    PlayerTextDrawSetShadow(playerid, _@m_ptd[playerid][3], 0);
    PlayerTextDrawColor(playerid, _@m_ptd[playerid][3], -1);
    PlayerTextDrawFont(playerid, _@m_ptd[playerid][3], 2);

    _@m_ptd[playerid][4] = CreatePlayerTextDraw(playerid, 397.598144, 193.359298, #); // Page
    PlayerTextDrawLetterSize(playerid, _@m_ptd[playerid][4], 0.123999, 0.766222);
    PlayerTextDrawSetProportional(playerid, _@m_ptd[playerid][4], 1);
    PlayerTextDrawSetShadow(playerid, _@m_ptd[playerid][4], 0);
    PlayerTextDrawAlignment(playerid, _@m_ptd[playerid][4], 3);
    PlayerTextDrawColor(playerid, _@m_ptd[playerid][4], -1);
    PlayerTextDrawFont(playerid, _@m_ptd[playerid][4], 2);
    
    player_menu_id[playerid] = -1;
    for(new i = 0; i < MENU_MAX_ITEM_PER_PAGE; i ++) 
    {
        _@m_item[playerid][i] = PlayerText:-1;
        _@m_box[playerid][i] = PlayerText:-1;
    }

    SetPVarInt(playerid, "Menu.SelectIndex", -1);
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    if(IsPlayerNPC(playerid)) return 1;
    Iter_Clear(player_items[playerid]);
    for(new i = 0; i < 5; i ++) 
    {
        PlayerTextDrawDestroy(playerid, _@m_ptd[playerid][i]);
    }
    for(new i = 0; i < MENU_MAX_ITEM_PER_PAGE; i ++) 
    {
        if(_@m_item[playerid][i] != PlayerText:INVALID_TEXT_DRAW) 
        {
            PlayerTextDrawDestroy(playerid, _@m_item[playerid][i]);
            _@m_item[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
        }               
        if(_@m_box[playerid][i] != PlayerText:INVALID_TEXT_DRAW) 
        {
            PlayerTextDrawDestroy(playerid, _@m_box[playerid][i]);
            _@m_box[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
        }   
    }   
    return 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(Menu_Showing(playerid))
    {
        switch(newkeys)
        {
            case KEY_SECONDARY_ATTACK, KEY_SPRINT: 
            { 
                new 
                    response = (newkeys & KEY_SECONDARY_ATTACK) ? 1 : 0;
                
                if(player_menu_id[playerid] >= 0) 
                {
                    CallLocalFunction("OnMenuResponse", "dddd", playerid, player_menu_id[playerid], response, Menu_CurrentItem(playerid));
                }
            }           
        }
    }
    return 1;
}

hook OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
    if(player_menu_id[playerid] >= 0) 
    {
        new 
            nextIndex,
            curPage = GetPVarInt(playerid, "Menu.curPage"),
            oldIndex = GetPVarInt(playerid, "Menu.SelectIndex");

        if(playertextid  == _@m_BUTTON[playerid][ 0 ])
        {
            if(oldIndex + 1 >= MENU_MAX_ITEM_PER_PAGE) 
            {
                nextIndex = 0;
                Menu_ShowPage(playerid, ++curPage >= Menu_GetMaxPage(playerid) ? 0 : curPage);
            }
            else 
            {
                nextIndex = oldIndex + 1;
                if((MENU_MAX_ITEM_PER_PAGE * curPage + nextIndex) >= Iter_Count(player_items[playerid])) 
                {
                    nextIndex = 0;
                    Menu_ShowPage(playerid, 0);
                }
            }
            Menu_SelectIndex(playerid, nextIndex, oldIndex);    
        }
        if(playertextid  == _@m_BUTTON[playerid][ 2 ])
        {
            if(oldIndex - 1 <= -1) 
            {
                new 
                    page;
                
                Menu_ShowPage(playerid, --curPage <= -1 ? Menu_GetMaxPage(playerid) - 1: curPage);
                page = (curPage == -1) ? Menu_GetMaxPage(playerid) - 1 : GetPVarInt(playerid, "Menu.curPage");
                nextIndex = Menu_GetMaxItemPerPage(playerid, page) - 1;
            //  printf("<A> nextIndex=%d; curPage=%d;", nextIndex, curPage);
            }
            else 
            {
                nextIndex = oldIndex - 1;
                if((MENU_MAX_ITEM_PER_PAGE * curPage + nextIndex) >= Iter_Count(player_items[playerid])) 
                {
                    nextIndex = 0;
                    Menu_ShowPage(playerid, 0);
                }
            //  printf("<B> nextIndex=%d; curPage=%d;", nextIndex, curPage);
            }
            if(nextIndex <= -1)
            {
                new 
                    page = Menu_GetMaxPage(playerid) - 1;

                Menu_ShowPage(playerid, page);
                nextIndex = Menu_GetMaxItemPerPage(playerid, page - 1);
            //  printf("Page=%d; nextIndex=%d;", page, nextIndex);
            }
            Menu_SelectIndex(playerid, nextIndex, oldIndex);    
        }
    }
    return 1;
}
hook OnGameModeInit()
{
    Iter_Init(player_items);
}

*/
 //
//  Circular Menu by BigSmoke_bb
// Respetar creditos.
// - Discord: BigSmoke_bb
// - First version
//
// Version 1.0
//
//
//Macros/Defines/Funciones
#include <YSI\y_iterate>
#define CM_CircularMenu_VERSION                                   (10000) //a.b.c g+10000*a+100*b+c
#define CM_CircularMenu_LIBRARY_NAME                              "Circular Menu"
#define CM_CircularMenu_AUTHOR_NAME                               "BigSmoke_bb"
#define CM_CircularMenu_AUTHOR_DISCORD                            "bigsmoke_bb"
#if defined _samp_included
    #if (!defined GetPlayerPoolSize || !defined GetSVarInt)
        #error [OA EDITOR] This include requires SA:MP version 0.3.7
    #endif
    #define CM_CircularMenu_PLATFORM_NAME                        "SA:MP"
    #if defined GetPlayerCustomSkin
        #define CM_CircularMenu_PLATFORM_VERSION                 "0.3 DL"
    #else
        #define CM_CircularMenu_PLATFORM_VERSION                 "0.3.7"
    #endif
    #define CM_CircularMenu_PLATFORM_CODE                        CM_CircularMenu_PLATFORM_SAMP
#endif
#if !defined CM_CircularMenu_PLATFORM_CODE
    #define CM_CircularMenu_PLATFORM_CODE                        CM_CircularMenu_PLATFORM_PAWN
    #define CM_CircularMenu_PLATFORM_NAME                        "Pawn"
    #define CM_CircularMenu_PLATFORM_VERSION                     "3.0"
#endif
#define CM_EDITOR::                                        CM_
#define CM_GetVersion(%0)                                  ((%0) / 10000),(((%0) % 10000) / 100),(((%0) % 10000) % 100)

forward OnCircularMenuResponse(playerid, itemid, menuid);
/********************************
 * Definitions                  *
 ********************************/
#define MAX_OPTIONS 10
#define INITIAL_RADIUS 60.0
#define MAX_SIZE 70.0

#define ALSS_OPC    0x1
const  CIRCULAR_MENU_MAX_ITEM = 20;
/********************************
 * Enums                        *
 ********************************/
 
enum @CM_MENU_TEXTDRAWS
{
    PlayerText:CM_TEXTDRAW_SPRITE[ CIRCULAR_MENU_MAX_ITEM ],
    PlayerText:CM_TEXTDRAW_CIRCULAR[ CIRCULAR_MENU_MAX_ITEM ],
    PlayerText:CM_TEXTDRAW_TEXT[ CIRCULAR_MENU_MAX_ITEM ]
}
enum @CMSAVE_LIST_MENU
{   
    CM_TextName[ 50 ],
    bool:CM_IsSprite, //false = sprite || true = object
    CM_Object,
    CM_SpriteIMG[ 24 ],
    CM_SpriteName[ 24 ]
}
/********************************
 * Variables                        *
 ********************************/
new ALSS_CallBacks;

static 
        Iterator:PLAYER_CM_ITEMS[MAX_PLAYERS]<CIRCULAR_MENU_MAX_ITEM>,
        CM_PLAYER_CIRCULAR_IDMENU[ MAX_PLAYERS ],
        CM_MENU_CIRCULAR_TEXTDRAW[ MAX_PLAYERS ][ @CM_MENU_TEXTDRAWS ],
        CMSAVE_LIST_MENU[MAX_PLAYERS][ CIRCULAR_MENU_MAX_ITEM ][ @CMSAVE_LIST_MENU ],
        Float:angle_step;
// ===============================================================================================

public OnGameModeInit()
{
    Iter_Init(PLAYER_CM_ITEMS);
    cm_InfoScript();
    return (funcidx("ALSS_OnGameModeInit") != -1 ? CallLocalFunction("ALSS_OnGameModeInit", "") : 1);
}

#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit ALSS_OnGameModeInit
forward ALSS_OnGameModeInit();
// ===============================================================================================
public OnFilterScriptInit()
{
    cm_InfoScript();
    return (funcidx("ALSS_OnFilterScriptInit") != -1 ? CallLocalFunction("ALSS_OnFilterScriptInit", "") : 1);
}

#if defined _ALS_OnFilterScriptInit
    #undef OnFilterScriptInit
#else
    #define _ALS_OnFilterScriptInit
#endif
#define OnFilterScriptInit ALSS_OnFilterScriptInit

forward ALSS_OnFilterScriptInit();
// ===============================================================================================
public OnPlayerConnect(playerid)
{
 
    CM_PLAYER_CIRCULAR_IDMENU[ playerid ] = 0;
    return CallLocalFunction("Big_OnPlayerc", "i", playerid);
}

#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect Big_OnPlayerc

forward Big_OnPlayerc(playerid);
// ===============================================================================================
public OnPlayerDisconnect(playerid, reason)
{
    CM_DestroyMenuEditor(playerid);
    return (ALSS_CallBacks & ALSS_OPC ? CallLocalFunction("ALSS_OnPlayerConnect", "i", playerid) : 1);
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect ALSS_OnPlayerConnect
forward ALSS_OnPlayerConnect(playerid);
// ===============================================================================================
public OnClickDynamicPlayerTextDraw(playerid, PlayerText:textid)
{

    //Code
    if(CM_PLAYER_CIRCULAR_IDMENU[playerid] != 0)
    {
        foreach(new i : PLAYER_CM_ITEMS[playerid])
        {
            if(textid == CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_CIRCULAR ][ i ] 
                ||
                textid == CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_TEXT ][ i ])
            {
                // El jugador ha clickeado en un círculo
                printf("El jugador %d ha clickeado en el círculo %d", playerid, i);
                CallLocalFunction("OnCircularMenuResponse", "iii", playerid,i,CM_PLAYER_CIRCULAR_IDMENU[playerid]);
            }
 
        }
    }

    return CallLocalFunction("cm_OnPlayerClickPlayerTextDraw", "i", playerid);
}
#if defined _ALS_CM_PlayerCPlayerTextdraw
    #undef OnClickDynamicPlayerTextDraw
#else
    #define _ALS_CM_PlayerCPlayerTextdraw
#endif

#define OnClickDynamicPlayerTextDraw cm_OnPlayerClickPlayerTextDraw
//==================================================================== 
//====================================================================
public OnClickDynamicTextDraw(playerid, Text:textid)
{
    if(textid == Text:INVALID_TEXT_DRAW)
    {
        if(CM_PLAYER_CIRCULAR_IDMENU[playerid] != 0)
        {
            CM_DestroyMenuEditor(playerid);
            return 1;
        }
    }

    return CallLocalFunction("cm_OnPlayerClickTextDraw", "i", playerid);

}
#if defined _ALS_Oa_ClicTextdraw
    #undef OnClickDynamicTextDraw
#else
    #define _ALS_Oa_ClicTextdraw
#endif

#define OnClickDynamicTextDraw cm_OnPlayerClickTextDraw
//====================================================================

stock ADD_ITEM_MC(playerid, item[ 50 ], bool:ispsrite = true, cmobjectid = 18631, cm_spritename[] = "ld_chat:goodcha")
{
    new 
        index = Iter_Free(PLAYER_CM_ITEMS[playerid]);

    if(-1 < index < CIRCULAR_MENU_MAX_ITEM)
    {
        CMSAVE_LIST_MENU[playerid][index][ CM_IsSprite ] = ispsrite;
        CMSAVE_LIST_MENU[playerid][index][ CM_Object ] = cmobjectid;
 
        strcat((CMSAVE_LIST_MENU[playerid][index][ CM_TextName ] = EOS, CMSAVE_LIST_MENU[playerid][index][ CM_TextName ]), item);
        strcat((CMSAVE_LIST_MENU[playerid][index][ CM_SpriteIMG ] = EOS, CMSAVE_LIST_MENU[playerid][index][ CM_SpriteIMG ]), cm_spritename);
        Iter_Add(PLAYER_CM_ITEMS[playerid], index);

        //printf("%s",CMSAVE_LIST_MENU[playerid][index][ CM_TextName ] );
        return 1;
    }
    return 0;
}

stock SHOW_CIRCULAR_MENU(playerid, menuid)//,  tittle[] = "Menu",  header[] = "Interaction")
{
    if(menuid <= 0) 
        return 0;
    new pipixd = 0;

    if(CM_PLAYER_CIRCULAR_IDMENU[playerid] != 0)
    {
        CM_DestroyMenuEditor(playerid);
        return 1;
    }
    foreach(new i : PLAYER_CM_ITEMS[playerid])
    {
   
            //Menu_Create_TD(playerid, j, player_item_list[playerid][i]);

        if(CMSAVE_LIST_MENU[playerid][ i ][ CM_IsSprite ] == true)
        {
            CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_SPRITE ][ i ] = CreatePlayerTextDraw(playerid, 295.00, 235.00, CMSAVE_LIST_MENU[playerid][i][ CM_SpriteIMG ]);
            PlayerTextDrawTextSize(playerid, CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_SPRITE ][ i ] , 25.000000, 25.000000);
            PlayerTextDrawSetProportional(playerid, CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_SPRITE ][ i ] , 0);
            PlayerTextDrawColor(playerid, CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_SPRITE ][ i ] , 0xFFFFFF99);
            PlayerTextDrawFont(playerid, CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_SPRITE ][ i ] , 4);
            PlayerTextDrawSetSelectable(playerid, CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_SPRITE ][ i ] , 0);
        }
        else
        {
            CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_SPRITE ][ i ] = CreatePlayerTextDraw(playerid,295.00, 235.00, "_");
            PlayerTextDrawTextSize(playerid,CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_SPRITE ][ i ], 25.000000, 25.000000);
            PlayerTextDrawSetProportional(playerid,CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_SPRITE ][ i ], 0);
            PlayerTextDrawColor(playerid,CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_SPRITE ][ i ], 0xFFFFFFFF);
            PlayerTextDrawFont(playerid,CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_SPRITE ][ i ], 5);
            PlayerTextDrawBackgroundColor(playerid,CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_SPRITE ][ i ], 0xFFFFFF00);
            PlayerTextDrawSetSelectable(playerid,CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_SPRITE ][ i ], 0);
            PlayerTextDrawSetPreviewModel(playerid,CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_SPRITE ][ i ], CMSAVE_LIST_MENU[playerid][i][ CM_Object ]);
            PlayerTextDrawSetPreviewRot(playerid,CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_SPRITE ][ i ], -90.000000, 0.000000, 0.000000, 1.000000);
        }
        CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_CIRCULAR ][ i ] = CreatePlayerTextDraw(playerid, 295.00, 235.00, "ld_beat:chit");
        PlayerTextDrawTextSize(playerid, CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_CIRCULAR ][ i ], 60.000000, 60.000000);
        PlayerTextDrawSetProportional(playerid, CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_CIRCULAR ][ i ], 0);
        PlayerTextDrawColor(playerid, CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_CIRCULAR ][ i ], 0xFFFFFF90);
        PlayerTextDrawFont(playerid, CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_CIRCULAR ][ i ], 4);
        PlayerTextDrawSetSelectable(playerid, CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_CIRCULAR ][ i ], 0);
        PlayerTextDrawSetSelectable(playerid, CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_CIRCULAR ][ i ], 1);

        CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_TEXT ][ i ] = CreatePlayerTextDraw(playerid, 295.00, 240.00, CMSAVE_LIST_MENU[playerid][i][ CM_TextName ]);
        PlayerTextDrawTextSize(playerid, CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_TEXT ][ i ], 295.000000+40, 30.000000);
        PlayerTextDrawLetterSize(playerid, CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_TEXT ][ i ], 0.20, 0.8);
        PlayerTextDrawSetProportional(playerid, CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_TEXT ][ i ], 1);
        PlayerTextDrawColor(playerid, CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_TEXT ][ i ], 0xFFFFFF00);
        PlayerTextDrawFont(playerid, CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_TEXT ][ i ], 2);
        PlayerTextDrawAlignment(playerid, CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_TEXT ][ i ], 0);
        PlayerTextDrawSetOutline(playerid, CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_TEXT ][ i ], 0);
        PlayerTextDrawSetShadow(playerid, CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_TEXT ][ i ], 0);
        PlayerTextDrawSetSelectable(playerid, CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_TEXT ][ i ], 1);
        pipixd ++ ;
    }
    CreateCircularMenu(playerid,295,235, pipixd);

    CM_PLAYER_CIRCULAR_IDMENU[playerid] = menuid;
    return 1;
}

CreateCircularMenu(playerid,Float:x, Float:y, size)
{
    angle_step = 360.0 / size;
    new Float:radius = INITIAL_RADIUS;

    if(size > MAX_OPTIONS)
    {
        radius += (size - MAX_OPTIONS) * 10.0; // Ajusta este valor según sea necesario
    }
     
    for(new i = 0; i < size; i++)
    {
        new Float:angle = angle_step * i;
        new Float:pos_x = x + (radius * floatsin(-angle, degrees));
        new Float:pos_y = y + (radius * floatcos(-angle, degrees));

        DynamicPlayerTextDrawSetPos(playerid, CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_CIRCULAR ][ i ], pos_x, pos_y);
        DynamicPlayerTextDrawSetPos(playerid, CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_TEXT ][ i ] , pos_x+9, pos_y+3);
        PlayerTextDrawTextSize(playerid,CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_TEXT ][ i ] , pos_x+15+45, 50.000000);


        DynamicPlayerTextDrawSetPos(playerid,CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_SPRITE ][ i ] , pos_x+18, pos_y+18);

        if(size > MAX_OPTIONS)
        {
            new Float:scale = MAX_SIZE / size;
            PlayerTextDrawLetterSize(playerid,CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_CIRCULAR ][ i ], scale, scale);
            PlayerTextDrawLetterSize(playerid,CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_SPRITE ][ i ] , scale, scale);
        }

        PlayerTextDrawShow(playerid, CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_CIRCULAR ][ i ]);
        PlayerTextDrawShow(playerid, CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_TEXT ][ i ]);
        PlayerTextDrawShow(playerid, CM_MENU_CIRCULAR_TEXTDRAW[ playerid ][ CM_TEXTDRAW_SPRITE ][ i ]);
    }
    SelectTextDraw(playerid, 0x000000FF);
}
 
CM_DestroyMenuEditor(playerid)
{
    for(new i; i < sizeof(CM_MENU_CIRCULAR_TEXTDRAW[]); i++)
    {
        if(CM_MENU_CIRCULAR_TEXTDRAW[playerid][@CM_MENU_TEXTDRAWS:i] == INVALID_TEXT_DRAW) continue;
        PlayerTextDrawDestroy(playerid, PlayerText:CM_MENU_CIRCULAR_TEXTDRAW[playerid][@CM_MENU_TEXTDRAWS:i]);
        CM_MENU_CIRCULAR_TEXTDRAW[playerid][@CM_MENU_TEXTDRAWS:i] = INVALID_TEXT_DRAW;
    }
    CM_PLAYER_CIRCULAR_IDMENU[playerid] = 0;
    Iter_Clear(PLAYER_CM_ITEMS[playerid]);
    CancelSelectTextDraw(playerid);
    return 1;
}


 

cm_InfoScript()
{
    print(" ");
    print(" ======================== Circular Menu ==============================================");
    new ver_name[16];
    format(ver_name,sizeof(ver_name),"%d.%d.%d",CM_EDITOR::GetVersion(CM_CircularMenu_VERSION));
    printf("[OA EDITOR] Info: LOADED. %s v%s for %s %s by %s\n\nDiscord: %s\n",
        CM_CircularMenu_LIBRARY_NAME,
        ver_name,
        CM_CircularMenu_PLATFORM_NAME,
        CM_CircularMenu_PLATFORM_VERSION,
        CM_CircularMenu_AUTHOR_NAME,
        CM_CircularMenu_AUTHOR_DISCORD);
    print(" ======================== Circular Menu ==============================================");
    print(" ");
}