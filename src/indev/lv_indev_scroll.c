/**
 * @file lv_indev_scroll.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../core/lv_obj_scroll_private.h"
#include "../core/lv_obj_private.h"
#include "lv_indev.h"
#include "lv_indev_private.h"
#include "lv_indev_scroll.h"

/*********************
 *      DEFINES
 *********************/
#define ELASTIC_SLOWNESS_FACTOR 4   /*Scrolling on elastic parts are slower by this factor*/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void init_scroll_limits(lv_indev_t * indev);
static int32_t find_snap_point_x(const lv_obj_t * obj, int32_t min, int32_t max, int32_t ofs);
static int32_t find_snap_point_y(const lv_obj_t * obj, int32_t min, int32_t max, int32_t ofs);
static void scroll_limit_diff(lv_indev_t * indev, int32_t * diff_x, int32_t * diff_y);
static int32_t elastic_diff(lv_obj_t * scroll_obj, int32_t diff, int32_t scroll_start, int32_t scroll_end,
                            lv_dir_t dir);
static void has_more_snap_points(lv_obj_t * scroll_obj, lv_dir_t dir, bool * has_start_snap, bool * has_end_snap);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_indev_scroll_handler(lv_indev_t * indev)
{
    if(indev->pointer.vect.x == 0 && indev->pointer.vect.y == 0) {
        return;
    }

    lv_obj_t * scroll_obj = indev->pointer.scroll_obj;
    /*If there is no scroll object yet try to find one*/
    if(scroll_obj == NULL) {
        scroll_obj = lv_indev_find_scroll_obj(indev);
        if(scroll_obj == NULL) return;

        init_scroll_limits(indev);

        lv_obj_remove_state(indev->pointer.act_obj, LV_STATE_PRESSED);
        lv_obj_send_event(scroll_obj, LV_EVENT_SCROLL_BEGIN, NULL);
        if(indev->reset_query) return;
    }

    /*Set new position or scroll if the vector is not zero*/
    int16_t angle = 0;
    int16_t scale_x = 256;
    int16_t scale_y = 256;
    lv_obj_t * parent = scroll_obj;
    while(parent) {
        angle += lv_obj_get_style_transform_rotation(parent, LV_PART_MAIN);
        int32_t zoom_act_x = lv_obj_get_style_transform_scale_x_safe(parent, LV_PART_MAIN);
        int32_t zoom_act_y = lv_obj_get_style_transform_scale_y_safe(parent, LV_PART_MAIN);
        scale_x = (scale_x * zoom_act_x) >> 8;
        scale_y = (scale_y * zoom_act_y) >> 8;
        parent = lv_obj_get_parent(parent);
    }

    if(scale_x == 0) {
        scale_x = 1;
    }

    if(scale_y == 0) {
        scale_y = 1;
    }

    if(angle != 0 || scale_x != LV_SCALE_NONE || scale_y != LV_SCALE_NONE) {
        angle = -angle;
        scale_x = (256 * 256) / scale_x;
        scale_y = (256 * 256) / scale_y;
        lv_point_t pivot = { 0, 0 };
        lv_point_transform(&indev->pointer.vect, angle, scale_x, scale_y, &pivot, false);
    }

    int32_t diff_x = 0;
    int32_t diff_y = 0;
    if(indev->pointer.scroll_dir == LV_DIR_HOR) {
        int32_t sr = lv_obj_get_scroll_right(scroll_obj);
        int32_t sl = lv_obj_get_scroll_left(scroll_obj);
        diff_x = elastic_diff(scroll_obj, indev->pointer.vect.x, sl, sr, LV_DIR_HOR);
    }
    else {
        int32_t st = lv_obj_get_scroll_top(scroll_obj);
        int32_t sb = lv_obj_get_scroll_bottom(scroll_obj);
        diff_y = elastic_diff(scroll_obj, indev->pointer.vect.y, st, sb, LV_DIR_VER);
    }

    lv_dir_t scroll_dir = lv_obj_get_scroll_dir(scroll_obj);
    if((scroll_dir & LV_DIR_LEFT)   == 0 && diff_x > 0) diff_x = 0;
    if((scroll_dir & LV_DIR_RIGHT)  == 0 && diff_x < 0) diff_x = 0;
    if((scroll_dir & LV_DIR_TOP)    == 0 && diff_y > 0) diff_y = 0;
    if((scroll_dir & LV_DIR_BOTTOM) == 0 && diff_y < 0) diff_y = 0;

    /*Respect the scroll limit area*/
    scroll_limit_diff(indev, &diff_x, &diff_y);

    lv_obj_scroll_by_raw(scroll_obj, diff_x, diff_y);
    if(indev->reset_query) return;
    indev->pointer.scroll_sum.x += diff_x;
    indev->pointer.scroll_sum.y += diff_y;
}

