#include "Game.h"
#include "TileMap.h"
#include "TilePanel.h"
#include "interfaceobj.h"
#include "Platform.h"
#include "Singleton.h"
using namespace std;
using namespace sf;

DetectedImage::DetectedImage(string str, Mouse* mouse):DetectedImage() {
	link = str;
	ObjTar = Singleton::instance().getPoolWindow()[0].get();
		//pool_window[0].get();
	image = new Image;
	texture = new Texture;
	sprite = new Sprite;
	this->mouse = mouse;
	image->loadFromFile(link.c_str());
	texture->loadFromImage(*image);
	sprite->setTexture(*texture);
	setOrigin(0, texture->getSize().y * 3 / 4);
	clicked = false;
	update();
}
DetectedImage::DetectedImage() {
	this->texture = nullptr;
	this->sprite = nullptr;
	this->image = nullptr;
	//std::cout << "sprite n = " << (this->sprite != nullptr);
	ObjTar = Singleton::instance().getPoolWindow()[0].get();
	is_bordered = false;
	scale = Vector2i(1, 1);
}
void DetectedImage::setPosition(int x, int y)
{
	sprite->setPosition(x, y);
	update();
	if (is_bordered) borders->setPosition(pos_x, pos_y);
}
void DetectedImage::setOrigin(int x, int y)
{
	sprite->setOrigin(x, y);
	sprite->setPosition(pos_x+sprite->getOrigin().x, pos_y + sprite->getOrigin().y);
}
void DetectedImage::setTarget(RenderTarget* target)
{
	ObjTar = target;
}
void DetectedImage::init_border()
{
	if (!is_bordered)
	{
		is_bordered = true;
		borders = new RectangleShape;
		update();
		borders->setSize(Vector2f(size_x, size_y));
		borders->setPosition(pos_x, pos_y);
		borders->setOutlineColor(Color::White);
		borders->setOutlineThickness(4);
		borders->setFillColor(*Singleton::instance().getMainColor());
	}
	
}
Vector2i DetectedImage::getScale()
{
	return scale;
}
Vector2f DetectedImage::getPosition()
{
	update();
	return Vector2f(pos_x, pos_y);
}
Vector2f DetectedImage::getSize()
{
	update();
	return Vector2f(size_x,size_y);
}
string DetectedImage::getLink()
{
	return link;
}
void DetectedImage::Scale(int xmod, int ymod) {
	scale = Vector2i(scale.x * xmod, scale.y * ymod);
	Image* tempimage = new Image;
	tempimage->create(size_x*xmod, size_y*xmod);
	std::cout << tempimage->getSize().x;
	for (int i = 0; i < image->getSize().x; i++)
	{
		int loadedpix = 0;
		for (int j = 0; j < image->getSize().y; j++)
		{
			for (int m = 0; m < ymod; m++)
			{
				tempimage->setPixel(i + (xmod - 1) * i, j + m + (ymod - 1) * j, image->getPixel(i, j));

			}
		}
		for (int j = 0; j < image->getSize().y * ymod; j++)
		{
			for (int t = 0; t < xmod; t++)
			{
				tempimage->setPixel(i + i * (xmod - 1) + t, j, tempimage->getPixel(i + i * (xmod - 1), j));
			}
		}

	}
	delete texture, sprite, image;
	image = tempimage;
	texture = new Texture;
	texture->loadFromImage(*tempimage);
	sprite = new Sprite;
	sprite->setTexture(*texture);
	sprite->setOrigin(0, texture->getSize().y * 3 / 4);
	sprite->setPosition(texture->getSize().x / 2, texture->getSize().y);
	update();

}
void DetectedImage::draw() {
	if (is_bordered) ObjTar->draw(*borders);
	if(sprite!=nullptr) ObjTar->draw(*sprite);
}
void DetectedImage::setActive() {
	if (Click())
	{
		//Mark on image pixel which was clicked
		/*update();
		int mpx = mouse->getPosition(*pool_window[0].get()).x, mpy = mouse->getPosition(*pool_window[0].get()).y;
		image->setPixel(mpx - pos_x, mpy - pos_y, Color::Red);
		texture->loadFromImage(*image);
		sprite->setTexture(*texture);*/
		std::cout << "click\n";
	}
}
bool DetectedImage::Click() {
	RenderWindow* temp = Singleton::instance().getPoolWindow()[0].get();
	update();
	//std::cout <<"mouse position "<< mouse->getPosition().x << " " << mouse->getPosition().y << '\n';
	bool in_area = (Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right)) &&
		(mouse->getPosition(*temp).y >= pos_y) &&
		(mouse->getPosition(*temp).y < pos_y + size_y) &&
		(mouse->getPosition(*temp).x >= pos_x) &&
		(mouse->getPosition(*temp).x < pos_x + size_x);
	if (is_bordered) {
		clicked = true;
		return in_area;
	}
	if (in_area)
	{
		bool alpha_pixel = (bool)(image->getPixel(mouse->getPosition(*temp).x - pos_x,
			mouse->getPosition(*temp).y - pos_y).a);
		if (alpha_pixel) clicked = true;
		return alpha_pixel;
	}
	return in_area;
}
bool DetectedImage::Click(int difference_x, int difference_y)
{
	RenderWindow* temp = Singleton::instance().getPoolWindow()[0].get();
	update();
	//std::cout <<"mouse position "<< mouse->getPosition().x << " " << mouse->getPosition().y << '\n';
	bool in_area = Mouse::isButtonPressed(Mouse::Left) &&
		(mouse->getPosition(*temp).y >= pos_y + difference_y) &&
		(mouse->getPosition(*temp).y < pos_y + size_y + difference_y) &&
		(mouse->getPosition(*temp).x >= pos_x + difference_x) &&
		(mouse->getPosition(*temp).x < pos_x + size_x + difference_x);
	if (is_bordered) {
		clicked = true;
		return in_area;
	}
	if (in_area)
	{
		bool alpha_pixel = (bool)(image->getPixel(mouse->getPosition(*temp).x - pos_x - difference_x,
			mouse->getPosition(*temp).y - pos_y - difference_y).a);
		if (alpha_pixel) clicked = true;
		return alpha_pixel;
	}
	return in_area;
}
void DetectedImage::update()
{
	Vector2f temp = sprite->getOrigin();
	pos_x = sprite->getPosition().x - temp.x;
	pos_y = sprite->getPosition().y - temp.y;
	size_x = texture->getSize().x;
	size_y = texture->getSize().y;
}






