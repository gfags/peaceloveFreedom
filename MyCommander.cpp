#include <string>
#include <boost/none.hpp>
#include "api/GameInfo.h"
#include "api/Commands.h"
#include "api/Commander.h"
#include "api/CommanderFactory.h"
using namespace std;

class rms : public Commander
{
public:
    virtual string getName() const;
    virtual void initialize();
    virtual void tick();
    virtual void shutdown();
private:
	BotInfo *attacker;
    BotInfo *defender;

};

REGISTER_COMMANDER(rms);


string
rms::getName() const
{
    // change this to return the commander name
    return "Douchba/g/s";
}

void
rms::initialize()
{
     // Use this function to setup your bot before the game starts.
	attacker = NULL;
    defender = NULL;
	int defends=0;
}

void
rms::tick()
{
    // Use this function to do stuff each time a game update is received.
    // Here you can access all the information in m_level (information about the level being played)
    // and m_game (information about the current game state).
    // You can send commands to your bots using the issue member function
	//--------BEGIN CALCULATIONS--------//

	auto enemyFlag = m_game->enemyTeam->flag;
	auto ourFlag = m_game->team->flagScoreLocation;
    auto enemyFlagPosition = enemyFlag->position;

	Vector2 target = m_game->enemyTeam->flagScoreLocation + Vector2(0,16); //TODO needs to calculate position and chose correct Flank. + for left, - for right.
	vector<Vector2> path;
    path.push_back(enemyFlagPosition);
    path.insert(path.begin(),target);

	/*vector<Vector2> pathR;
	pathR.push_back(ourFlag);
	pathR.insert(pathR.begin(),target);*/
	//TODO define all the roles: apply to each member. 


	//--------END CALCULATIONS--------//
	
	for(int index=0; index<m_game->bots_available.size(); ++index)
	//--------START LOGIC--------//
	{
		auto bot = m_game->bots_available[index];
		if (defender && *defender->health <= 0)
			defender = NULL;		
			
        if( (defender == NULL || defender == bot) &&  !bot->flag) //TODO: Stop everbody protecting.
		{
			defender = bot;
			if(((ourFlag - *bot->position).length() > 9.0f &&  (ourFlag - *bot->position).length() > 3.0f))
			{
				issue(new ChargeCommand(bot->name, ourFlag, "Wgeting stage 3"));


			}else
			{
				issue(new DefendCommand(bot->name, enemyFlagPosition-*bot->position, "Installing Gentoo"));
				attacker = NULL;

			}
			
		}else if(bot->flag != NULL)
            {
                issue(new ChargeCommand(bot->name,ourFlag, "Bringing home the freedom, rms would be proud")); //Needs proper route
				bot = attacker;
				//TODO: Camp on flagspawn and preech freedom to the enemies.
            }else /*if((m_game->bots_available.size() - index) > 1)*/
		{
			issue(new ChargeCommand(bot->name, path, "Looking for free software"));			
			//Do we have the flag. Have we fanned out into the map?
			attacker = bot;

		}
		
		   

	}
	//--------END LOGIC--------//
}

void
rms::shutdown()
{
    // Use this function to do stuff after the game finishes.
}

