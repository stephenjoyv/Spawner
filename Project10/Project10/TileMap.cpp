#include "TileMap.h"
void Map::draw()
{
	int t = 0;
	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			for (int k = 0; k < info_z[i][j]; k++)
			{
				ownmap[i][j][k]->draw();
				t++;
			}
			
		}

	}
	//std::cout << "tyles drawned = " << t << '\n';
}

void Map::click(std::tuple<int,int,int>&data)
{
	bool clicked = false;
	for (int i = 40-1; i >= 0; i--)
	{
		for (int j = 40-1; j >=0; j--)
		{
			int k = info_z[i][j]; 
			for (int z = k-1; z >= 0; z--)
			{
				ownmap[i][j][z]->setActive();
				//std::cout << ownmap[i][j]->getSize().x << '\n';
				if (ownmap[i][j][z]->clicked)
				{
					cout << "COORDINATES | " << "x : " << i << " | " << "y : " << j << " | " << "z : " << z << '\n';
					cout << "POSITION " << "X : " << ownmap[i][j][z]->getPosition().x << " Y : " << ownmap[i][j][z]->getPosition().y << '\n';
					data = std::tuple<int, int, int>{ i,j,z };


					ownmap[i][j][z]->clicked = false;
					
					clicked = true;
					break;
				}
			}
			if (clicked) break;		
		}
		if (clicked) break;
	}
	if (!clicked) {
		data = std::tuple<int, int, int>{ -1,-1,-1 };
		std::cout << "wasn't drawn click\n";
	}
}

Tile* Map::getTile(int x, int y)
{
	return ownmap[x][y][0];
}

void Map::setTile(Tile* tile,int x,int y,int z)
{
	*ownmap[x][y][z] = *tile;
	std::cout << "hdhawdakdk\n";
}

void Map::addTile(Tile* tile, int x, int y)
{
	++info_z[x][y];
	int z = info_z[x][y]-1;
	cout << "z is " << z << '\n';
	ownmap[x][y][z] = new Tile{};
	*ownmap[x][y][z] = *tile;
	int nx,ny;
	if (info_z[x][y] > 0) {
		//cout << "COMPONENTS ";
		nx = ownmap[x][y][z - 1]->getPosition().x;
		ny = ownmap[x][y][z - 1]->getPosition().y + ownmap[x][y][z - 1]->getSize().y / 2;
		cout << "NEW TILE POSITION | X : " << nx << " Y : " << ny << '\n';
		
	}
	else return;
	ownmap[x][y][z]->setPosition(nx,ny);
}

bool Map::controlTile(int x, int y, int z)
{
	if (z == info_z[x][y]) {
		return true;
	}
	return false;
}

std::vector<string> Map::splitter(string symbols) {
	std::vector<string> m;
	string tmp = "";
	for (int i = 0; i < symbols.size(); i++)
	{
		if (symbols[i] != ' ')
		{
			tmp += symbols[i];
		}
		else
		{
			m.emplace_back(tmp);
			tmp = "";
		}
	}
	return m;
}
Tile& Tile::operator=(const Tile& copy)
{
	//std::cout << "part 1\n";
	if (this != &copy)
	{
		int x = 0, y = 0;
		

		if (this->texture!=nullptr)
		{
			
			delete this->texture;
		}
		if (this->sprite != nullptr) {
			x = this->sprite->getPosition().x, y = this->sprite->getPosition().y;
			delete this->sprite;
		}
		if (this->image != nullptr) {
			delete this->image;
		}

		

		this->texture = new Texture;
		this->sprite = new Sprite;
		this->image = new Image;

		this->mouse = copy.mouse;
		*(this->texture) = *copy.texture;
		//std::cout << "part 2\n";
		*this->sprite = *copy.sprite;
		if(x!=0 && y!=0)this->sprite->setPosition(x, y);
		pool_window[0].get()->draw(*this->sprite);
		*this->image = *copy.image;
		//*this->borders = *copy.borders;
		this->clicked = 0;
		//this->is_bordered = copy.
		this->link = copy.link;
		std::cout << "copy pixel "<<  copy.image->getSize().x << '\n';
		std::cout << "orig pixel " << this->image->getSize().x<< '\n';
		std::cout << "LINK = " << this->link<<'\n';

		//*this = copy;
		//std::cout << "dwada\n";
		cout << "was copied\n";
	}
	//std::cout << "part 3\n";
	return *this;
	// TODO: �������� ����� �������� return
}
Tile::~Tile()
{
	if (this->texture != nullptr)delete this->texture;
	if (this->sprite != nullptr)delete this->sprite;
	if (this->image != nullptr)delete this->image;
}
Tile::Tile(const Tile& m) {
	this->sprite = m.sprite;
	this->texture = m.texture;
	this->mouse = m.mouse;
	this->image = m.image;
	this->ObjTar = m.ObjTar;
}
Tile::Tile(string str, Mouse* mouse) : DetectedImage(str, mouse) {}
Tile::Tile()
{

}