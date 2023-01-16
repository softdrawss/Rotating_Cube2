#ifndef __INPBUTTON_H__
#define __INPBUTTON_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class InpButton : public InpControl
{
public:

	InpButton(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~InpButton();

	bool Update(float dt);
	bool Draw(Render* render);

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;

	uint audioFxId;
};

#endif // __GUIBUTTON_H__