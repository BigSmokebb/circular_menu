

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
    Iter_Init(PLAYER_CM_ITEMS);
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
                //printf("El jugador %d ha clickeado en el círculo %d", playerid, i);
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

stock SHOW_CIRCULAR_MENU(playerid, menuid)
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
