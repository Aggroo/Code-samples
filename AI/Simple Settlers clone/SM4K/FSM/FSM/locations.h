#ifndef LOCATIONS_H
#define LOCATIONS_H


enum locations
{
	home1,
	home2,
	work1,
	work2,
	pub,
	restaurant,
	shop,
	graveyard
};


class locationList
{
public:
	locationList(){}
	~locationList(){}
	
	//Returns the position of the chosen location
	glm::vec2 getListOfLocations(locations name)
	{
		switch (name)
		{
		case home1:
			position = glm::vec2(400, 100);
			return position;
		case home2:
			position = glm::vec2(650, 100);
			return position;
		case work1:
			position = glm::vec2(500, 500);
			return position;
		case work2:
			position = glm::vec2(650, 550);
			return position;
		case pub:
			position = glm::vec2(280, 550);
			return position;
		case restaurant:
			position = glm::vec2(250, 340);
			return position;
		case shop:
			position = glm::vec2(680, 320);
			return position;
		case graveyard:
			position = glm::vec2(30, 670);
			return position;
		}
	}

private:
	glm::vec2 position;
};



#endif