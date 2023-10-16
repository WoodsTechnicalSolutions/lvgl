/**
 * @file lv_demo_benchmark.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_demo_benchmark.h"

#if LV_USE_DEMO_BENCHMARK
#include "../../src/display/lv_display_private.h"
#include "../../src/core/lv_global.h"

/**********************
 *      TYPEDEFS
 **********************/

#define SCENE_TIME  2000


typedef struct {
    const char * name;
    void (*create_cb)(void);
} scene_dsc_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void load_scene(uint32_t scene);
static void next_scene_timer_cb(lv_timer_t * timer);
static void sysmon_perf_observer_cb(lv_subject_t * subject, lv_observer_t * observer);
static int32_t rnd_next(int32_t min, int32_t max);

static void empty_screen_cb(void)
{
    lv_obj_t * obj = lv_obj_create(lv_screen_active());
    lv_obj_set_pos(obj, rnd_next(0, 300), rnd_next(0, 300));
}

static void moving_wallpaepr_cb(void)
{
    lv_obj_t * obj = lv_obj_create(lv_screen_active());
    lv_obj_set_pos(obj, rnd_next(0, 300), rnd_next(0, 300));
}

static void single_circle_cb(void)
{
    lv_obj_t * obj = lv_obj_create(lv_screen_active());
    lv_obj_set_pos(obj, rnd_next(0, 300), rnd_next(0, 300));
}

static void multiple_rectangles_cb(void)
{
    lv_obj_t * obj = lv_obj_create(lv_screen_active());
    lv_obj_set_pos(obj, rnd_next(0, 300), rnd_next(0, 300));
}

static void multiple_rgb_images_cb(void)
{
    lv_obj_t * obj = lv_obj_create(lv_screen_active());
    lv_obj_set_pos(obj, rnd_next(0, 300), rnd_next(0, 300));
}

static void multiple_argb_images_cb(void)
{
    lv_obj_t * obj = lv_obj_create(lv_screen_active());
    lv_obj_set_pos(obj, rnd_next(0, 300), rnd_next(0, 300));
}

static void rotated_argb_image_cb(void)
{
    lv_obj_t * obj = lv_obj_create(lv_screen_active());
    lv_obj_set_pos(obj, rnd_next(0, 300), rnd_next(0, 300));
}

static void multiple_labels_cb(void)
{
    lv_obj_t * obj = lv_obj_create(lv_screen_active());
    lv_obj_set_pos(obj, rnd_next(0, 300), rnd_next(0, 300));
}

static void screen_sized_text_cb(void)
{
    lv_obj_t * obj = lv_obj_create(lv_screen_active());
    lv_obj_set_pos(obj, rnd_next(0, 300), rnd_next(0, 300));
}

static void multiple_arcs_cb(void)
{
    lv_obj_t * obj = lv_obj_create(lv_screen_active());
    lv_obj_set_pos(obj, rnd_next(0, 300), rnd_next(0, 300));
}


static void containers_cb(void)
{
    lv_obj_t * obj = lv_obj_create(lv_screen_active());
    lv_obj_set_pos(obj, rnd_next(0, 300), rnd_next(0, 300));
}

static void containers_with_overlay_cb(void)
{
    lv_obj_t * obj = lv_obj_create(lv_screen_active());
    lv_obj_set_pos(obj, rnd_next(0, 300), rnd_next(0, 300));
}

static void containers_with_opa_cb(void)
{
    lv_obj_t * obj = lv_obj_create(lv_screen_active());
    lv_obj_set_pos(obj, rnd_next(0, 300), rnd_next(0, 300));
}

static void containers_with_opa_layer_cb(void)
{
    lv_obj_t * obj = lv_obj_create(lv_screen_active());
    lv_obj_set_pos(obj, rnd_next(0, 300), rnd_next(0, 300));
}

static void containers_with_scrolling_cb(void)
{
    lv_obj_t * obj = lv_obj_create(lv_screen_active());
    lv_obj_set_pos(obj, rnd_next(0, 300), rnd_next(0, 300));
}

/**********************
 *  STATIC VARIABLES
 **********************/

static scene_dsc_t scenes[] = {
    {.name = "Empty screen",               .create_cb = empty_screen_cb},
    {.name = "Moving wallpaper",           .create_cb = moving_wallpaepr_cb},
    {.name = "Single rectangle",           .create_cb = single_circle_cb},
    {.name = "Multiple rectangles",        .create_cb = multiple_rectangles_cb},
    {.name = "Multiple RGB images",        .create_cb = multiple_rgb_images_cb},
    {.name = "Multiple ARGB images",       .create_cb = multiple_argb_images_cb},
    {.name = "Rotated ARGB images",        .create_cb = rotated_argb_image_cb},
    {.name = "Multiple labels",            .create_cb = multiple_labels_cb},
    {.name = "Screen sized text",          .create_cb = screen_sized_text_cb},
    {.name = "Multiple arcs",              .create_cb = multiple_arcs_cb},

    {.name = "Containers",                 .create_cb = containers_cb},
    {.name = "Containers with overlay",    .create_cb = containers_with_overlay_cb},
    {.name = "Containers with opa",        .create_cb = containers_with_opa_cb},
    {.name = "Containers with opa_layer",  .create_cb = containers_with_opa_layer_cb},
    {.name = "Containers with scrolling",  .create_cb = containers_with_scrolling_cb},

    {.name = "", .create_cb = NULL}
};

