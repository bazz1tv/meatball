#include "Globals.h"

struct MenuItem
{
  std::string label;
  float x,y;
  SDL_Rect rect;
  //DWORD dwID;
};
 
   
class CMenu;
 
class CMenuItem
{
  MenuItem m_MenuItemProps;
 
  //Possible pointers to previous and next menus
  //Enables menu items to lead to pop-ups and/or can
  //implement a PREV/NEXT setup
  CMenu *m_pNextMenu;
  CMenu *m_pPrevMenu;
   
  //Selection ID of menu item
  //Returned when this menu item has been selected
 
  bool m_bHasFocus;
   
  public:
    //...
    //...
    bool Create(float x,float y,std::string lpszText,bool bHasFocus=false);
 
    void SetText(std::string lpszText);
    //...
};
 
class CMenu
{
  std::vector<CMenuItem *> m_vMenuItems;
   
  /*....
   ....*/
  public:
   /*...
   ...*/
   bool AddItem(CMenuItem *pItem);
   bool AddItem(MenuItem item);
   /*...
   ...*/
};