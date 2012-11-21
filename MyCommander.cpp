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

	Vector2 target = (m_game->enemyTeam->flagScoreLocation.y > m_game->team->flagScoreLocation.y)? m_game->enemyTeam->flagScoreLocation - Vector2(0,16) : m_game->enemyTeam->flagScoreLocation + Vector2(0,16); 
	vector<Vector2> path;
    path.push_back(enemyFlagPosition);
    path.insert(path.begin(),target);

	//--------END CALCULATIONS--------//
	
	for(int index=0; index<m_game->bots_available.size(); ++index)
	//--------START LOGIC--------//
	{
		auto bot = m_game->bots_available[index];
		if (defender && *defender->health <= 0)
			defender = NULL;		
			
        if( (defender == NULL || defender == bot) &&  !bot->flag) //Assign a defender. 
		{
			defender = bot;
			if(((ourFlag - *bot->position).length() > 9.0f &&  (ourFlag - *bot->position).length() > 3.0f) && bot->flag == NULL)
			{
				issue(new ChargeCommand(bot->name, ourFlag, "Wgeting stage 3"));


			}else if(bot->health > 0 && m_game->bots_alive.size() == 1 && enemyFlag->position != m_game->enemyTeam->flagScoreLocation && !enemyFlag->carrier)
			{
				
				issue(new ChargeCommand(bot->name,enemyFlagPosition,"Somebody is installing propiertary software"));
				bot = attacker;

			}else
			{
				issue(new DefendCommand(bot->name, enemyFlagPosition-*bot->position, "Installing Gentoo"));
				attacker = NULL;

			}
			
		}else if(bot->flag != NULL)
            {
                issue(new ChargeCommand(bot->name,ourFlag, "Bringing home the freedom, rms would be proud")); //Needs proper route
				bot = attacker;				
				
				for( int eachBot=0;eachBot < m_game->bots_available.size(); eachBot++ )
				{
					if(/*bot->flag == NULL && */defender != bot)
					{
						auto bot = m_game->bots_available[eachBot];
						issue(new ChargeCommand(bot->name,ourFlag, "Maximum Freedom"));
												
					}
					index = m_game->bots_available.size(); 

				}

				//TODO: Camp on flagspawn and preech freedom to the enemies.
            }else 
			{
				//TODO: Have we fanned out into the map?
				if(enemyFlag->position == m_game->enemyTeam->flagScoreLocation)
				{
					issue(new ChargeCommand(bot->name, path, "Looking for free software"));	//Flanking
					attacker = bot;
				}else
				{
					issue(new ChargeCommand(bot->name, enemyFlagPosition, "Looking for free software"));	
					attacker = bot;
				}

			}
		

		   

	}
	//--------END LOGIC--------//
}

void
rms::shutdown()
{
    // Use this function to do stuff after the game finishes.
}