void lv_indev_scroll_throw_handler(lv_indev_t * indev)
{
    lv_obj_t * scroll_obj = indev->pointer.scroll_obj;
    if(scroll_obj == NULL) return;
    if(indev->pointer.scroll_dir == LV_DIR_NONE) return;

    int32_t scroll_throw = indev->scroll_throw;

    if(lv_obj_has_flag(scroll_obj, LV_OBJ_FLAG_SCROLL_MOMENTUM) == false) {
        indev->pointer.scroll_throw_vect.y = 0;
        indev->pointer.scroll_throw_vect.x = 0;
    }

    lv_scroll_snap_t align_x = lv_obj_get_scroll_snap_x(scroll_obj);
    lv_scroll_snap_t align_y = lv_obj_get_scroll_snap_y(scroll_obj);

    if(indev->pointer.scroll_dir == LV_DIR_VER) {
        indev->pointer.scroll_throw_vect.x = 0;
        /*If no snapping "throw"*/
        if(align_y == LV_SCROLL_SNAP_NONE) {
            indev->pointer.scroll_throw_vect.y =
                indev->pointer.scroll_throw_vect.y * (100 - scroll_throw) / 100;

            int32_t sb = lv_obj_get_scroll_bottom(scroll_obj);
            int32_t st = lv_obj_get_scroll_top(scroll_obj);

            indev->pointer.scroll_throw_vect.y = elastic_diff(scroll_obj, indev->pointer.scroll_throw_vect.y, st, sb,
                                                              LV_DIR_VER);

            lv_obj_scroll_by_raw(scroll_obj, 0,  indev->pointer.scroll_throw_vect.y);
            if(indev->reset_query) return;
        }
        /*With snapping find the nearest snap point and scroll there*/
        else {
            int32_t diff_y = lv_indev_scroll_throw_predict(indev, LV_DIR_VER);
            indev->pointer.scroll_throw_vect.y = 0;
            scroll_limit_diff(indev, NULL, &diff_y);
            int32_t y = find_snap_point_y(scroll_obj, LV_COORD_MIN, LV_COORD_MAX, diff_y);
            lv_obj_scroll_by(scroll_obj, 0, diff_y + y, LV_ANIM_ON);
            if(indev->reset_query) return;
        }
    }
    else if(indev->pointer.scroll_dir == LV_DIR_HOR) {
        indev->pointer.scroll_throw_vect.y = 0;
        /*If no snapping "throw"*/
        if(align_x == LV_SCROLL_SNAP_NONE) {
            indev->pointer.scroll_throw_vect.x =
                indev->pointer.scroll_throw_vect.x * (100 - scroll_throw) / 100;

            int32_t sl = lv_obj_get_scroll_left(scroll_obj);
            int32_t sr = lv_obj_get_scroll_right(scroll_obj);

            indev->pointer.scroll_throw_vect.x = elastic_diff(scroll_obj, indev->pointer.scroll_throw_vect.x, sl, sr,
                                                              LV_DIR_HOR);

            lv_obj_scroll_by_raw(scroll_obj, indev->pointer.scroll_throw_vect.x, 0);
            if(indev->reset_query) return;
        }
        /*With snapping find the nearest snap point and scroll there*/
        else {
            int32_t diff_x = lv_indev_scroll_throw_predict(indev, LV_DIR_HOR);
            indev->pointer.scroll_throw_vect.x = 0;
            scroll_limit_diff(indev, &diff_x, NULL);
            int32_t x = find_snap_point_x(scroll_obj, LV_COORD_MIN, LV_COORD_MAX, diff_x);
            lv_obj_scroll_by(scroll_obj, x + diff_x, 0, LV_ANIM_ON);
            if(indev->reset_query) return;
        }
    }

    /*Check if the scroll has finished*/
    if(indev->pointer.scroll_throw_vect.x == 0 && indev->pointer.scroll_throw_vect.y == 0) {
        /*Revert if scrolled in*/
        /*If vertically scrollable and not controlled by snap*/
        if(align_y == LV_SCROLL_SNAP_NONE) {
            int32_t st = lv_obj_get_scroll_top(scroll_obj);
            int32_t sb = lv_obj_get_scroll_bottom(scroll_obj);
            if(st > 0 || sb > 0) {
                if(st < 0) {
                    lv_obj_scroll_by(scroll_obj, 0, st, LV_ANIM_ON);
                    if(indev->reset_query) return;
                }
                else if(sb < 0) {
                    lv_obj_scroll_by(scroll_obj, 0, -sb, LV_ANIM_ON);
                    if(indev->reset_query) return;
                }
            }
        }

        /*If horizontally scrollable and not controlled by snap*/
        if(align_x == LV_SCROLL_SNAP_NONE) {
            int32_t sl = lv_obj_get_scroll_left(scroll_obj);
            int32_t sr = lv_obj_get_scroll_right(scroll_obj);
            if(sl > 0 || sr > 0) {
                if(sl < 0) {
                    lv_obj_scroll_by(scroll_obj, sl, 0, LV_ANIM_ON);
                    if(indev->reset_query) return;
                }
                else if(sr < 0) {
                    lv_obj_scroll_by(scroll_obj, -sr, 0, LV_ANIM_ON);
                    if(indev->reset_query) return;
                }
            }
        }

        lv_obj_send_event(scroll_obj, LV_EVENT_SCROLL_END, indev);
        if(indev->reset_query) return;

        indev->pointer.scroll_dir = LV_DIR_NONE;
        indev->pointer.scroll_obj = NULL;
    }
}

