#include <stdio.h>
#include "H264Parser.h"
#include "math.h"
#include <assert.h>

int H264_Ue_Decode(bool *pBinaryArray, int &Des)
{
	if (pBinaryArray == NULL)
	{
		return -1;
	}

	int iter = 0;
	int NumOfZero = 0;
	int M_bit = 0;
	int INFO = 0;
	double result;

	//找到第一个1
	while(pBinaryArray[iter++] == 0)
	{
		NumOfZero++;
	}
	for (int i=NumOfZero-1; i>=0; i--)
	{
		INFO += pBinaryArray[iter++]<<i;
	}
	result = pow((double)2, (double)NumOfZero) + INFO - 1;
	Des = result;

	return iter;
}

int H264_Se_Decode(bool *pBinaryArray, int &Des)
{
	int ret = H264_Ue_Decode(pBinaryArray, Des);
	if ((Des & 0x1) == 0) 
	{
		Des >>= 1;
		Des = 0 - Des;
	}
	else
	{
		Des = (Des + 1) >> 1;
	}

	return ret;
}

int ParseSPS(unsigned char *pSrc, unsigned long SrcLen, unsigned short &VideoWidth, unsigned short &VideoHeight)
{
	int ret = 0;
	unsigned long offset = 0;
	bool *pBinaryArray = new bool[SrcLen*8];
	assert(pBinaryArray);
	unsigned long iter = 0;
	unsigned char mask = 0x80;
	for (int i=0; i<SrcLen; i++)
	{
		mask = 0x80;
		for (int j=0; j<8; j++)
		{
			if (pSrc[i] & (mask >> j))
			{
				pBinaryArray[iter] = 1;
			}
			else
			{
				pBinaryArray[iter] = 0;
			}
			iter++;
		}
	}
	iter = 0;

	unsigned char profile_idc = *pSrc;
	int seq_parameter_set_id;
	offset = 24;
	ret = H264_Ue_Decode(pBinaryArray+offset, seq_parameter_set_id);
	offset += ret;
	if (profile_idc == 100 || profile_idc == 110 || profile_idc == 122 || profile_idc == 144)
	{
		int chroma_format_idc;
		ret = H264_Ue_Decode(pBinaryArray+offset, chroma_format_idc);
		offset += ret;
		if(chroma_format_idc == 3)
		{
			offset += 1;
		}
		int bit_depth_luma_minus8, bit_depth_chroma_minus8;
		ret = H264_Ue_Decode(pBinaryArray+offset, bit_depth_luma_minus8);
		offset += ret;
		ret = H264_Ue_Decode(pBinaryArray+offset, bit_depth_chroma_minus8);
		offset += ret;
		offset += 1;
		bool seq_scaling_matrix_present_flag = false;
		seq_scaling_matrix_present_flag = pBinaryArray[offset];
		offset += 1;
		if (seq_scaling_matrix_present_flag)
		{
			unsigned long offset_tmp;
			for (int ix=0; ix<8; ix++)
			{
				if (pBinaryArray[offset++])
				{
					offset_tmp = Scaling_list(ix < 6 ? 16 : 64, pBinaryArray + offset);
					offset += offset_tmp;
				}
			}
		}
	}
	int log2_max_frame_num_minus4, pic_order_cnt_type;
	ret = H264_Ue_Decode(pBinaryArray+offset, log2_max_frame_num_minus4);
	offset += ret;
	ret = H264_Ue_Decode(pBinaryArray+offset, pic_order_cnt_type);
	offset += ret;
	if (pic_order_cnt_type == 0)
	{
		int log2_max_pic_order_cnt_lsb_minus4;
		ret = H264_Ue_Decode(pBinaryArray+offset, pic_order_cnt_type);
		offset += ret;
	}
	else if (pic_order_cnt_type == 1)
	{
		offset += 1;
		int offset_for_non_ref_pic, offset_for_top_to_bottom_field, num_ref_frames_in_pic_order_cnt_cycle, offset_for_ref_frame;
		ret = H264_Se_Decode(pBinaryArray+offset, offset_for_non_ref_pic);
		offset += ret;
		ret = H264_Se_Decode(pBinaryArray+offset, offset_for_top_to_bottom_field);
		offset += ret;
		ret = H264_Ue_Decode(pBinaryArray+offset, num_ref_frames_in_pic_order_cnt_cycle);
		offset += ret;
		for (int i=0; i<num_ref_frames_in_pic_order_cnt_cycle ;i++)
		{
			ret = H264_Se_Decode(pBinaryArray+offset, offset_for_ref_frame);
			offset += ret;
		}
	}
	int num_ref_frames;
	ret = H264_Ue_Decode(pBinaryArray+offset, num_ref_frames);
	offset += ret;
	offset += 1;
	int pic_width_in_mbs_minus1, pic_height_in_map_units_minus1, frame_mbs_only_flag;
	ret = H264_Ue_Decode(pBinaryArray+offset, pic_width_in_mbs_minus1);
	offset += ret;
	ret = H264_Ue_Decode(pBinaryArray+offset, pic_height_in_map_units_minus1);
	offset += ret;
	
	frame_mbs_only_flag = pBinaryArray[offset];
	offset += 1;
	
	if ( !frame_mbs_only_flag)
		offset += 1;
	offset += 1;

	int frame_cropping_flag = pBinaryArray[offset];
	offset += 1;

	int frame_crop_left_offset = 0;
	int frame_crop_right_offset = 0;
	int frame_crop_top_offset = 0;
	int frame_crop_buttom_offset = 0;
	
	if ( frame_cropping_flag)
	{
		ret = H264_Ue_Decode(pBinaryArray+offset, frame_crop_left_offset);
		offset += ret;
		ret = H264_Ue_Decode(pBinaryArray+offset, frame_crop_right_offset);
		offset += ret;
		ret = H264_Ue_Decode(pBinaryArray+offset, frame_crop_top_offset);
		offset += ret;
		ret = H264_Ue_Decode(pBinaryArray+offset, frame_crop_buttom_offset);
		offset += ret;
	}
	
	
	

	VideoWidth = (pic_width_in_mbs_minus1 + 1) * 16 - frame_crop_left_offset*2 - frame_crop_right_offset*2;;
	VideoHeight = (2 - frame_mbs_only_flag) * (pic_height_in_map_units_minus1 + 1) * 16  - (frame_crop_top_offset*2) - (frame_crop_buttom_offset*2);

	delete []pBinaryArray;
	pBinaryArray = NULL;
	return 0;
}

unsigned long Scaling_list (int sizeOfScalingList, bool *pBinaryArray)
{
	int lastScale = 8, nextScale = 8;
	int j, ret;
	unsigned long offset = 0;

	for (j = 0; j < sizeOfScalingList; j++)
	{
		if (nextScale != 0)
		{
			int deltaScale;
			ret = H264_Se_Decode(pBinaryArray + offset, deltaScale);
			offset += ret;
			nextScale = (lastScale + deltaScale + 256) % 256;
		}
		if (nextScale == 0)
		{
			lastScale = lastScale;
		} 
		else
		{
			lastScale = nextScale;
		}
	}
	return offset;
}
