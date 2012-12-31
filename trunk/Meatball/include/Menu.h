#include "Globals.h"

struct MenuItemStruc
{
  std::string label;
  float x,y;
  float left,top,right,bottom;
  float width,height;
  //DWORD dwID;
};
 
   
class CMenu;
 
class CMenuItem
{
  MenuItemStruc m_MenuItemProps;
 
  //Possible pointers to previous and next menus
  //Enables menu items to lead to pop-ups and/or can
  //implement a PREV/NEXT setup
  CMenu *m_pNextMenu;
  CMenu *m_pPrevMenu;
   
  //Selection ID of menu item
  //Returned when this menu item has been selected
 
  bool m_bHasFrame;  //If frame is visible this is true
   
  public:
    //...
    //...
    bool Create(float x,float y,std::string lpszText,bool bHasFrame=false);
 
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
   bool AddItem(MenuItemStruc item);
   /*...
   ...*/
};