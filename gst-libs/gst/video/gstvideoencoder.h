/* GStreamer
 * Copyright (C) 2008 David Schleef <ds@schleef.org>
 * Copyright (C) 2011 Mark Nauwelaerts <mark.nauwelaerts@collabora.co.uk>.
 * Copyright (C) 2011 Nokia Corporation. All rights reserved.
 *   Contact: Stefan Kost <stefan.kost@nokia.com>
 * Copyright (C) 2012 Collabora Ltd.
 *	Author : Edward Hervey <edward@collabora.com>
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

#ifndef _GST_VIDEO_ENCODER_H_
#define _GST_VIDEO_ENCODER_H_

#include <gst/video/gstvideoutils.h>

G_BEGIN_DECLS

#define GST_TYPE_VIDEO_ENCODER \
  (gst_video_encoder_get_type())
#define GST_VIDEO_ENCODER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_VIDEO_ENCODER,GstVideoEncoder))
#define GST_VIDEO_ENCODER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_VIDEO_ENCODER,GstVideoEncoderClass))
#define GST_VIDEO_ENCODER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS((obj),GST_TYPE_VIDEO_ENCODER,GstVideoEncoderClass))
#define GST_IS_VIDEO_ENCODER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_VIDEO_ENCODER))
#define GST_IS_VIDEO_ENCODER_CLASS(obj) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_VIDEO_ENCODER))
#define GST_VIDEO_ENCODER_CAST(enc) ((GstVideoEncoder*)enc)

/**
 * GST_VIDEO_ENCODER_SINK_NAME:
 *
 * The name of the templates for the sink pad.
 *
 * Since: 0.10.36
 */
#define GST_VIDEO_ENCODER_SINK_NAME    "sink"
/**
 * GST_VIDEO_ENCODER_SRC_NAME:
 *
 * The name of the templates for the source pad.
 *
 * Since: 0.10.36
 */
#define GST_VIDEO_ENCODER_SRC_NAME     "src"

/**
 * GST_VIDEO_ENCODER_FLOW_DROPPED:
 *
 * Returned when the event/buffer should be dropped.
 *
 * Since: 0.10.36
 */
#define GST_VIDEO_ENCODER_FLOW_DROPPED GST_FLOW_CUSTOM_SUCCESS_1

/**
 * GST_VIDEO_ENCODER_SRC_PAD:
 * @obj: a #GstVideoEncoder
 *
 * Gives the pointer to the source #GstPad object of the element.
 *
 * Since: 0.10.36
 */
#define GST_VIDEO_ENCODER_SRC_PAD(obj)         (((GstVideoEncoder *) (obj))->srcpad)

/**
 * GST_VIDEO_ENCODER_SINK_PAD:
 * @obj: a #GstVideoEncoder
 *
 * Gives the pointer to the sink #GstPad object of the element.
 *
 * Since: 0.10.36
 */
#define GST_VIDEO_ENCODER_SINK_PAD(obj)        (((GstVideoEncoder *) (obj))->sinkpad)

/**
 * GST_VIDEO_ENCODER_FLOW_NEED_DATA:
 *
 * Returned while parsing to indicate more data is needed.
 *
 * Since: 0.10.36
 **/
#define GST_VIDEO_ENCODER_FLOW_NEED_DATA GST_FLOW_CUSTOM_SUCCESS

/**
 * GST_VIDEO_ENCODER_FLOW_DROPPED:
 *
 * Returned when the event/buffer should be dropped.
 *
 * Since: 0.10.36
 */
#define GST_VIDEO_ENCODER_FLOW_DROPPED GST_FLOW_CUSTOM_SUCCESS_1

/**
 * GST_VIDEO_ENCODER_INPUT_SEGMENT:
 * @obj: base parse instance
 *
 * Gives the segment of the element.
 *
 * Since: 0.10.36
 */
#define GST_VIDEO_ENCODER_INPUT_SEGMENT(obj)     (GST_VIDEO_ENCODER_CAST (obj)->input_segment)

/**
 * GST_VIDEO_ENCODER_OUTPUT_SEGMENT:
 * @obj: base parse instance
 *
 * Gives the segment of the element.
 *
 * Since: 0.10.36
 */
#define GST_VIDEO_ENCODER_OUTPUT_SEGMENT(obj)     (GST_VIDEO_ENCODER_CAST (obj)->output_segment)

/**
 * GST_VIDEO_ENCODER_STREAM_LOCK:
 * @encoder: video encoder instance
 *
 * Obtain a lock to protect the encoder function from concurrent access.
 *
 * Since: 0.10.36
 */
#define GST_VIDEO_ENCODER_STREAM_LOCK(encoder) g_rec_mutex_lock (&GST_VIDEO_ENCODER (encoder)->stream_lock)

/**
 * GST_VIDEO_ENCODER_STREAM_UNLOCK:
 * @encoder: video encoder instance
 *
 * Release the lock that protects the encoder function from concurrent access.
 *
 * Since: 0.10.36
 */
#define GST_VIDEO_ENCODER_STREAM_UNLOCK(encoder) g_rec_mutex_unlock (&GST_VIDEO_ENCODER (encoder)->stream_lock)

typedef struct _GstVideoEncoder GstVideoEncoder;
typedef struct _GstVideoEncoderPrivate GstVideoEncoderPrivate;
typedef struct _GstVideoEncoderClass GstVideoEncoderClass;

/**
 * GstVideoEncoder:
 *
 * The opaque #GstVideoEncoder data structure.
 *
 * Since: 0.10.36
 */
