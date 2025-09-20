#pragma once

#include <libplacebo/gpu.h>
#include <libplacebo/renderer.h>
#include <libplacebo/log.h>
#include "video/mp_image.h"
#include "video/out/vo.h"

/* Forward declarations from mpv */
struct mpv_global;
struct mp_log;

/* Public RA handle exposed to higher layers (minimal surface). */
struct ra_next {
    pl_gpu gpu;
    struct mpv_global *global;
    struct mp_log *log;
};

/* Upload an mp_image into a pl_frame suitable for pl_render_image.
 * Caller must call ra_cleanup_pl_frame() to free any textures created.
 * Returns true on success, false on failure. */
bool ra_upload_mp_image(struct ra_next *ra, struct pl_frame *out_frame,
                        const struct mp_image *img);

/* Cleanup any textures/resources attached to a pl_frame created by upload. */
void ra_cleanup_pl_frame(struct ra_next *ra, struct pl_frame *frame);

/* Get the pl_renderer associated with this RA (may be NULL). */
pl_renderer ra_get_renderer(struct ra_next *ra);

/* Get the raw pl_gpu (for pl_tex_create / pl_tex_download etc). */
pl_gpu ra_get_gpu(struct ra_next *ra);

/* Flush libplacebo internal caches (wrapper for pl_renderer_flush_cache). */
void ra_reset(struct ra_next *ra);

/* Create the pl-specific RA implementation. */
struct ra_next *ra_pl_create(pl_gpu gpu, struct mp_log *log, pl_log log_pl);

/* Destroys the pl-specific RA implementation. */
void ra_pl_destroy(struct ra_next **rap);

/* Optional helper: let VO set a vo pointer on RA implementation. */
void ra_pl_set_vo(struct ra_next *ra, struct vo *vo);

/* Return the pl_log associated with the RA (or NULL). */
pl_log ra_get_pl_log(struct ra_next *ra);

/* Reset the RA (flush caches etc). */
void ra_pl_reset(struct ra_next *ra);

/* Internal helper: upload an mp_image to a pl_frame (used by ra_upload_mp_image).
 * This is exposed for use in pl_video_screenshot. */
bool upload_mp_image_to_pl_frame(struct ra_next *ra, struct pl_frame *out_frame, const struct mp_image *img);

/* Internal helper: cleanup a pl_frame (used by ra_cleanup_pl_frame).
 * This is exposed for use in pl_video_screenshot. */
void ra_pl_cleanup_frame(struct ra_next *ra, struct pl_frame *frame);

