#pragma once  

class Player
{

public:

	//Constructor
	int getCash();
	int getLevel();
	void setCash(int cash);
	void setLevel(int level);


private:
	int m_level = 1;
	int m_cash = 600;
};