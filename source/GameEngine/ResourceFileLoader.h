#ifndef _RESOURCEFILELOADER_
#define _RESOURCEFILELOADER_

#include <string>

/** Loads a resource file
	 * resource file structure:
	 * texture UniqueStringID PathToImage/img.png
	 * shader UniqueStringID3 PathToShader/VertexShader.vert PathToShader/FragmentShader.frag
	 **/
void LoadResourceFile(class Game&, const std::string& file ,const std::string& folder = ".");

#endif // _RESOURCEFILELOADER_
