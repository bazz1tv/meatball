#ifndef __PLAYLIST_H__
#define __PLAYLIST_H__

#include "boost.h"


namespace Playlist
{
	static char musicfiles[100][100];
	static int numsongs;
	static fs::path full_path;

	void ls_music();
	void musicover();
}

#endif