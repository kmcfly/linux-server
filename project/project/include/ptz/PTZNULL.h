#ifndef PTZNULL_H
#define PTZNULL_H

#include "PTZProtocol.h"
#undef  COMMANDLEN
#define COMMANDLEN 8

class CPTZNULL : public CPTZProtocol  
{
public:
	CPTZNULL();
	virtual ~CPTZNULL();
		
protected:

	bool Stop(){return false;}
	bool Left(){return false;}
	bool Right(){return false;}
	bool Up(){return false;}
	bool Down(){return false;}
	bool Near(){return false;}
	bool Far(){return false;}
	bool ZoomOut(){return false;}
	bool ZoomIn(){return false;}
	bool IrisOpen(){return false;}
	bool IrisClose(){return false;}

	//hby 增加四个方向
	bool LeftDown(){return false;}
	bool LeftUp(){return false;}
	bool RightDown(){return false;}
	bool RightUp(){return false;}

	bool GetPanSpeedRange(long& lMin,long& lMax){lMin = 0; lMax = 0; return false;}
	bool GetTiltSpeedRange(long& lMin,long& lMax){lMin = 0; lMax = 0; return false;}
	bool GetFocusSpeedRange(long& lMin,long& lMax){lMin = 0; lMax = 0; return false;}
	bool GetZoomSpeedRange(long& lMin,long& lMax){lMin = 0; lMax = 0; return false;}

	bool PresetGo(BYTE ucPresetID){return false;}
	bool PresetSet(BYTE ucPresetID){return false;}
};

#endif

