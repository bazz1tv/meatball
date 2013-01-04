#include "playlist.h"


#include "Globals.h"


namespace Playlist
{
void ls_music()
{
	full_path = ( fs::initial_path<fs::path>() );
	full_path = fs::system_complete(fs::path(full_path.string() +"\\data\\music"));
	//printf("%s",full_path.string().c_str());
	fs::directory_iterator end_iter;
		int i=0;
		for ( fs::directory_iterator dir_itr( full_path ); dir_itr != end_iter; ++dir_itr )
		{
			//printf(" %d \n",i);
			 strcpy(musicfiles[i++],dir_itr->path().filename().string().c_str());
			//printf(" %d \n",i);
		}

		numsongs=i;
		for (i=0; i < numsongs; i++)
		{
			printf("%s\n",musicfiles[i]);
		}
}

void musicover()
{
	int index = Random(0,numsongs-1);
		char temp[200] = MUSIC_DIR;
		//MUSIC_DIR+
		strcat(temp,musicfiles[index]);

		
		printf("Song ended. loading %s", musicfiles[index]);
		pAudio->PlayMusik(temp, 1, 1);
}
}