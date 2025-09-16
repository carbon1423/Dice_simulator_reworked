#include "button.h"

Button::Button(float x_pos, float y_pos, float w, float h, int i){
	x = x_pos;
	y = y_pos;
	width = w;
	height = h;
	index = i;
}

float Button::get_x() const {
	return x;
}

float Button::get_y() const {
	return y;
}

float Button::get_width() const {
	return width;
}

float Button::get_height() const {
	return height;
}

int Button::get_index() const{
	return index;
}

bool Button::is_selected() const {
	return selected;
}

void Button::set_selected(bool b){
	selected = b;
}



bool Button::contains(float mouseX, float mouseY) const {
	if(mouseX > x && mouseX < x + width && mouseY > y && mouseY < y + height){
		return true;
	}
	return false;
}