struct _GstVideoEncoder
{
  /*< private >*/
  GstElement    element;

  /*< protected >*/
  GstPad         *sinkpad;
  GstPad         *srcpad;

  /* protects all data processing, i.e. is locked
   * in the chain function, finish_frame and when
   * processing serialized events */
  GRecMutex stream_lock;

  /* MT-protected (with STREAM_LOCK) */
  GstSegment      input_segment;
  GstSegment      output_segment;

  GstVideoEncoderPrivate *priv;
  /* FIXME before moving to base */
  void         *padding[GST_PADDING_LARGE];
};

/**
 * GstVideoEncoderClass:
 * @open:           Optional.
 *                  Called when the element changes to GST_STATE_READY.
 *                  Allows opening external resources. Since: 0.10.37.
 * @close:          Optional.
 *                  Called when the element changes to GST_STATE_NULL.
 *                  Allows closing external resources. Since: 0.10.37.
 * @start:          Optional.
 *                  Called when the element starts processing.
 *                  Allows opening external resources.
 * @stop:           Optional.
 *                  Called when the element stops processing.
 *                  Allows closing external resources.
 * @set_format:     Optional.
 *                  Notifies subclass of incoming data format.
 *                  GstVideoCodecState fields have already been
 *                  set according to provided caps.
 * @handle_frame:   Provides input frame to subclass.
 * @reset:          Optional.
 *                  Allows subclass (encoder) to perform post-seek semantics reset.
 * @finish:         Optional.
 *                  Called to request subclass to dispatch any pending remaining
 *                  data (e.g. at EOS).
 * @pre_push:   Optional.
 *                  Allows subclass to push frame downstream in whatever
 *                  shape or form it deems appropriate.  If not provided,
 *                  provided encoded frame data is simply pushed downstream.
 * @getcaps:        Optional.
 *                  Allows for a custom sink getcaps implementation (e.g.
 *                  for multichannel input specification).  If not implemented,
 *                  default returns gst_video_encoder_proxy_getcaps
 *                  applied to sink template caps.
 * @sink_event:     Optional.
 *                  Event handler on the sink pad. This function should return
 *                  TRUE if the event was handled and should be discarded
 *                  (i.e. not unref'ed).
 * @src_event:      Optional.
 *                  Event handler on the source pad. This function should return
 *                  TRUE if the event was handled and should be discarded
 *                  (i.e. not unref'ed).
 *
 * Subclasses can override any of the available virtual methods or not, as
 * needed. At minimum @handle_frame needs to be overridden, and @set_format
 * and @get_caps are likely needed as well.
 *
 * Since: 0.10.36
 */
struct _GstVideoEncoderClass
{
  /*< private >*/
  GstElementClass  element_class;

  /*< public >*/
  /* virtual methods for subclasses */
  gboolean      (*open)         (GstVideoEncoder *encoder);

  gboolean      (*close)        (GstVideoEncoder *encoder);

  gboolean      (*start)        (GstVideoEncoder *encoder);

  gboolean      (*stop)         (GstVideoEncoder *encoder);

  gboolean      (*set_format)   (GstVideoEncoder *encoder,
				 GstVideoCodecState *state);

  GstFlowReturn (*handle_frame) (GstVideoEncoder *encoder,
				 GstVideoCodecFrame *frame);

  gboolean      (*reset)        (GstVideoEncoder *encoder,
				 gboolean hard);

  GstFlowReturn (*finish)       (GstVideoEncoder *encoder);

  GstFlowReturn (*pre_push)     (GstVideoEncoder *encoder,
				 GstVideoCodecFrame *frame);

  GstCaps *     (*getcaps)      (GstVideoEncoder *enc,
                                 GstCaps *filter);

  gboolean      (*sink_event)   (GstVideoEncoder *encoder,
				 GstEvent *event);

  gboolean      (*src_event)    (GstVideoEncoder *encoder,
				 GstEvent *event);

  /*< private >*/
  /* FIXME before moving to base */
  gpointer       _gst_reserved[GST_PADDING_LARGE];
};

GType                gst_video_encoder_get_type (void);

GstVideoCodecState*  gst_video_encoder_get_output_state (GstVideoEncoder *encoder);

GstVideoCodecState*  gst_video_encoder_set_output_state (GstVideoEncoder * encoder,
							 GstCaps * caps,
							 GstVideoCodecState * reference);

GstVideoCodecFrame*  gst_video_encoder_get_frame        (GstVideoEncoder *encoder,
						         int frame_number);
GstVideoCodecFrame*  gst_video_encoder_get_oldest_frame (GstVideoEncoder *encoder);

GstFlowReturn        gst_video_encoder_finish_frame (GstVideoEncoder *encoder,
						     GstVideoCodecFrame *frame);

GstCaps *            gst_video_encoder_proxy_getcaps (GstVideoEncoder * enc,
						      GstCaps         * caps,
                                                      GstCaps         * filter);
void                 gst_video_encoder_set_discont (GstVideoEncoder *encoder);
gboolean             gst_video_encoder_get_discont (GstVideoEncoder *encoder);

void                 gst_video_encoder_set_latency (GstVideoEncoder *encoder,
						    GstClockTime min_latency,
						    GstClockTime max_latency);
void		     gst_video_encoder_get_latency (GstVideoEncoder *encoder,
						    GstClockTime *min_latency,
						    GstClockTime *max_latency);

void                 gst_video_encoder_set_headers (GstVideoEncoder *encoder,
						    GList *headers);
G_END_DECLS

#endif

