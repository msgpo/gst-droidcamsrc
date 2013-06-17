/*
 * Copyright (C) 2013 Jolla LTD.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GST_DROID_CAM_SRC_H__
#define __GST_DROID_CAM_SRC_H__

#include <gst/gst.h>

#include "android/camera.h"
#include "gst/gstgralloc.h"
#include "gstcamerabufferpool.h"

G_BEGIN_DECLS

#define GST_TYPE_DROID_CAM_SRC \
  (gst_droid_cam_src_get_type())
#define GST_DROID_CAM_SRC(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_DROID_CAM_SRC,GstDroidCamSrc))
#define GST_DROID_CAM_SRC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_DROID_CAM_SRC,GstDroidCamSrcClass))
#define GST_IS_DROID_CAM_SRC(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_DROID_CAM_SRC))
#define GST_IS_DROID_CAM_SRC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_DROID_CAM_SRC))
#define GST_DROID_CAM_SRC_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), GST_TYPE_DROID_CAM_SRC, GstDroidCamSrcClass))

#define DEFAULT_VF_WIDTH         640
#define DEFAULT_VF_HEIGHT        480

#define DEFAULT_IMG_WIDTH        640
#define DEFAULT_IMG_HEIGHT       480

#define DEFAULT_FPS           30

typedef struct _GstDroidCamSrc GstDroidCamSrc;
typedef struct _GstDroidCamSrcClass GstDroidCamSrcClass;

struct _GstDroidCamSrc {
  GstBin parent;

  GstGralloc *gralloc;
  struct hw_module_t *hwmod;
  camera_module_t *cam;

  struct hw_device_t *cam_dev;
  camera_device_t *dev;

  struct camera_params *camera_params;

  GstCameraBufferPool *pool;

  gint camera_device;
  gint mode;

  GList *events;

  GstSegment segment;

  GstPad *vfsrc;
  GstPad *imgsrc;
  GstPad *vidsrc;

  gboolean send_new_segment;

  gboolean capturing;
  GMutex capturing_mutex;

  gboolean image_renegotiate;

  GQueue *img_queue;
  GCond img_cond;
  GMutex img_lock;
  gboolean img_task_running;
  /*
  GQueue *video_queue;
  GCond video_cond;
  GMutex video_lock;
  gboolean video_task_running;
  */
};

struct _GstDroidCamSrcClass {
  GstBinClass parent_class;

  gboolean (* set_camera_params) (GstDroidCamSrc *src);

  gboolean (* open_segment) (GstDroidCamSrc *src, GstPad * pad);
  void (* update_segment) (GstDroidCamSrc *src, GstBuffer * buffer);
};

GType gst_droid_cam_src_get_type (void);

G_END_DECLS

#endif /* __GST_DROID_CAM_SRC_H__ */
