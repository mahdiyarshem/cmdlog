#ifndef GAME_SERVER_ACCOUNT_H
#define GAME_SERVER_ACCOUNT_H
#include <engine/server.h>
#include <game/server/player.h>

class CAccount
{
public:
	CAccount(CPlayer *pPlayer, CGameContext *pGameServer);

	void Login(char *Username, char *Password);
	void Register(char *Username, char *Password);
	void Apply();
	void Reset();
	void NewPassword(char *NewPassword);
	bool Exists(const char * Username);
	int NextID();
	
private:
	class CPlayer *m_pPlayer;
	class CGameContext *m_pGameServer;
	CGameContext *GameServer() const { return m_pGameServer; }
};

#endif
