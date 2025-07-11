
/**
 * GENERATED FILE, DO NOT EDIT IT!
 * @file lv_style_properties.h
 */
#ifndef LV_STYLE_PROPERTIES_H
#define LV_STYLE_PROPERTIES_H

#include "../../core/lv_obj_property.h"
#if LV_USE_OBJ_PROPERTY


/* *INDENT-OFF* */
enum _lv_property_style_id_t {
    LV_PROPERTY_ID(STYLE, ALIGN,                    LV_PROPERTY_TYPE_INT,        LV_STYLE_ALIGN),
    LV_PROPERTY_ID(STYLE, ANIM,                     LV_PROPERTY_TYPE_INT,        LV_STYLE_ANIM),
    LV_PROPERTY_ID(STYLE, ANIM_DURATION,            LV_PROPERTY_TYPE_INT,        LV_STYLE_ANIM_DURATION),
    LV_PROPERTY_ID(STYLE, ARC_COLOR,                LV_PROPERTY_TYPE_INT,        LV_STYLE_ARC_COLOR),
    LV_PROPERTY_ID(STYLE, ARC_IMAGE_SRC,            LV_PROPERTY_TYPE_INT,        LV_STYLE_ARC_IMAGE_SRC),
    LV_PROPERTY_ID(STYLE, ARC_OPA,                  LV_PROPERTY_TYPE_INT,        LV_STYLE_ARC_OPA),
    LV_PROPERTY_ID(STYLE, ARC_ROUNDED,              LV_PROPERTY_TYPE_INT,        LV_STYLE_ARC_ROUNDED),
    LV_PROPERTY_ID(STYLE, ARC_WIDTH,                LV_PROPERTY_TYPE_INT,        LV_STYLE_ARC_WIDTH),
    LV_PROPERTY_ID(STYLE, BASE_DIR,                 LV_PROPERTY_TYPE_INT,        LV_STYLE_BASE_DIR),
    LV_PROPERTY_ID(STYLE, BG_COLOR,                 LV_PROPERTY_TYPE_COLOR,      LV_STYLE_BG_COLOR),
    LV_PROPERTY_ID(STYLE, BG_GRAD,                  LV_PROPERTY_TYPE_INT,        LV_STYLE_BG_GRAD),
    LV_PROPERTY_ID(STYLE, BG_GRAD_COLOR,            LV_PROPERTY_TYPE_COLOR,      LV_STYLE_BG_GRAD_COLOR),
    LV_PROPERTY_ID(STYLE, BG_GRAD_DIR,              LV_PROPERTY_TYPE_INT,        LV_STYLE_BG_GRAD_DIR),
    LV_PROPERTY_ID(STYLE, BG_GRAD_OPA,              LV_PROPERTY_TYPE_INT,        LV_STYLE_BG_GRAD_OPA),
    LV_PROPERTY_ID(STYLE, BG_GRAD_STOP,             LV_PROPERTY_TYPE_INT,        LV_STYLE_BG_GRAD_STOP),
    LV_PROPERTY_ID(STYLE, BG_IMAGE_OPA,             LV_PROPERTY_TYPE_INT,        LV_STYLE_BG_IMAGE_OPA),
    LV_PROPERTY_ID(STYLE, BG_IMAGE_RECOLOR,         LV_PROPERTY_TYPE_COLOR,      LV_STYLE_BG_IMAGE_RECOLOR),
    LV_PROPERTY_ID(STYLE, BG_IMAGE_RECOLOR_OPA,     LV_PROPERTY_TYPE_INT,        LV_STYLE_BG_IMAGE_RECOLOR_OPA),
    LV_PROPERTY_ID(STYLE, BG_IMAGE_SRC,             LV_PROPERTY_TYPE_IMGSRC,     LV_STYLE_BG_IMAGE_SRC),
    LV_PROPERTY_ID(STYLE, BG_IMAGE_TILED,           LV_PROPERTY_TYPE_INT,        LV_STYLE_BG_IMAGE_TILED),
    LV_PROPERTY_ID(STYLE, BG_MAIN_OPA,              LV_PROPERTY_TYPE_INT,        LV_STYLE_BG_MAIN_OPA),
    LV_PROPERTY_ID(STYLE, BG_MAIN_STOP,             LV_PROPERTY_TYPE_INT,        LV_STYLE_BG_MAIN_STOP),
    LV_PROPERTY_ID(STYLE, BG_OPA,                   LV_PROPERTY_TYPE_INT,        LV_STYLE_BG_OPA),
    LV_PROPERTY_ID(STYLE, BITMAP_MASK_SRC,          LV_PROPERTY_TYPE_INT,        LV_STYLE_BITMAP_MASK_SRC),
    LV_PROPERTY_ID(STYLE, BLEND_MODE,               LV_PROPERTY_TYPE_INT,        LV_STYLE_BLEND_MODE),
    LV_PROPERTY_ID(STYLE, BORDER_COLOR,             LV_PROPERTY_TYPE_COLOR,      LV_STYLE_BORDER_COLOR),
    LV_PROPERTY_ID(STYLE, BORDER_OPA,               LV_PROPERTY_TYPE_INT,        LV_STYLE_BORDER_OPA),
    LV_PROPERTY_ID(STYLE, BORDER_POST,              LV_PROPERTY_TYPE_INT,        LV_STYLE_BORDER_POST),
    LV_PROPERTY_ID(STYLE, BORDER_SIDE,              LV_PROPERTY_TYPE_INT,        LV_STYLE_BORDER_SIDE),
    LV_PROPERTY_ID(STYLE, BORDER_WIDTH,             LV_PROPERTY_TYPE_INT,        LV_STYLE_BORDER_WIDTH),
    LV_PROPERTY_ID(STYLE, CLIP_CORNER,              LV_PROPERTY_TYPE_INT,        LV_STYLE_CLIP_CORNER),
    LV_PROPERTY_ID(STYLE, COLOR_FILTER_DSC,         LV_PROPERTY_TYPE_INT,        LV_STYLE_COLOR_FILTER_DSC),
    LV_PROPERTY_ID(STYLE, COLOR_FILTER_OPA,         LV_PROPERTY_TYPE_INT,        LV_STYLE_COLOR_FILTER_OPA),
    LV_PROPERTY_ID(STYLE, FLEX_CROSS_PLACE,         LV_PROPERTY_TYPE_INT,        LV_STYLE_FLEX_CROSS_PLACE),
    LV_PROPERTY_ID(STYLE, FLEX_FLOW,                LV_PROPERTY_TYPE_INT,        LV_STYLE_FLEX_FLOW),
    LV_PROPERTY_ID(STYLE, FLEX_GROW,                LV_PROPERTY_TYPE_INT,        LV_STYLE_FLEX_GROW),
    LV_PROPERTY_ID(STYLE, FLEX_MAIN_PLACE,          LV_PROPERTY_TYPE_INT,        LV_STYLE_FLEX_MAIN_PLACE),
    LV_PROPERTY_ID(STYLE, FLEX_TRACK_PLACE,         LV_PROPERTY_TYPE_INT,        LV_STYLE_FLEX_TRACK_PLACE),
    LV_PROPERTY_ID(STYLE, GRID_CELL_COLUMN_POS,     LV_PROPERTY_TYPE_INT,        LV_STYLE_GRID_CELL_COLUMN_POS),
    LV_PROPERTY_ID(STYLE, GRID_CELL_COLUMN_SPAN,    LV_PROPERTY_TYPE_INT,        LV_STYLE_GRID_CELL_COLUMN_SPAN),
    LV_PROPERTY_ID(STYLE, GRID_CELL_ROW_POS,        LV_PROPERTY_TYPE_INT,        LV_STYLE_GRID_CELL_ROW_POS),
    LV_PROPERTY_ID(STYLE, GRID_CELL_ROW_SPAN,       LV_PROPERTY_TYPE_INT,        LV_STYLE_GRID_CELL_ROW_SPAN),
    LV_PROPERTY_ID(STYLE, GRID_CELL_X_ALIGN,        LV_PROPERTY_TYPE_INT,        LV_STYLE_GRID_CELL_X_ALIGN),
    LV_PROPERTY_ID(STYLE, GRID_CELL_Y_ALIGN,        LV_PROPERTY_TYPE_INT,        LV_STYLE_GRID_CELL_Y_ALIGN),
    LV_PROPERTY_ID(STYLE, GRID_COLUMN_ALIGN,        LV_PROPERTY_TYPE_INT,        LV_STYLE_GRID_COLUMN_ALIGN),
    LV_PROPERTY_ID(STYLE, GRID_COLUMN_DSC_ARRAY,    LV_PROPERTY_TYPE_INT,        LV_STYLE_GRID_COLUMN_DSC_ARRAY),
    LV_PROPERTY_ID(STYLE, GRID_ROW_ALIGN,           LV_PROPERTY_TYPE_INT,        LV_STYLE_GRID_ROW_ALIGN),
    LV_PROPERTY_ID(STYLE, GRID_ROW_DSC_ARRAY,       LV_PROPERTY_TYPE_INT,        LV_STYLE_GRID_ROW_DSC_ARRAY),
    LV_PROPERTY_ID(STYLE, HEIGHT,                   LV_PROPERTY_TYPE_INT,        LV_STYLE_HEIGHT),
    LV_PROPERTY_ID(STYLE, IMAGE_OPA,                LV_PROPERTY_TYPE_INT,        LV_STYLE_IMAGE_OPA),
    LV_PROPERTY_ID(STYLE, IMAGE_RECOLOR,            LV_PROPERTY_TYPE_COLOR,      LV_STYLE_IMAGE_RECOLOR),
    LV_PROPERTY_ID(STYLE, IMAGE_RECOLOR_OPA,        LV_PROPERTY_TYPE_INT,        LV_STYLE_IMAGE_RECOLOR_OPA),
    LV_PROPERTY_ID(STYLE, LAST_BUILT_IN_PROP,       LV_PROPERTY_TYPE_INT,        LV_STYLE_LAST_BUILT_IN_PROP),
    LV_PROPERTY_ID(STYLE, LAYOUT,                   LV_PROPERTY_TYPE_INT,        LV_STYLE_LAYOUT),
    LV_PROPERTY_ID(STYLE, LENGTH,                   LV_PROPERTY_TYPE_INT,        LV_STYLE_LENGTH),
    LV_PROPERTY_ID(STYLE, LINE_COLOR,               LV_PROPERTY_TYPE_COLOR,      LV_STYLE_LINE_COLOR),
    LV_PROPERTY_ID(STYLE, LINE_DASH_GAP,            LV_PROPERTY_TYPE_INT,        LV_STYLE_LINE_DASH_GAP),
    LV_PROPERTY_ID(STYLE, LINE_DASH_WIDTH,          LV_PROPERTY_TYPE_INT,        LV_STYLE_LINE_DASH_WIDTH),
    LV_PROPERTY_ID(STYLE, LINE_OPA,                 LV_PROPERTY_TYPE_INT,        LV_STYLE_LINE_OPA),
    LV_PROPERTY_ID(STYLE, LINE_ROUNDED,             LV_PROPERTY_TYPE_INT,        LV_STYLE_LINE_ROUNDED),
    LV_PROPERTY_ID(STYLE, LINE_WIDTH,               LV_PROPERTY_TYPE_INT,        LV_STYLE_LINE_WIDTH),
    LV_PROPERTY_ID(STYLE, MARGIN_BOTTOM,            LV_PROPERTY_TYPE_INT,        LV_STYLE_MARGIN_BOTTOM),
    LV_PROPERTY_ID(STYLE, MARGIN_LEFT,              LV_PROPERTY_TYPE_INT,        LV_STYLE_MARGIN_LEFT),
    LV_PROPERTY_ID(STYLE, MARGIN_RIGHT,             LV_PROPERTY_TYPE_INT,        LV_STYLE_MARGIN_RIGHT),
    LV_PROPERTY_ID(STYLE, MARGIN_TOP,               LV_PROPERTY_TYPE_INT,        LV_STYLE_MARGIN_TOP),
    LV_PROPERTY_ID(STYLE, MAX_HEIGHT,               LV_PROPERTY_TYPE_INT,        LV_STYLE_MAX_HEIGHT),
    LV_PROPERTY_ID(STYLE, MAX_WIDTH,                LV_PROPERTY_TYPE_INT,        LV_STYLE_MAX_WIDTH),
    LV_PROPERTY_ID(STYLE, MIN_HEIGHT,               LV_PROPERTY_TYPE_INT,        LV_STYLE_MIN_HEIGHT),
    LV_PROPERTY_ID(STYLE, MIN_WIDTH,                LV_PROPERTY_TYPE_INT,        LV_STYLE_MIN_WIDTH),
    LV_PROPERTY_ID(STYLE, OPA,                      LV_PROPERTY_TYPE_INT,        LV_STYLE_OPA),
    LV_PROPERTY_ID(STYLE, OPA_LAYERED,              LV_PROPERTY_TYPE_INT,        LV_STYLE_OPA_LAYERED),
    LV_PROPERTY_ID(STYLE, OUTLINE_COLOR,            LV_PROPERTY_TYPE_COLOR,      LV_STYLE_OUTLINE_COLOR),
    LV_PROPERTY_ID(STYLE, OUTLINE_OPA,              LV_PROPERTY_TYPE_INT,        LV_STYLE_OUTLINE_OPA),
    LV_PROPERTY_ID(STYLE, OUTLINE_PAD,              LV_PROPERTY_TYPE_INT,        LV_STYLE_OUTLINE_PAD),
    LV_PROPERTY_ID(STYLE, OUTLINE_WIDTH,            LV_PROPERTY_TYPE_INT,        LV_STYLE_OUTLINE_WIDTH),
    LV_PROPERTY_ID(STYLE, PAD_BOTTOM,               LV_PROPERTY_TYPE_INT,        LV_STYLE_PAD_BOTTOM),
    LV_PROPERTY_ID(STYLE, PAD_COLUMN,               LV_PROPERTY_TYPE_INT,        LV_STYLE_PAD_COLUMN),
    LV_PROPERTY_ID(STYLE, PAD_LEFT,                 LV_PROPERTY_TYPE_INT,        LV_STYLE_PAD_LEFT),
    LV_PROPERTY_ID(STYLE, PAD_RADIAL,               LV_PROPERTY_TYPE_INT,        LV_STYLE_PAD_RADIAL),
    LV_PROPERTY_ID(STYLE, PAD_RIGHT,                LV_PROPERTY_TYPE_INT,        LV_STYLE_PAD_RIGHT),
    LV_PROPERTY_ID(STYLE, PAD_ROW,                  LV_PROPERTY_TYPE_INT,        LV_STYLE_PAD_ROW),
    LV_PROPERTY_ID(STYLE, PAD_TOP,                  LV_PROPERTY_TYPE_INT,        LV_STYLE_PAD_TOP),
    LV_PROPERTY_ID(STYLE, PROP_INV,                 LV_PROPERTY_TYPE_INT,        LV_STYLE_PROP_INV),
    LV_PROPERTY_ID(STYLE, RADIAL_OFFSET,            LV_PROPERTY_TYPE_INT,        LV_STYLE_RADIAL_OFFSET),
    LV_PROPERTY_ID(STYLE, RADIUS,                   LV_PROPERTY_TYPE_INT,        LV_STYLE_RADIUS),
    LV_PROPERTY_ID(STYLE, RECOLOR,                  LV_PROPERTY_TYPE_INT,        LV_STYLE_RECOLOR),
    LV_PROPERTY_ID(STYLE, RECOLOR_OPA,              LV_PROPERTY_TYPE_INT,        LV_STYLE_RECOLOR_OPA),
    LV_PROPERTY_ID(STYLE, ROTARY_SENSITIVITY,       LV_PROPERTY_TYPE_INT,        LV_STYLE_ROTARY_SENSITIVITY),
    LV_PROPERTY_ID(STYLE, SHADOW_COLOR,             LV_PROPERTY_TYPE_COLOR,      LV_STYLE_SHADOW_COLOR),
    LV_PROPERTY_ID(STYLE, SHADOW_OFFSET_X,          LV_PROPERTY_TYPE_INT,        LV_STYLE_SHADOW_OFFSET_X),
    LV_PROPERTY_ID(STYLE, SHADOW_OFFSET_Y,          LV_PROPERTY_TYPE_INT,        LV_STYLE_SHADOW_OFFSET_Y),
    LV_PROPERTY_ID(STYLE, SHADOW_OPA,               LV_PROPERTY_TYPE_INT,        LV_STYLE_SHADOW_OPA),
    LV_PROPERTY_ID(STYLE, SHADOW_SPREAD,            LV_PROPERTY_TYPE_INT,        LV_STYLE_SHADOW_SPREAD),
    LV_PROPERTY_ID(STYLE, SHADOW_WIDTH,             LV_PROPERTY_TYPE_INT,        LV_STYLE_SHADOW_WIDTH),
    LV_PROPERTY_ID(STYLE, TEXT_ALIGN,               LV_PROPERTY_TYPE_INT,        LV_STYLE_TEXT_ALIGN),
    LV_PROPERTY_ID(STYLE, TEXT_COLOR,               LV_PROPERTY_TYPE_COLOR,      LV_STYLE_TEXT_COLOR),
    LV_PROPERTY_ID(STYLE, TEXT_DECOR,               LV_PROPERTY_TYPE_INT,        LV_STYLE_TEXT_DECOR),
    LV_PROPERTY_ID(STYLE, TEXT_FONT,                LV_PROPERTY_TYPE_FONT,       LV_STYLE_TEXT_FONT),
    LV_PROPERTY_ID(STYLE, TEXT_LETTER_SPACE,        LV_PROPERTY_TYPE_INT,        LV_STYLE_TEXT_LETTER_SPACE),
    LV_PROPERTY_ID(STYLE, TEXT_LINE_SPACE,          LV_PROPERTY_TYPE_INT,        LV_STYLE_TEXT_LINE_SPACE),
    LV_PROPERTY_ID(STYLE, TEXT_OPA,                 LV_PROPERTY_TYPE_INT,        LV_STYLE_TEXT_OPA),
    LV_PROPERTY_ID(STYLE, TEXT_OUTLINE_STROKE_COLOR, LV_PROPERTY_TYPE_INT,        LV_STYLE_TEXT_OUTLINE_STROKE_COLOR),
    LV_PROPERTY_ID(STYLE, TEXT_OUTLINE_STROKE_OPA,  LV_PROPERTY_TYPE_INT,        LV_STYLE_TEXT_OUTLINE_STROKE_OPA),
    LV_PROPERTY_ID(STYLE, TEXT_OUTLINE_STROKE_WIDTH, LV_PROPERTY_TYPE_INT,        LV_STYLE_TEXT_OUTLINE_STROKE_WIDTH),
    LV_PROPERTY_ID(STYLE, TRANSFORM_HEIGHT,         LV_PROPERTY_TYPE_INT,        LV_STYLE_TRANSFORM_HEIGHT),
    LV_PROPERTY_ID(STYLE, TRANSFORM_PIVOT_X,        LV_PROPERTY_TYPE_INT,        LV_STYLE_TRANSFORM_PIVOT_X),
    LV_PROPERTY_ID(STYLE, TRANSFORM_PIVOT_Y,        LV_PROPERTY_TYPE_INT,        LV_STYLE_TRANSFORM_PIVOT_Y),
    LV_PROPERTY_ID(STYLE, TRANSFORM_ROTATION,       LV_PROPERTY_TYPE_INT,        LV_STYLE_TRANSFORM_ROTATION),
    LV_PROPERTY_ID(STYLE, TRANSFORM_SCALE_X,        LV_PROPERTY_TYPE_INT,        LV_STYLE_TRANSFORM_SCALE_X),
    LV_PROPERTY_ID(STYLE, TRANSFORM_SCALE_Y,        LV_PROPERTY_TYPE_INT,        LV_STYLE_TRANSFORM_SCALE_Y),
    LV_PROPERTY_ID(STYLE, TRANSFORM_SKEW_X,         LV_PROPERTY_TYPE_INT,        LV_STYLE_TRANSFORM_SKEW_X),
    LV_PROPERTY_ID(STYLE, TRANSFORM_SKEW_Y,         LV_PROPERTY_TYPE_INT,        LV_STYLE_TRANSFORM_SKEW_Y),
    LV_PROPERTY_ID(STYLE, TRANSFORM_WIDTH,          LV_PROPERTY_TYPE_INT,        LV_STYLE_TRANSFORM_WIDTH),
    LV_PROPERTY_ID(STYLE, TRANSITION,               LV_PROPERTY_TYPE_INT,        LV_STYLE_TRANSITION),
    LV_PROPERTY_ID(STYLE, TRANSLATE_RADIAL,         LV_PROPERTY_TYPE_INT,        LV_STYLE_TRANSLATE_RADIAL),
    LV_PROPERTY_ID(STYLE, TRANSLATE_X,              LV_PROPERTY_TYPE_INT,        LV_STYLE_TRANSLATE_X),
    LV_PROPERTY_ID(STYLE, TRANSLATE_Y,              LV_PROPERTY_TYPE_INT,        LV_STYLE_TRANSLATE_Y),
    LV_PROPERTY_ID(STYLE, WIDTH,                    LV_PROPERTY_TYPE_INT,        LV_STYLE_WIDTH),
    LV_PROPERTY_ID(STYLE, X,                        LV_PROPERTY_TYPE_INT,        LV_STYLE_X),
    LV_PROPERTY_ID(STYLE, Y,                        LV_PROPERTY_TYPE_INT,        LV_STYLE_Y),
};

#endif
#endif
