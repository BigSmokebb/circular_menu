#include "a_samp"
#include "zcmd"
 
#include <textdraw-streamer>
#include "circular_menu" //Required include "textdraw-streamer"
 
enum
{
    NULL_CMMENU,//0
    MENU_CIRCULAR_PRUEBA,//1
    MENU_CIRCULAR_PRUEBA_2,//2
    MENU_CIRCULAR_PRUEBA_3//3
}
/*
    ADD_ITEM_MC
    (id del jugador, 
    "Texto que va aparecer arriba de cada circulo", 
    true, Verdadero para mostrar el sprite, falso para mostrar el objecto
    0, id del objecto solo se va mostrar si lo anterior es falso
    "ld_chat:thumbup" nombre del sprite que va aparecer si es verdadero
    );

*/

CMD:prueba(playerid)
{
    ADD_ITEM_MC(playerid, "Prueba1", true, 0, "ld_chat:thumbup");
    ADD_ITEM_MC(playerid, "Prueba2", false, 18631, "ld_chat:thumbup");
    ADD_ITEM_MC(playerid, "Prueba3", false, 18631, "ld_chat:thumbup");
    ADD_ITEM_MC(playerid, "Prueba4", false, 18630, "ld_chat:thumbup");

    SHOW_CIRCULAR_MENU(playerid, MENU_CIRCULAR_PRUEBA);
    return 1;
}
CMD:prueba2(playerid)
{
    ADD_ITEM_MC(playerid, "AYUDA", true, 0, "ld_chat:goodcha");
    ADD_ITEM_MC(playerid, "INVENTARIO", false, 18631, "hud:radar_tshirt");
    ADD_ITEM_MC(playerid, "PANEL", false, 19627, "hud:radar_gangy");
    ADD_ITEM_MC(playerid, "SOCIAL", true, 0, "hud:radar_modgarage");
    ADD_ITEM_MC(playerid, "GPS", true, 0, "hud:radar_waypoint");
    ADD_ITEM_MC(playerid, "TRABAJOS", true, 0, "ld_chat:thumbup");
    ADD_ITEM_MC(playerid, "VIP", true, 0, "ld_chat:badchat");

    SHOW_CIRCULAR_MENU(playerid, MENU_CIRCULAR_PRUEBA_2);
    return 1;
}
CMD:prueba3(playerid)
{
    ADD_ITEM_MC(playerid, "PRUEBA", true, 0, "ld_chat:goodcha");
    ADD_ITEM_MC(playerid, "PRUEBA", true, 0, "ld_chat:goodcha");
    ADD_ITEM_MC(playerid, "PRUEBA", false, 18631, "hud:radar_tshirt");
    ADD_ITEM_MC(playerid, "PRUEBA", false, 18631, "hud:radar_tshirt");
    ADD_ITEM_MC(playerid, "PRUEBA", false, 19627, "hud:radar_gangy");
    ADD_ITEM_MC(playerid, "PRUEBA", true, 0, "hud:radar_modgarage");
    ADD_ITEM_MC(playerid, "PRUEBA", true, 0, "hud:radar_modgarage");
    ADD_ITEM_MC(playerid, "PRUEBA", true, 0, "hud:radar_waypoint");
    ADD_ITEM_MC(playerid, "PRUEBA", true, 0, "hud:radar_waypoint");
    ADD_ITEM_MC(playerid, "PRUEBA", true, 0, "ld_chat:thumbup");
    ADD_ITEM_MC(playerid, "PRUEBA", true, 0, "ld_chat:badchat");
    ADD_ITEM_MC(playerid, "PRUEBA", true, 0, "ld_chat:badchat");
    ADD_ITEM_MC(playerid, "PRUEBA", true, 0, "ld_chat:badchat");
    ADD_ITEM_MC(playerid, "PRUEBA", true, 0, "ld_chat:badchat");
    ADD_ITEM_MC(playerid, "PRUEBA", true, 0, "ld_chat:badchat");
    SHOW_CIRCULAR_MENU(playerid, MENU_CIRCULAR_PRUEBA_3);
    return 1;
}
CMD:hide(playerid)
{
    CM_DestroyMenuEditor(playerid);
    return 1;
}


public OnCircularMenuResponse(playerid, itemid, menuid)
{
    switch(menuid)
    {
        case MENU_CIRCULAR_PRUEBA:
        {
            switch(itemid)
            {
                case 0: SendClientMessage(playerid,-1,"Item 1");
                case 1: SendClientMessage(playerid,-1,"Item 2");
                case 2: SendClientMessage(playerid,-1,"Item 3");
                case 3: SendClientMessage(playerid,-1,"Item 4");
                
            }
            
        }
        case MENU_CIRCULAR_PRUEBA_2:
        {
            switch(itemid)
            {
                case 0: SendClientMessage(playerid,-1,"Clic - AYUDA");
                case 1: SendClientMessage(playerid,-1,"Clic - INVENTARIO");
                case 2: SendClientMessage(playerid,-1,"Clic - PANEL");
                case 3: SendClientMessage(playerid,-1,"Clic - SOCIAL");
                case 4: SendClientMessage(playerid,-1,"Clic - GPS");
                case 5: SendClientMessage(playerid,-1,"Clic - TRABAJOS");
                case 6: SendClientMessage(playerid,-1,"Clic - VIP");
                
            }
            
        }
    }
    return 1;
}
