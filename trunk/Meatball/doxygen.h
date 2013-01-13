// This header file will define groups for the rest of the project
// The purpose of this is to have one centralized location for group organization

///@defgroup Engine Engine
/// The Engine Meatball runs on. Includes an Image manager and Sound Manager
// @{
	/// @defgroup Engine_Meatball		Meatball
	/// @defgroup Engine_Video			Video
	/// @defgroup Engine_Sprite			Sprite
	/// @defgroup Engine_Audio			Audio
	/// @defgroup Engine_Snd_Manager	Sound Manager
	/// @defgroup Engine_Image			Image
	/// @defgroup Engine_Image_Manager	Image Manager
	/// @defgroup Engine_Font			Font
	/// @defgroup Engine_Net			Networking
	/// @defgroup Engine_Misc			Misc
	/// @defgroup Engine_Gfx			Gfx
	/// @defgroup Engine_Framerate		Framerate
// @}

/// @defgroup Game
/// Grouped functions related to the Game's Core
// @{
	/// Modes of Gameplay
	/// @defgroup GAME_MODE Game Modes
// @}

/// @defgroup Input
/// @ingroup Game
// @{
		
		/// @defgroup Event_Handlers Event Handlers
		///< Tracks events such as window resize, mouse down, static key presses
		
		/// @defgroup HeldKeys_Handlers HeldKey Handlers
		///< Goes to a @link GAME_MODE @endlink specific handler to handle keys being held down.
	
// @}

/// @defgroup Guns
///< Weapons and Bullets information


/// @defgroup Level Levels
/// Levels and their data. 
/// @todo	How to Load level



/// @defgroup LevelEditor Level Editor
/// How to Edit Levels, Todo etc.
// @{
	/// @defgroup LE_Input Input
// @}
/// @defgroup Guns

/**
@defgroup Player Working with the Player
@brief Anything special relating to Player himself. The Star --MEATBALL--
*/
// @{
	/// @defgroup Camera
	/// @defgroup PLAYER_DIRECTIONS
// @}

///@defgroup Sprites
// @{
	/// @defgroup SPRITE_TYPES SPRITE_TYPES
// @}

/// @defgroup Console
/// In-game Console can track/manipulate variables and game phenomena
// @{
	/// @defgroup Console_Input Input
// @}