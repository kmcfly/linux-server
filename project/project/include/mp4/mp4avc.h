
#if !defined(_NS_VISION_MP4_AVC_H)
#define _NS_VISION_MP4_AVC_H
#include "mp4define.h"
///////////////////////////////////////////////////////////////////////////////
// 

/** H.264 网络抽象层单元 (NALU) 类型定义. */
enum H264NalType {
	H264_NAL_TYPE_NON_IDR_SLICE		= 1,
	H264_NAL_TYPE_DP_A_SLICE		= 2,
	H264_NAL_TYPE_DP_B_SLICE		= 3,
	H264_NAL_TYPE_DP_C_SLICE		= 0x4,
	H264_NAL_TYPE_IDR_SLICE			= 0x5,
	H264_NAL_TYPE_SEI				= 0x6,	// 
	H264_NAL_TYPE_SEQ_PARAM			= 0x7,	// 序列参数集
	H264_NAL_TYPE_PIC_PARAM			= 0x8,	// 图像参数集
	H264_NAL_TYPE_ACCESS_UNIT		= 0x9,
	H264_NAL_TYPE_END_OF_SEQ		= 0xa,
	H264_NAL_TYPE_END_OF_STREAM		= 0xb,
	H264_NAL_TYPE_FILLER_DATA		= 0xc,
	H264_NAL_TYPE_SEQ_EXTENSION		= 0xd
};

enum H264SliceType {
	H264_TYPE_P		= 0,
	H264_TYPE_B		= 1,
	H264_TYPE_I		= 2,
	H264_TYPE_SP	= 3,
	H264_TYPE_SI	= 4,
	H264_TYPE2_P	= 5,
	H264_TYPE2_B	= 6,
	H264_TYPE2_I	= 7,
	H264_TYPE2_SP	= 8,
	H264_TYPE2_SI	= 9
};

///////////////////////////////////////////////////////////////////////////////
// Bitstream class

/** 比特流解析工具类. */
class Bitstream 
{
public:
	typedef enum BitstreamErr_t {
		BITSTREAM_TOO_MANY_BITS, 
		BITSTREAM_PAST_END,
	} BitstreamErr_t;

public:
	Bitstream(void) {};
	Bitstream(const BYTE *buffer, UINT bit_len) { init(buffer, bit_len); }
	~Bitstream (void) {};

// Operations -------------------------------------------------
public:
	int bits_remain (void) { return fBufferSize + fBitsInBuffer; };
	void init(const BYTE *buffer, UINT bit_len);
	UINT GetBits(UINT bitsCount);;
	void SetBit(int bit);
	UINT PeekBits(UINT bits);
	void bookmark(int bSet);;
	
// Data Members -----------------------------------------------
private:
	UINT fBitsInBuffer;		///< 
	BYTE fBitsBuffer;		///< 
	UINT fBufferSize;		///< 缓存区中数据的比特数
	const BYTE *fBuffer;	///< 比特流缓存区
	
	int  fBookmarkOn;		///< 
	BYTE fBitsBufferMark;
	UINT fBitsInBufferMark;
	UINT fBufferSizeMark;
	const BYTE *fBufferMark;
};

/** H.264 条带头信息. */
typedef struct H264SliceHeader 
{	
	bool is_slice;				///< 当前单元是否是条带数据
	BYTE nal_unit_type;			///< 当前单元的类型
	UINT slice_type;			///< 当前条带的类型
	BYTE field_pic_flag;
	BYTE bottom_field_flag;
	UINT frame_num;				///< 帧序列
	UINT idr_pic_id;
	UINT pic_order_cnt_lsb;
	int  delta_pic_order_cnt_bottom;
	int  delta_pic_order_cnt[2];	
} H264SliceHeader;

/** H.264 序列参数集. */
typedef struct H264SeqParams
{
	BYTE profile;
	BYTE level;
	UINT chroma_format_idc;
	BYTE residual_colour_transform_flag;
	UINT bit_depth_luma_minus8;
	UINT bit_depth_chroma_minus8;
	BYTE qpprime_y_zero_transform_bypass_flag;
	BYTE seq_scaling_matrix_present_flag;
	UINT log2_max_frame_num_minus4;
	UINT log2_max_pic_order_cnt_lsb_minus4;
	UINT pic_order_cnt_type;
	BYTE pic_order_present_flag;
	BYTE delta_pic_order_always_zero_flag;
	int  offset_for_non_ref_pic;
	int  offset_for_top_to_bottom_field;
	UINT pic_order_cnt_cycle_length;
	short offset_for_ref_frame[256];
	UINT pic_width;				///< 视频的宽度
	UINT pic_height;			///< 视频的高度
	BYTE frame_mbs_only_flag;	///< 是否只包括了帧

} H264SeqParams;

///////////////////////////////////////////////////////////////////////////////
// H264HeaderParser class 

/** H.264 头结构解析器. */
class H264HeaderParser
{
public:
	H264HeaderParser();
	virtual ~H264HeaderParser();

// Operations -------------------------------------------------
public:
	LRESULT ParseHeader(const BYTE* nalu, int length);
	void Clear();

	bool IsSclice() { return fSliceHeader.is_slice; }
	int  GetFrameNum() { return fSliceHeader.frame_num; }
	
	static BYTE GetNaluType (const BYTE *buffer);
	static int  GetSliceType (const BYTE *buffer, UINT buflen, BYTE *slice_type, bool noheader);


// Data Members -----------------------------------------------
public:
	H264SliceHeader fSliceHeader;	///< 当前条带的头信息
	H264SeqParams   fSeqParams;		///< 当前序列参数集
};

#endif // !defined(_NS_VISION_MP4_AVC_H)
