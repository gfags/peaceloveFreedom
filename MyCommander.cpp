#include <string>
#include <boost/none.hpp>
#include "api/GameInfo.h"
#include "api/Commands.h"
#include "api/Commander.h"
#include "api/CommanderFactory.h"
#include <cstdlib>
using namespace std;

class rms : public Commander
{

public:
    virtual string getName() const;
    virtual void initialize();
    virtual void tick();
    virtual void shutdown();
	Vector2 getStallmansLunch(BotInfo* bot, Vector2 target);

private:
	BotInfo *attacker;
    BotInfo *defender;//[5] With 15 bots possible allocate a maximum of 5;

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
	//not implemented: vector<Vector2> hotspots; //Used to store visibleEnemies first seen x,y when in "our" zone.
	Vector2 target;
	
	//--------END CALCULATIONS--------//
	
	for(int index=0; index<m_game->bots_available.size(); ++index)
	//--------START LOGIC--------//
	{
		auto bot = m_game->bots_available[index];
		if (defender && *defender->health <= 0)
			defender = NULL;	
		/*if(bot->visibleEnemies.size() != 0)
			for(int botCheck = 0;botCheck < bot->visibleEnemies.size();botCheck++)
			{
				hotspots.push_back(*bot->visibleEnemies[botCheck]->position); //TODO: all positions of every bot ever is no good we need to find bots in our zone
			}*/

        if( (defender == NULL || defender == bot) &&  !bot->flag) //Assign a defender. Feature no 9001 - add a patrol defender
		{
			defender = bot;
			if(((m_game->team->flagSpawnLocation - *bot->position).length() > 9.0f &&  (m_game->team->flagSpawnLocation - *bot->position).length() > 3.0f) && bot->flag == NULL)
			{
				issue(new ChargeCommand(bot->name, ourFlag, "Wgeting stage 3"));


			}else if(m_game->bots_alive.size() == 1 && (enemyFlag->position - *bot->position).length() < (16) && bot->health > 0 && enemyFlag->position != m_game->enemyTeam->flagSpawnLocation && !enemyFlag->carrier)
			{
				
				issue(new ChargeCommand(bot->name,enemyFlagPosition,"Somebody is installing propiertary software"));
				bot = attacker;

			}else
			{
				vector<pair<Vector2, float>> faceMe;
				faceMe.push_back( make_pair( enemyFlagPosition-*bot->position,5 ) );
				faceMe.push_back( make_pair( *bot->position - enemyFlagPosition,2) );
				issue(new DefendCommand(bot->name, faceMe, "Installing Gentoo"));
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
					
					//providing flank was finished m_level->findRandomFreePositionInBox( target,enemyFlagPosition-Vector2( 8.0f,8.0f ),enemyFlagPosition+Vector2( 16.0f,16.0f ) );
					target = (m_game->enemyTeam->flagScoreLocation.y > m_game->team->flagScoreLocation.y)? m_game->enemyTeam->flagScoreLocation - Vector2(rand()%4,16) : m_game->enemyTeam->flagScoreLocation + Vector2(rand()%4,16); 
					vector<Vector2> path; /*path.push_back( getStallmansLunch( bot, target ) );*/	path.push_back( target );
					path.push_back(enemyFlagPosition);
					
					issue(new ChargeCommand(bot->name, path, "Looking for free software"));	//Flanking
					path.clear();
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
Vector2 rms::getStallmansLunch( BotInfo* bot, Vector2 target ) //incomplete flank. Find the middle of the map - choose a side and point bots down one edge.
{
	Vector2 enemyFlag = m_game->enemyTeam->flag->position;
	Vector2 ourFlag = m_game->team->flag->position;
	
	return Vector2( ( enemyFlag.x - ourFlag.x),( (rand()%1) == 1)? m_level->height : 0 );
		//*bot->position. ) 

}
void
rms::shutdown()
{
    // Use this function to do stuff after the game finishes.
}


