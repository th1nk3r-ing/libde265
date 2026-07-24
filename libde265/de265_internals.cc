/*
* H.265 video codec.
* Copyright (c) 2013-2014 struktur AG, Dirk Farin <farin@struktur.de>
*
* This file is part of libde265.
*
* libde265 is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as
* published by the Free Software Foundation, either version 3 of
* the License, or (at your option) any later version.
*
* libde265 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with libde265.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "de265_internals.h"
#include "image.h"
#include "decctx.h"
#include "refpic.h"

// Stage 2: prediction/residual/tr_coeff signal saving is now implemented.
// The decoder_context flags control buffer allocation in de265_image_get_buffer,
// and the decode flow (intrapred/motion/transform/slice) fills the buffers.
LIBDE265_API void de265_internals_set_parameter_bool(de265_decoder_context* de265ctx, enum de265_internals_param param, int value)
{
  decoder_context* ctx = (decoder_context*)de265ctx;

  switch (param)
  {
  case DE265_INTERNALS_DECODER_PARAM_SAVE_PREDICTION:
    ctx->param_internals_save_prediction = (value != 0);
    break;
  case DE265_INTERNALS_DECODER_PARAM_SAVE_RESIDUAL:
    ctx->param_internals_save_residual = (value != 0);
    break;
  case DE265_INTERNALS_DECODER_PARAM_SAVE_TR_COEFF:
    ctx->param_internals_save_tr_coeff = (value != 0);
    break;
  case DE265_INTERNALS_DECODER_PARAM_HEADER_ONLY_MODE:
    ctx->param_header_only = (value != 0);
    break;
  default:
    assert(false);
    break;
  }
}

LIBDE265_API const uint8_t* de265_internals_get_image_plane(const struct de265_image* img, de265_internals_param signal, int channel, int* out_stride)
{
  assert(channel>=0 && channel <= 2);

  uint8_t* data = nullptr;
  switch (signal)
  {
  case DE265_INTERNALS_DECODER_PARAM_SAVE_PREDICTION:
    data = img->pixels_confwin_prediction[channel];
    break;
  case DE265_INTERNALS_DECODER_PARAM_SAVE_RESIDUAL:
    data = img->pixels_confwin_residual[channel];
    break;
  case DE265_INTERNALS_DECODER_PARAM_SAVE_TR_COEFF:
    data = img->pixels_confwin_tr_coeff[channel];
    break;
  default:
    assert(false);
    break;
  }

  if (out_stride) *out_stride = img->get_image_stride(channel) * ((de265_get_bits_per_pixel(img, channel)+7) / 8);

  return data;
}

LIBDE265_API void de265_internals_get_CTB_Info_Layout(const struct de265_image *img, int *widthInUnits, int *heightInUnits, int *log2UnitSize)
{
  img->internals_get_CTB_Info_Layout(widthInUnits, heightInUnits, log2UnitSize);
}

LIBDE265_API void de265_internals_get_CTB_sliceIdx(const struct de265_image *img, uint16_t *idxArray)
{
  img->internals_get_sliceIdx(idxArray);
}

LIBDE265_API void de265_internals_get_CB_Info_Layout(const struct de265_image *img, int *widthInUnits, int *heightInUnits, int *log2UnitSize)
{
  img->internals_get_CB_Info_Layout(widthInUnits, heightInUnits, log2UnitSize);
}

LIBDE265_API void de265_internals_get_CB_info(const struct de265_image *img, uint16_t *idxArray)
{
  img->internals_get_CB_info(idxArray);
}

LIBDE265_API void de265_internals_get_PB_Info_layout(const struct de265_image *img, int *widthInUnits, int *heightInUnits, int *log2UnitSize)
{
  img->internals_get_PB_Info_layout(widthInUnits, heightInUnits, log2UnitSize);
}

LIBDE265_API void de265_internals_get_PB_info(const struct de265_image *img, int16_t *refPOC0, int16_t *refPOC1, int16_t *x0, int16_t *y0, int16_t *x1, int16_t *y1)
{
  img->internals_get_PB_info(refPOC0, refPOC1, x0, y0, x1, y1);
}

LIBDE265_API void de265_internals_get_IntraDir_Info_layout(const struct de265_image *img, int *widthInUnits, int *heightInUnits, int *log2UnitSize)
{
  img->internals_get_IntraDir_Info_Layer(widthInUnits, heightInUnits, log2UnitSize);
}

LIBDE265_API void de265_internals_get_intraDir_info(const struct de265_image *img, uint8_t *intraDir, uint8_t *intraDirChroma)
{
  img->internals_get_intraDir_info(intraDir, intraDirChroma);
}

LIBDE265_API void de265_internals_get_TUInfo_Info_layout(const struct de265_image *img, int *widthInUnits, int *heightInUnits, int *log2UnitSize)
{
  img->internals_get_TUInfo_Info_layout(widthInUnits, heightInUnits, log2UnitSize);
}

LIBDE265_API void de265_internals_get_TUInfo_info(const struct de265_image *img, uint8_t *tuInfo)
{
  img->internals_get_TUInfo_info(tuInfo);
}

LIBDE265_API void de265_internals_get_gop_info(const struct de265_image *img,
                                                int* out_poc,
                                                int* out_slice_type,
                                                int* out_nal_unit_type,
                                                int* out_num_ref_l0,
                                                int* out_num_ref_l1,
                                                int  ref_poc_l0[16],
                                                int  ref_poc_l1[16])
{
  if (out_poc)            *out_poc = -1;
  if (out_slice_type)     *out_slice_type = -1;
  if (out_nal_unit_type)  *out_nal_unit_type = -1;
  if (out_num_ref_l0)     *out_num_ref_l0 = 0;
  if (out_num_ref_l1)     *out_num_ref_l1 = 0;
  for (int i = 0; i < MAX_NUM_REF_PICS; i++) {
    if (ref_poc_l0) ref_poc_l0[i] = -1;
    if (ref_poc_l1) ref_poc_l1[i] = -1;
  }

  if (!img || img->slices.empty())
    return;

  if (out_poc)
    *out_poc = img->PicOrderCntVal;

  const slice_segment_header* shdr = img->slices[0];
  if (!shdr)
    return;

  if (out_slice_type)
    *out_slice_type = shdr->slice_type;

  if (out_nal_unit_type)
    *out_nal_unit_type = img->nal_hdr.nal_unit_type;

  int n0 = shdr->num_ref_idx_l0_active;
  // Per spec 8.3.4, RefPicList1 is only constructed for B slices. For P slices,
  // num_ref_idx_l1_active may be non-zero (inferred from
  // pps num_ref_idx_l1_default_active per 7.4.7.1 when override_flag==0), but
  // RefPicList_POC[1] is never populated by construct_reference_picture_lists.
  // Reading it would yield the zero-initialized clear() value, producing a
  // phantom L1 reference to POC 0. Gate L1 output on slice_type to avoid this.
  int n1 = (shdr->slice_type == SLICE_TYPE_B) ? shdr->num_ref_idx_l1_active : 0;

  if (out_num_ref_l0)
    *out_num_ref_l0 = n0;
  if (out_num_ref_l1)
    *out_num_ref_l1 = n1;

  for (int i = 0; i < n0 && i < MAX_NUM_REF_PICS; i++) {
    if (ref_poc_l0)
      ref_poc_l0[i] = shdr->RefPicList_POC[0][i];
  }
  for (int i = 0; i < n1 && i < MAX_NUM_REF_PICS; i++) {
    if (ref_poc_l1)
      ref_poc_l1[i] = shdr->RefPicList_POC[1][i];
  }
}

LIBDE265_API int64_t de265_internals_get_image_dts(const struct de265_image *img)
{
  if (!img)
    return -1;
  return img->dts;
}
