#ifndef BUTTON_H
#define BUTTON_H

class Button {
private:
	float x,y,width,height;
	bool selected;
	int index;
public:
	Button(float x_pos, float y_pos, float w, float h, int i);

	float get_x() const;

	float get_y() const;

	float get_width() const;

	float get_height() const;

	int get_index() const;

	bool is_selected() const;

	void set_selected(bool b);

	bool contains(float mouseX, float mouseY) const;
};

#endif