void game() {
	Mouse mouse;
	/*Tile m("tyles/tile_022.png", &mouse);
	Tile k = m;*/
	Platform* pl = new Platform(&mouse);
	RectButtonImageRolled* img = new RectButtonImageRolled{ 1300,50,1,"images/settings.png",settings,Singleton::instance().getPoolWindow()[0].get(),&mouse};
	img->scale(0.5, 0.5);
	
	bool jammed = false;
	Jammed* jm = new Jammed{ Singleton::instance().getFPS(),0.1,[&pl]() {pl->leftClickedMap(); } };
	std::unique_ptr<Playable> player;
	player = std::make_unique<Playable>(Singleton::instance().getPoolWindow()[0].get());
	player->load("images/lords_avatars/blu_1.png");
	DetectedImage* dm = new DetectedImage{ "tyles/house/rem_0014.png",&mouse };
	dm->sprite->setScale(1 / 4.5234375, 1 / 5.16964286);
	dm->setPosition(200, 400);
	sf::Text texp;
	texp.setFont(*Singleton::instance().getGlobalFont());
	texp.setPosition(500, 500);
	texp.setCharacterSize(50);
	texp.setFillColor(Color::White);
	/*SelectedTile *k = new SelectedTile;
	Tile* m = new Tile("tyles/tile_000.png", &mouse);
	m->Scale(5, 5);
	m->setPosition(0, 400);
	k->loadCur(m);*/

	while (Singleton::instance().getPoolWindow()[0].get()->isOpen())
		{
			Event event;
			while (Singleton::instance().getPoolWindow()[0].get()->pollEvent(event)) {
				switch (event.type)
				{
				case Event::Closed: {
					for (int i = 0; i < Singleton::instance().getPoolWindow().size(); i++)
					{
						Singleton::instance().getPoolWindow()[0].get()->close();
					}
					break;
					//pool_button.clear();
					return;
				}
				case Event::KeyPressed: {
					switch (event.key.scancode) {
					case Keyboard::Scan::LAlt:
					{
						std::cout << "LALT\n";
						pl->textClear();
						break;
					}
					case Keyboard::Scan::Backspace:
						pl->deleteLast();
						cout << "dellast\n";
						break;
					case Keyboard::Scancode::Left:
					{
						player->move(Playable::Direction::left, 15);
						break;
					}
					case Keyboard::Scancode::Up:
					{
						player->move(Playable::Direction::up, 15);
						break;
					}
					case Keyboard::Scancode::Down:
					{
						player->move(Playable::Direction::down, 15);
						break;
					}
					case Keyboard::Scancode::Right:
					{
						player->move(Playable::Direction::right, 15);
						break;
					}
					}
					
					
					break;

				}
				case Event::MouseButtonReleased: {
					if (event.mouseButton.button == mouse.Left) {
						jm->disable();
					}
					break;
				}
				case Event::MouseButtonPressed: {
					if (mouse.isButtonPressed(mouse.Left))
					{
						jm->enable();				
						//m->setActive();
						pl->leftClicked();
						
						for (auto i : Singleton::instance().getPoolButton())
						{
							i.get()->setActive();
						}
						//std::cout << "frames = " << img->getFrames();
					}
					else if (mouse.isButtonPressed(mouse.Right) ){
						pl->rightClicked();
					}
					else if (mouse.isButtonPressed(mouse.Middle))
					{
						pl->wheelClicked();
					}
					
					break;
				}
				case Event::TextEntered:
					if (event.text.unicode != 8) {
						texp.setString(event.text.unicode);
						pl->input(event.text.unicode);
					}
					
					break;
				}
				//Sleep(5);

			}
			buttonWork();
			jm->exec();
			//std::cout << '\n';
			//std::cout << "frame " << pool_pair[0].get()->getFrame() << '\n';
			Singleton::instance().getPoolWindow()[0].get()->clear(*Singleton::instance().getBackgroundColor());
			std::cout << "";
			
			pl->draw();
			dm->draw();
			player->draw();
			globalDraw();
			Singleton::instance().getPoolWindow()[0].get()->draw(texp);
			//m->draw();
			Singleton::instance().getPoolWindow()[0].get()->display();
		}
	//if (img != nullptr) delete img;
}