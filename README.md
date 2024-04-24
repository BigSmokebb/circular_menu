# circular_menu
Menu circular

ADD_ITEM_MC: Esta función añade un nuevo ítem al menú. Puedes especificar si el ítem es un sprite y qué objeto o sprite se debe usar.
SHOW_CIRCULAR_MENU: Esta función muestra el menú al jugador. Si el jugador ya tiene un menú abierto, lo cierra primero. Luego, crea un TextDraw para cada ítem en el menú.
CreateCircularMenu: Esta función organiza los TextDraws en un círculo. Si hay muchos ítems, hace el círculo más grande para que no se superpongan.
CM_DestroyMenuEditor: Esta función cierra el menú. Borra todos los TextDraws y limpia la lista de ítems del menú.
