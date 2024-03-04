#pragma once
#include "Libs.h"
using namespace sf;
class IBaseClass;
class Button;
class Clicable;
extern std::vector<std::shared_ptr<Button>> pool_button;
extern std::vector<std::shared_ptr<RenderWindow>> pool_window;
extern Font font_global;



class IBaseClass {
protected:
	int size_x, size_y,pos_x,pos_y;
	RenderTarget* ObjTar;
public:
	virtual void draw() = 0;
	IBaseClass() = default;
	virtual ~IBaseClass();
};


//����� �����������
//��������������� ����� ��� ������������ �������
class Clicable : public virtual IBaseClass
{
protected:
	Mouse* mouse;
public:
	virtual bool Click();
	virtual void setActive() = 0;
	virtual void isActive() = 0;
	Clicable()=default;
	virtual ~Clicable()=default;
};

//�������� ������ ��� ��������
class Button;

//������ ����� �������� ����� ������ ��� ����������� ���������� ������


//����� ������
class Button:public virtual Clicable,public virtual IBaseClass {
protected:
	Shape* but_shape;
	Color color;
	Text text;
	bool active = false;
	int frames[2];
	std::function<void()> run;
public:
	Button(int size_x, int size_y, int pos_x, int pos_y, int frames, char* text,std::function<void()>, RenderTarget* space, Mouse* mouse, Color color);
	void setActive() override;
	void standart();
	void draw() override;
	bool getActive();
	int getFrames();
	void isActive() override;
	~Button() override;
};
class CircleButton : public Button {
public:
	CircleButton(int size_x, int size_y, int pos_x, int pos_y, int frames, char* text, std::function<void()>, RenderTarget* space, Mouse* mouse, Color color);
	~CircleButton()=default;
};
class RectButton : public Button {;
public:
	RectButton(int size_x, int size_y, int pos_x, int pos_y, int frames, char* text, std::function<void()>, RenderTarget* space, Mouse* mouse, Color color);
	~RectButton()=default;
};
class CustomButton : public Button {
public:
	CustomButton(int size_x, int size_y, int pos_x, int pos_y, int frames, char* text, std::function<void()>, RenderTarget* space, Mouse* mouse, Color color);
	~CustomButton()=default;
};
void globalDraw();

void ButtonLoader(sf::RenderTarget* space, sf::Mouse* mouse, sf::Color color);