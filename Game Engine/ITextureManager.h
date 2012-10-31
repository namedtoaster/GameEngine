#ifndef _ITEXTUREMANAGER_
#define _ITEXTUREMANAGER_

#include <map>
#include <string>
#include <d3dx9.h>

struct TextureInfo
{
	unsigned int uiWidth;
	unsigned int uiHeight;
	unsigned int uiCells;
};

class ITextureManager
{
public:

	virtual ~ITextureManager() {}

	virtual void LoadAllTexturesFromFolder(const std::string& folder) = 0;
	virtual void LoadTexture(const std::string& file) = 0;
	virtual bool GetTextureInfo(const std::string& name, TextureInfo& out) const = 0;
	virtual void RemoveTexture(const std::string& name) = 0;
	virtual void RemoveAllTextures() = 0;

};



#endif //_TIEXTUREMANAGER_