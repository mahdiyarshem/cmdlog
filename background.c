#ifndef GAME_CLIENT_COMPONENTS_BACKGROUND_H
#define GAME_CLIENT_COMPONENTS_BACKGROUND_H

#include <engine/shared/map.h>

#include <game/client/components/maplayers.h>

#include <stdint.h>

class CLayers;
class CMapImages;


#define CURRENT_MAP "%current%"

class CBackgroundEngineMap : public CMap
{
	MACRO_INTERFACE("background_enginemap", 0)
};

class CBackground : public CMapLayers
{
protected:
	IEngineMap *m_pMap;
	bool m_Loaded;
	char m_aMapName[MAX_MAP_LENGTH];

	
	int64_t m_LastLoad;


	CBackgroundEngineMap *m_pBackgroundMap;
	CLayers *m_pBackgroundLayers;
	CMapImages *m_pBackgroundImages;

	virtual CBackgroundEngineMap *CreateBGMap();

public:
	CBackground(int MapType = CMapLayers::TYPE_BACKGROUND_FORCE, bool OnlineOnly = true);
	virtual ~CBackground();
	virtual int Sizeof() const override { return sizeof(*this); }

	virtual void OnInit() override;
	virtual void OnMapLoad() override;
	virtual void OnRender() override;

	void LoadBackground();
};

#endif
