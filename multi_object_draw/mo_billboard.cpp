#include "mo_billboard.hpp"

enum { WIN_W = 800, WIN_H = 600, FPS = 100 };

glm::mat4 MoBillboard::V = glm::mat4(1);
glm::mat4 MoBillboard::P = glm::ortho(
        -WIN_W / 2.0f, // left
        WIN_W / 2.0f, // right
        -WIN_H / 2.0f, // bottom
        WIN_H / 2.0f, // top
        -0.1f, // near
        100.0f // far
    ); 
BoalerViewUnit *MoBillboard::view_unit = NULL; 
BoalerModel *MoBillboard::model = NULL;

BoalerVSLink *MoBillboard::vs_link = NULL;

SDL_Surface *MoBillboard::bb_bg_img = NULL;
SDL_Surface *MoBillboard::bb_bg_img_active = NULL;
SDL_Surface *MoBillboard::bb_bg_img_rotate = NULL;

int MoBillboard::bb_w = 40;
int MoBillboard::bb_h = 50;

SDL_Rect MoBillboard::txt_rect = {
    .x = 0,
    .y = 12,
    .w = bb_w,
    .h = bb_h
};

void MoBillboard::prep(
    MoEng *eng,
    BoalerShaderUnit &shader_unit
) {
    view_unit = new BoalerViewUnit(V, P);
    eng->beng.reg_view_unit(view_unit);

    model = new BoalerModel("square.bin");

    vs_link = new BoalerVSLink(
            *view_unit,
            shader_unit);

    bb_bg_img = IMG_Load("resources/id_bb_bg.png");
    bb_bg_img_active = IMG_Load("resources/id_bb_bg_active.png");
    bb_bg_img_rotate = IMG_Load("resources/id_bb_bg_rotate.png");
}

MoBillboard::MoBillboard(
    const BoalerVSLModelUnitLink &mother_vslm_link,
    int shape_id,
    GLfloat r_bb) :
        shape_id(shape_id),
        text_texture(TextTexture(txt_rect, bb_bg_img)),
        model_unit(BoalerModelUnit(
            glm::scale(glm::mat4(1), glm::vec3(bb_w, bb_h, 1.0f)),
            3,
            text_texture.texture_id,
            *model)),
        vslm_link(BoalerVSLModelUnitLink(*vs_link, model_unit)),
        bb(Billboard(mother_vslm_link, vslm_link, r_bb))
{
    char tmp_string[150];
    sprintf(tmp_string, "<span color='white'>%d</span>", shape_id);
    text_texture.set_text(tmp_string);
}

void MoBillboard::set_to_active()
{
    text_texture.bg_img = bb_bg_img_active;
    char tmp_string[150];
    sprintf(tmp_string, "<span color='black'>%d</span>", shape_id);
    text_texture.set_text(tmp_string);
}

void MoBillboard::set_to_rotate()
{
    text_texture.bg_img = bb_bg_img_rotate;
    char tmp_string[150];
    sprintf(tmp_string, "<span color='black'>%d</span>", shape_id);
    text_texture.set_text(tmp_string);
}

void MoBillboard::set_to_normal()
{
    text_texture.bg_img = bb_bg_img;
    char tmp_string[150];
    sprintf(tmp_string, "<span color='white'>%d</span>", shape_id);
    text_texture.set_text(tmp_string);
}

