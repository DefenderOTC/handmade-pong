struct {
	b32 is_down;
	b32 changed;
} typedef Button;

enum {
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_DOWN,
	BUTTON_UP,
	BUTTON_COUNT,
};

struct {
	int mouse_x, mouse_y;
	Button buttons[BUTTON_COUNT];
} typedef Input;


#define pressed(b)  (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)
#define is_down(b)  (input->buttons[b].is_down)

#define process_button(vk, b)	if(vk_code == vk){\
				input.buttons[b].changed = is_down !=\
				input.buttons[b].is_down;\
				input.buttons[b].is_down = is_down;\
				}