#define sysmon_perf LV_GLOBAL_DEFAULT()->sysmon_perf

static uint32_t scene_act;
static uint32_t rnd_act;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_demo_benchmark(void)
{
    scene_act = 0;

    lv_obj_t * scr = lv_screen_active();
    lv_obj_remove_style_all(scr);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(scr, lv_palette_lighten(LV_PALETTE_GREY, 4), 0);

    lv_obj_t * title = lv_label_create(lv_layer_top());
    lv_obj_set_style_bg_opa(title, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(title, lv_color_white(), 0);
    lv_obj_set_style_text_color(title, lv_color_black(), 0);
    lv_obj_set_width(title, lv_pct(100));
    lv_label_set_text(title, "-");

    load_scene(scene_act);

    lv_timer_create(next_scene_timer_cb, SCENE_TIME, NULL);

    lv_subject_add_observer_obj(&sysmon_perf.subject, sysmon_perf_observer_cb, title, NULL);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


static void load_scene(uint32_t scene)
{
    lv_obj_t * scr = lv_screen_active();
    lv_obj_clean(scr);

    if(scenes[scene].create_cb) scenes[scene].create_cb();
}



static void next_scene_timer_cb(lv_timer_t * timer)
{
    LV_UNUSED(timer);

    scene_act++;
    load_scene(scene_act);
}

static void sysmon_perf_observer_cb(lv_subject_t * subject, lv_observer_t * observer)
{
    const lv_sysmon_perf_info_t * info = lv_subject_get_pointer(subject);
    lv_obj_t * label = lv_observer_get_target(observer);

    lv_label_set_text_fmt(label,
                          "%" LV_PRIu32" FPS, %" LV_PRIu32 "%% CPU\n"
                          "refr. %" LV_PRIu32" ms = %" LV_PRIu32 "ms render + %" LV_PRIu32" ms flush",
                          info->calculated.fps, info->calculated.cpu,
                          info->calculated.render_avg_time + info->calculated.flush_avg_time,
                          info->calculated.render_avg_time, info->calculated.flush_avg_time);
}

/*----------------
 * SCENE HELPERS
 *----------------*/

static void shake_anim_y_cb(void * var, int32_t v)
{
    lv_obj_set_y(var, v);
}

static void shake_anim(lv_obj_t * obj)
{
    lv_obj_update_layout(obj);  /*To be sure width an height are updated*/

    uint32_t t = rnd_next(500, 3000);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_exec_cb(&a, shake_anim_y_cb);
    lv_anim_set_values(&a, 0, lv_display_get_vertical_resolution(NULL) / 10);
    lv_anim_set_time(&a, t);
    lv_anim_set_playback_time(&a, t);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);
}

static void rnd_reset(void)
{
    rnd_act = 0;
}

static int32_t rnd_next(int32_t min, int32_t max)
{
    static const uint32_t rnd_map[] = {
        0xbd13204f, 0x67d8167f, 0x20211c99, 0xb0a7cc05,
        0x06d5c703, 0xeafb01a7, 0xd0473b5c, 0xc999aaa2,
        0x86f9d5d9, 0x294bdb29, 0x12a3c207, 0x78914d14,
        0x10a30006, 0x6134c7db, 0x194443af, 0x142d1099,
        0x376292d5, 0x20f433c5, 0x074d2a59, 0x4e74c293,
        0x072a0810, 0xdd0f136d, 0x5cca6dbc, 0x623bfdd8,
        0xb645eb2f, 0xbe50894a, 0xc9b56717, 0xe0f912c8,
        0x4f6b5e24, 0xfe44b128, 0xe12d57a8, 0x9b15c9cc,
        0xab2ae1d3, 0xb4dc5074, 0x67d457c8, 0x8e46b00c,
        0xa29a1871, 0xcee40332, 0x80f93aa1, 0x85286096,
        0x09bd6b49, 0x95072088, 0x2093924b, 0x6a27328f,
        0xa796079b, 0xc3b488bc, 0xe29bcce0, 0x07048a4c,
        0x7d81bd99, 0x27aacb30, 0x44fc7a0e, 0xa2382241,
        0x8357a17d, 0x97e9c9cc, 0xad10ff52, 0x9923fc5c,
        0x8f2c840a, 0x20356ba2, 0x7997a677, 0x9a7f1800,
        0x35c7562b, 0xd901fe51, 0x8f4e053d, 0xa5b94923,
    };

    if(min == max) return min;

    if(min > max) {
        int32_t t = min;
        min = max;
        max = t;
    }

    int32_t d = max - min;
    int32_t r = (rnd_map[rnd_act] % d) + min;

    rnd_act++;
    if(rnd_act >= sizeof(rnd_map) / sizeof(rnd_map[0])) rnd_act = 0;

    return r;
}

#endif