int32_t lv_indev_scroll_throw_predict(lv_indev_t * indev, lv_dir_t dir)
{
    if(indev == NULL) return 0;
    int32_t v;
    switch(dir) {
        case LV_DIR_VER:
            v = indev->pointer.scroll_throw_vect_ori.y;
            break;
        case LV_DIR_HOR:
            v = indev->pointer.scroll_throw_vect_ori.x;
            break;
        default:
            return 0;
    }

    int32_t scroll_throw = indev->scroll_throw;
    int32_t sum = 0;
    while(v) {
        sum += v;
        v = v * (100 - scroll_throw) / 100;
    }

    return sum;
}

void lv_indev_scroll_get_snap_dist(lv_obj_t * obj, lv_point_t * p)
{
    p->x = find_snap_point_x(obj, obj->coords.x1, obj->coords.x2, 0);
    p->y = find_snap_point_y(obj, obj->coords.y1, obj->coords.y2, 0);
}

lv_obj_t * lv_indev_find_scroll_obj(lv_indev_t * indev)
{
    lv_obj_t * obj_candidate = NULL;
    lv_dir_t dir_candidate = LV_DIR_NONE;
    int32_t scroll_limit = indev->scroll_limit;

    /*Go until find a scrollable object in the current direction
     *More precisely:
     * 1. Check the pressed object and all of its ancestors and try to find an object which is scrollable
     * 2. Scrollable means it has some content out of its area
     * 3. If an object can be scrolled into the current direction then use it ("real match"")
     * 4. If can be scrolled on the current axis (hor/ver) save it as candidate (at least show an elastic scroll effect)
     * 5. Use the last candidate. Always the "deepest" parent or the object from point 3*/
    lv_obj_t * obj_act = indev->pointer.act_obj;

    /*Decide if it's a horizontal or vertical scroll*/
    bool hor_en = false;
    bool ver_en = false;
    indev->pointer.scroll_sum.x += indev->pointer.vect.x;
    indev->pointer.scroll_sum.y += indev->pointer.vect.y;

    while(obj_act) {
        /*Get the transformed scroll_sum with this object*/
        int16_t angle = 0;
        int32_t scale_x = 256;
        int32_t scale_y = 256;
        lv_point_t pivot = { 0, 0 };
        lv_obj_t * parent = obj_act;
        while(parent) {
            angle += lv_obj_get_style_transform_rotation(parent, LV_PART_MAIN);
            int32_t zoom_act_x = lv_obj_get_style_transform_scale_x_safe(parent, LV_PART_MAIN);
            int32_t zoom_act_y = lv_obj_get_style_transform_scale_y_safe(parent, LV_PART_MAIN);
            scale_x = (scale_x * zoom_act_x) >> 8;
            scale_y = (scale_y * zoom_act_y) >> 8;
            parent = lv_obj_get_parent(parent);
        }

        if(scale_x == 0) {
            scale_x = 1;
        }

        if(scale_y == 0) {
            scale_y = 1;
        }

        lv_point_t obj_scroll_sum = indev->pointer.scroll_sum;
        if(angle != 0 || scale_x != LV_SCALE_NONE || scale_y != LV_SCALE_NONE) {
            angle = -angle;
            scale_x = (256 * 256) / scale_x;
            scale_y = (256 * 256) / scale_y;
            lv_point_transform(&obj_scroll_sum, angle, scale_x, scale_y, &pivot, false);
        }

        if(LV_ABS(obj_scroll_sum.x) > LV_ABS(obj_scroll_sum.y)) {
            hor_en = true;
        }
        else {
            ver_en = true;
        }

        if(lv_obj_has_flag(obj_act, LV_OBJ_FLAG_SCROLLABLE) == false) {
            /*If this object don't want to chain the scroll to the parent stop searching*/
            if(lv_obj_has_flag(obj_act, LV_OBJ_FLAG_SCROLL_CHAIN_HOR) == false && hor_en) break;
            if(lv_obj_has_flag(obj_act, LV_OBJ_FLAG_SCROLL_CHAIN_VER) == false && ver_en) break;

            obj_act = lv_obj_get_parent(obj_act);
            continue;
        }

        /*Consider both up-down or left/right scrollable according to the current direction*/
        bool up_en = ver_en;
        bool down_en = ver_en;
        bool left_en = hor_en;
        bool right_en = hor_en;

        /*The object might have disabled some directions.*/
        lv_dir_t scroll_dir = lv_obj_get_scroll_dir(obj_act);
        if((scroll_dir & LV_DIR_LEFT) == 0) left_en = false;
        if((scroll_dir & LV_DIR_RIGHT) == 0) right_en = false;
        if((scroll_dir & LV_DIR_TOP) == 0) up_en = false;
        if((scroll_dir & LV_DIR_BOTTOM) == 0) down_en = false;

        /*Horizontal scroll*/
        int32_t sl = 0;
        int32_t sr = 0;
        lv_scroll_snap_t snap_x = lv_obj_get_scroll_snap_x(obj_act);
        if(snap_x == LV_SCROLL_SNAP_NONE) {
            sl = lv_obj_get_scroll_left(obj_act);
            sr = lv_obj_get_scroll_right(obj_act);
        }
        else {
            bool has_start_snap;
            bool has_end_snap;
            has_more_snap_points(obj_act, LV_DIR_HOR, &has_start_snap, &has_end_snap);

            /*Assume scrolling is there are more snap point
             *Assumed scroll in if there are NO more nap points*/
            sl = has_start_snap ? 1 : -1;
            sr = has_end_snap ? 1 : -1;
        }

        /*Vertical scroll*/
        int32_t st = 0;
        int32_t sb = 0;
        lv_scroll_snap_t snap_y = lv_obj_get_scroll_snap_y(obj_act);
        if(snap_y == LV_SCROLL_SNAP_NONE) {
            st = lv_obj_get_scroll_top(obj_act);
            sb = lv_obj_get_scroll_bottom(obj_act);
        }
        else {
            bool has_start_snap;
            bool has_end_snap;
            has_more_snap_points(obj_act, LV_DIR_VER, &has_start_snap, &has_end_snap);

            /*Assume scrolling is there are more snap point
             *Assumed scroll in if there are NO more nap points*/
            st = has_start_snap ? 1 : -1;
            sb = has_end_snap ? 1 : -1;
        }

        /*If this object is scrollable into the current scroll direction then save it as a candidate.
         *It's important only to be scrollable on the current axis (hor/ver) because if the scroll
         *is propagated to this object it can show at least elastic scroll effect.
         *But if not hor/ver scrollable do not scroll it at all (so it's not a good candidate)*/
        if((st > 0 || sb > 0)  &&
           ((up_en    && obj_scroll_sum.y >=   scroll_limit) ||
            (down_en  && obj_scroll_sum.y <= - scroll_limit))) {
            obj_candidate = obj_act;
            dir_candidate = LV_DIR_VER;
        }

        if((sl > 0 || sr > 0)  &&
           ((left_en   && obj_scroll_sum.x >=   scroll_limit) ||
            (right_en  && obj_scroll_sum.x <= - scroll_limit))) {
            obj_candidate = obj_act;
            dir_candidate = LV_DIR_HOR;
        }

        if(st <= 0) up_en = false;
        if(sb <= 0) down_en = false;
        if(sl <= 0) left_en = false;
        if(sr <= 0) right_en = false;

        /*If the object really can be scrolled into the current direction then use it.*/
        if((left_en  && obj_scroll_sum.x >=   scroll_limit) ||
           (right_en && obj_scroll_sum.x <= - scroll_limit) ||
           (up_en    && obj_scroll_sum.y >=   scroll_limit) ||
           (down_en  && obj_scroll_sum.y <= - scroll_limit)) {
            indev->pointer.scroll_dir = hor_en ? LV_DIR_HOR : LV_DIR_VER;
            break;
        }

        /*If this object don't want to chain the scroll to the parent stop searching*/
        if(lv_obj_has_flag(obj_act, LV_OBJ_FLAG_SCROLL_CHAIN_HOR) == false && hor_en) break;
        if(lv_obj_has_flag(obj_act, LV_OBJ_FLAG_SCROLL_CHAIN_VER) == false && ver_en) break;

        /*Try the parent*/
        obj_act = lv_obj_get_parent(obj_act);
    }

    /*Use the last candidate*/
    if(obj_candidate) {
        indev->pointer.scroll_dir = dir_candidate;
        indev->pointer.scroll_obj = obj_candidate;
        indev->pointer.scroll_sum.x = 0;
        indev->pointer.scroll_sum.y = 0;
    }

    return obj_candidate;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void init_scroll_limits(lv_indev_t * indev)
{
    lv_obj_t * obj = indev->pointer.scroll_obj;
    /*If there no STOP allow scrolling anywhere*/
    if(lv_obj_has_flag(obj, LV_OBJ_FLAG_SCROLL_ONE) == false) {
        lv_area_set(&indev->pointer.scroll_area, LV_COORD_MIN, LV_COORD_MIN, LV_COORD_MAX, LV_COORD_MAX);
    }
    /*With STOP limit the scrolling to the perv and next snap point*/
    else {
        switch(lv_obj_get_scroll_snap_y(obj)) {
            case LV_SCROLL_SNAP_START:
                indev->pointer.scroll_area.y1 = find_snap_point_y(obj, obj->coords.y1 + 1, LV_COORD_MAX, 0);
                indev->pointer.scroll_area.y2 = find_snap_point_y(obj, LV_COORD_MIN, obj->coords.y1 - 1, 0);
                break;
            case LV_SCROLL_SNAP_END:
                indev->pointer.scroll_area.y1 = find_snap_point_y(obj, obj->coords.y2, LV_COORD_MAX, 0);
                indev->pointer.scroll_area.y2 = find_snap_point_y(obj, LV_COORD_MIN, obj->coords.y2, 0);
                break;
            case LV_SCROLL_SNAP_CENTER: {
                    int32_t y_mid = obj->coords.y1 + lv_area_get_height(&obj->coords) / 2;
                    indev->pointer.scroll_area.y1 = find_snap_point_y(obj, y_mid + 1, LV_COORD_MAX, 0);
                    indev->pointer.scroll_area.y2 = find_snap_point_y(obj, LV_COORD_MIN, y_mid - 1, 0);
                    break;
                }
            default:
                indev->pointer.scroll_area.y1 = LV_COORD_MIN;
                indev->pointer.scroll_area.y2 = LV_COORD_MAX;
                break;
        }

        switch(lv_obj_get_scroll_snap_x(obj)) {
            case LV_SCROLL_SNAP_START:
                indev->pointer.scroll_area.x1 = find_snap_point_x(obj, obj->coords.x1, LV_COORD_MAX, 0);
                indev->pointer.scroll_area.x2 = find_snap_point_x(obj, LV_COORD_MIN, obj->coords.x1, 0);
                break;
            case LV_SCROLL_SNAP_END:
                indev->pointer.scroll_area.x1 = find_snap_point_x(obj, obj->coords.x2, LV_COORD_MAX, 0);
                indev->pointer.scroll_area.x2 = find_snap_point_x(obj, LV_COORD_MIN, obj->coords.x2, 0);
                break;
            case LV_SCROLL_SNAP_CENTER: {
                    int32_t x_mid = obj->coords.x1 + lv_area_get_width(&obj->coords) / 2;
                    indev->pointer.scroll_area.x1 = find_snap_point_x(obj, x_mid + 1, LV_COORD_MAX, 0);
                    indev->pointer.scroll_area.x2 = find_snap_point_x(obj, LV_COORD_MIN, x_mid - 1, 0);
                    break;
                }
            default:
                indev->pointer.scroll_area.x1 = LV_COORD_MIN;
                indev->pointer.scroll_area.x2 = LV_COORD_MAX;
                break;
        }
    }

    /*`find_snap_point_x/y()` return LV_COORD_MAX is not snap point was found,
     *but x1/y1 should be small. */
    if(indev->pointer.scroll_area.x1 == LV_COORD_MAX) indev->pointer.scroll_area.x1 = LV_COORD_MIN;
    if(indev->pointer.scroll_area.y1 == LV_COORD_MAX) indev->pointer.scroll_area.y1 = LV_COORD_MIN;

    /*Allow scrolling on the edges. It will be reverted to the edge due to snapping anyway*/
    if(indev->pointer.scroll_area.x1 == 0) indev->pointer.scroll_area.x1 = LV_COORD_MIN;
    if(indev->pointer.scroll_area.x2 == 0) indev->pointer.scroll_area.x2 = LV_COORD_MAX;
    if(indev->pointer.scroll_area.y1 == 0) indev->pointer.scroll_area.y1 = LV_COORD_MIN;
    if(indev->pointer.scroll_area.y2 == 0) indev->pointer.scroll_area.y2 = LV_COORD_MAX;
}

/**
 * Search for snap point in the min..max range.
 * @param obj the object on which snap point should be found
 * @param min ignore snap points smaller than this. (Absolute coordinate)
 * @param max ignore snap points greater than this. (Absolute coordinate)
 * @param ofs offset to snap points. Useful the get a snap point in an imagined case
 *            what if children are already moved by this value
 * @return the absolute x coordinate of the nearest snap point
 *         or `LV_COORD_MAX` if there is no snap point in the min..max range
 */
static int32_t find_snap_point_x(const lv_obj_t * obj, int32_t min, int32_t max, int32_t ofs)
{
    lv_scroll_snap_t align = lv_obj_get_scroll_snap_x(obj);
    if(align == LV_SCROLL_SNAP_NONE) return LV_COORD_MAX;

    int32_t dist = LV_COORD_MAX;

    int32_t pad_left = lv_obj_get_style_pad_left(obj, LV_PART_MAIN);
    int32_t pad_right = lv_obj_get_style_pad_right(obj, LV_PART_MAIN);

    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_count(obj);
    for(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = obj->spec_attr->children[i];
        if(lv_obj_has_flag_any(child, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_FLOATING)) continue;
        if(lv_obj_has_flag(child, LV_OBJ_FLAG_SNAPPABLE)) {
            int32_t x_child = 0;
            int32_t x_parent = 0;
            switch(align) {
                case LV_SCROLL_SNAP_START:
                    x_child = child->coords.x1;
                    x_parent = obj->coords.x1 + pad_left;
                    break;
                case LV_SCROLL_SNAP_END:
                    x_child = child->coords.x2;
                    x_parent = obj->coords.x2 - pad_right;
                    break;
                case LV_SCROLL_SNAP_CENTER:
                    x_child = child->coords.x1 + lv_area_get_width(&child->coords) / 2;
                    x_parent = obj->coords.x1 + pad_left + (lv_area_get_width(&obj->coords) - pad_left - pad_right) / 2;
                    break;
                default:
                    continue;
            }

            x_child += ofs;
            if(x_child >= min && x_child <= max) {
                int32_t x = x_child -  x_parent;
                if(LV_ABS(x) < LV_ABS(dist)) dist = x;
            }
        }
    }

    return dist == LV_COORD_MAX ? LV_COORD_MAX : -dist;
}

/**
 * Search for snap point in the min..max range.
 * @param obj the object on which snap point should be found
 * @param min ignore snap points smaller than this. (Absolute coordinate)
 * @param max ignore snap points greater than this. (Absolute coordinate)
 * @param ofs offset to snap points. Useful to get a snap point in an imagined case
 *            what if children are already moved by this value
 * @return the absolute y coordinate of the nearest snap point
 *         or `LV_COORD_MAX` if there is no snap point in the min..max range
 */
static int32_t find_snap_point_y(const lv_obj_t * obj, int32_t min, int32_t max, int32_t ofs)
{
    lv_scroll_snap_t align = lv_obj_get_scroll_snap_y(obj);
    if(align == LV_SCROLL_SNAP_NONE) return LV_COORD_MAX;

    int32_t dist = LV_COORD_MAX;

    int32_t pad_top = lv_obj_get_style_pad_top(obj, LV_PART_MAIN);
    int32_t pad_bottom = lv_obj_get_style_pad_bottom(obj, LV_PART_MAIN);

    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_count(obj);
    for(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = obj->spec_attr->children[i];
        if(lv_obj_has_flag_any(child, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_FLOATING)) continue;
        if(lv_obj_has_flag(child, LV_OBJ_FLAG_SNAPPABLE)) {
            int32_t y_child = 0;
            int32_t y_parent = 0;
            switch(align) {
                case LV_SCROLL_SNAP_START:
                    y_child = child->coords.y1;
                    y_parent = obj->coords.y1 + pad_top;
                    break;
                case LV_SCROLL_SNAP_END:
                    y_child = child->coords.y2;
                    y_parent = obj->coords.y2 - pad_bottom;
                    break;
                case LV_SCROLL_SNAP_CENTER:
                    y_child = child->coords.y1 + lv_area_get_height(&child->coords) / 2;
                    y_parent = obj->coords.y1 + pad_top + (lv_area_get_height(&obj->coords) - pad_top - pad_bottom) / 2;
                    break;
                default:
                    continue;
            }

            y_child += ofs;
            if(y_child >= min && y_child <= max) {
                int32_t y = y_child -  y_parent;
                if(LV_ABS(y) < LV_ABS(dist)) dist = y;
            }
        }
    }

    return dist == LV_COORD_MAX ? LV_COORD_MAX : -dist;
}

static void scroll_limit_diff(lv_indev_t * indev, int32_t * diff_x, int32_t * diff_y)
{
    if(diff_y) {
        if(indev->pointer.scroll_sum.y + *diff_y < indev->pointer.scroll_area.y1) {
            *diff_y = indev->pointer.scroll_area.y1 - indev->pointer.scroll_sum.y;
        }

        if(indev->pointer.scroll_sum.y + *diff_y > indev->pointer.scroll_area.y2) {
            *diff_y = indev->pointer.scroll_area.y2 - indev->pointer.scroll_sum.y;
        }
    }

    if(diff_x) {
        if(indev->pointer.scroll_sum.x + *diff_x < indev->pointer.scroll_area.x1) {
            *diff_x = indev->pointer.scroll_area.x1 - indev->pointer.scroll_sum.x;
        }

        if(indev->pointer.scroll_sum.x + *diff_x > indev->pointer.scroll_area.x2) {
            *diff_x = indev->pointer.scroll_area.x2 - indev->pointer.scroll_sum.x;
        }
    }
}

static int32_t elastic_diff(lv_obj_t * scroll_obj, int32_t diff, int32_t scroll_start, int32_t scroll_end,
                            lv_dir_t dir)
{
    if(diff == 0) return 0;

    /*Scroll back to the edge if required*/
    if(!lv_obj_has_flag(scroll_obj, LV_OBJ_FLAG_SCROLL_ELASTIC)) {
        /*
         * If the scrolling object does not set the `LV_OBJ_FLAG_SCROLL_ELASTIC` flag,
         * make sure that `diff` will not cause the scroll to exceed the `start` or `end` boundary of the content.
         * If the content has exceeded the boundary due to external factors like `LV_SCROLL_SNAP_CENTER`,
         * then respect the current position instead of going straight back to 0.
         */
        const int32_t scroll_ended = diff > 0 ? scroll_start : scroll_end;
        if(scroll_ended < 0) diff = 0;
        else if(scroll_ended - diff < 0) diff = scroll_ended;
    }
    /*Handle elastic scrolling*/
    else {

        lv_scroll_snap_t snap;
        snap = dir == LV_DIR_HOR ? lv_obj_get_scroll_snap_x(scroll_obj) : lv_obj_get_scroll_snap_y(scroll_obj);

        /*Without snapping just scale down the diff when scrolled out*/
        if(snap == LV_SCROLL_SNAP_NONE) {
            if(scroll_end < 0 || scroll_start < 0) {
                /*Rounding*/
                if(diff < 0) diff -= ELASTIC_SLOWNESS_FACTOR / 2;
                if(diff > 0) diff += ELASTIC_SLOWNESS_FACTOR / 2;
                return diff / ELASTIC_SLOWNESS_FACTOR;
            }
            else {
                return diff;
            }
        }

        /*With snapping the widget is scrolled out if there are no more snap points
         *at least in one direction (start or end)*/
        bool has_start_snap;
        bool has_end_snap;
        has_more_snap_points(scroll_obj, dir, &has_start_snap, &has_end_snap);

        if(!has_start_snap || !has_end_snap) {
            /*Rounding*/
            if(diff < 0) diff -= ELASTIC_SLOWNESS_FACTOR / 2;
            if(diff > 0) diff += ELASTIC_SLOWNESS_FACTOR / 2;
            return diff / ELASTIC_SLOWNESS_FACTOR;
        }
        else {
            return diff;
        }
    }

    return diff;
}

/**
 * Tell is there are more snap point in a given direction considering snap position.
 * There is a snap point if there is a snapanble object in the given direction
 * @param scroll_obj        the object on which snap points should be found
 * @param dir               LV_DIR_HOR or LV_DIR_VER
 * @param has_start_snap    true: there is snap point in the start direction (top or left depending on dir)
 * @param has_end_snap      true: there is snap point in the end direction (bottom or right depending on dir)
 * @note snap points will be searched relative to the
 *       center point in case of LV_SCROLL_SNAP_CENTER
 *       start point (top or left) in case of LV_SCROLL_SNAP_START
 *       end point (bottom or right) in case of LV_SCROLL_SNAP_END
 */
static void has_more_snap_points(lv_obj_t * scroll_obj, lv_dir_t dir, bool * has_start_snap, bool * has_end_snap)
{
    *has_start_snap = true;
    *has_end_snap = true;
    lv_scroll_snap_t snap;
    snap = dir == LV_DIR_HOR ? lv_obj_get_scroll_snap_x(scroll_obj) : lv_obj_get_scroll_snap_y(scroll_obj);

    if(dir == LV_DIR_HOR) {
        int32_t x = 0;
        switch(snap) {
            case LV_SCROLL_SNAP_CENTER: {
                    int32_t pad_left = lv_obj_get_style_pad_left(scroll_obj, LV_PART_MAIN);
                    int32_t pad_right = lv_obj_get_style_pad_right(scroll_obj, LV_PART_MAIN);
                    x = scroll_obj->coords.x1;
                    x += (lv_area_get_width(&scroll_obj->coords) - pad_left - pad_right) / 2;
                    x += pad_left;
                }
                break;
            case LV_SCROLL_SNAP_START:
                x = scroll_obj->coords.x1 + lv_obj_get_style_pad_left(scroll_obj, LV_PART_MAIN);
                break;
            case LV_SCROLL_SNAP_END:
                x = scroll_obj->coords.x2 - lv_obj_get_style_pad_right(scroll_obj, LV_PART_MAIN);
                break;
            default:
                break;
        }
        int32_t d;
        d = find_snap_point_x(scroll_obj, x + 1, LV_COORD_MAX, 0);
        if(d == LV_COORD_MAX) *has_end_snap = false;
        d = find_snap_point_x(scroll_obj, LV_COORD_MIN, x - 1, 0);
        if(d == LV_COORD_MAX) *has_start_snap = false;
    }
    else {
        int32_t y = 0;
        switch(snap) {
            case LV_SCROLL_SNAP_CENTER: {
                    int32_t pad_top = lv_obj_get_style_pad_top(scroll_obj, LV_PART_MAIN);
                    int32_t pad_bottom = lv_obj_get_style_pad_bottom(scroll_obj, LV_PART_MAIN);
                    y = scroll_obj->coords.y1;
                    y += (lv_area_get_height(&scroll_obj->coords) - pad_top - pad_bottom) / 2;
                    y += pad_top;
                }
                break;
            case LV_SCROLL_SNAP_START:
                y = scroll_obj->coords.y1 + lv_obj_get_style_pad_top(scroll_obj, LV_PART_MAIN);
                break;
            case LV_SCROLL_SNAP_END:
                y = scroll_obj->coords.y2 - lv_obj_get_style_pad_bottom(scroll_obj, LV_PART_MAIN);
                break;
            default:
                break;
        }
        int32_t d;
        d = find_snap_point_y(scroll_obj, y + 1, LV_COORD_MAX, 0);
        if(d == LV_COORD_MAX) *has_end_snap = false;
        d = find_snap_point_y(scroll_obj, LV_COORD_MIN, y - 1, 0);
        if(d == LV_COORD_MAX) *has_start_snap = false;
    }
}
