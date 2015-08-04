
#if !defined(_NS_VISION_MP4_COMMON_H)
#define _NS_VISION_MP4_COMMON_H
#include "mp4define.h"

///////////////////////////////////////////////////////////////////////////////
// Mp4ErrorCode

/** MP4 模块错误返回码. */
enum Mp4ErrorCode
{
	MP4_S_OK				= 0,	///< 执行成功
	MP4_ERR_FAILED			= -1,	///< 一般性错误
	MP4_ERR_ALREADY_OPEN	= -10,	///< 文件已经打开
	MP4_ERR_OPEN			= -11,	///< 文件打开错误
	MP4_ERR_NOT_OPEN		= -12,	///< 文件还没有打开
	MP4_ERR_NULL_ATOM		= -13,	///< 碰到空的 atom
	MP4_ERR_NULL_PROPERTY	= -14,	///< 碰到空的属性
	MP4_ERR_WRITE			= -15,	///< 发生写错误
	MP4_ERR_READ			= -16,	///< 发生读错误
	MP4_ERR_NULL_FILE		= -17,	///< 碰到空文件指针
};

//______________________________________________________________________________
///////////////////////////////////////////////////////////////////////////////
// Mp4AudioType

#if 0
enum Mp4AudioType {
	MP4_INVALID_AUDIO_TYPE				= 0x00,
	MP4_MPEG1_AUDIO_TYPE				= 0x6B,
	MP4_MPEG2_AUDIO_TYPE				= 0x69,
	MP4_MPEG2_AAC_MAIN_AUDIO_TYPE		= 0x66,
	MP4_MPEG2_AAC_LC_AUDIO_TYPE			= 0x67,
	MP4_MPEG2_AAC_SSR_AUDIO_TYPE		= 0x68,
	MP4_MPEG4_AUDIO_TYPE				= 0x40,
	MP4_PRIVATE_AUDIO_TYPE				= 0xC0,
	MP4_PCM16_LITTLE_ENDIAN_AUDIO_TYPE	= 0xE0,	/* a private definition */
	MP4_VORBIS_AUDIO_TYPE				= 0xE1,	/* a private definition */
	MP4_AC3_AUDIO_TYPE					= 0xE2,	/* a private definition */
	MP4_ALAW_AUDIO_TYPE					= 0xE3,	/* a private definition */
	MP4_ULAW_AUDIO_TYPE					= 0xE4,	/* a private definition */
	MP4_G723_AUDIO_TYPE					= 0xE5, /* a private definition */
	MP4_PCM16_BIG_ENDIAN_AUDIO_TYPE		= 0xE6, /* a private definition */
};
#endif

//______________________________________________________________________________
///////////////////////////////////////////////////////////////////////////////
// Mp4PropertyType

/** MP4 Atom 属性类型. */
enum Mp4PropertyType
{
	IntegerProperty			= 1,
	FloatProperty			= 2,
	BitsProperty			= 3,
	BytesProperty			= 4,
	StringProperty			= 5,
	TableProperty			= 6,
	DescriptorProperty		= 7,
	IntegerArrayProperty	= 8,	///< 整数数组属性
	SizeTableProperty		= 9,	
};

//写mp4文件目标类型
#ifndef      ENUM_MP4_WRITE_DESTYPE_DEFINED
#define      ENUM_MP4_WRITE_DESTYPE_DEFINED
typedef enum _mp4_enum_write_destype_
{
	ENUM_MP4_DESTYPE_LOCAL,//写到本地
	ENUM_MP4_DESTYPE_FTP   //写到fty
}ENUM_MP4_WRITE_DESTYPE;
#endif
//______________________________________________________________________________
///////////////////////////////////////////////////////////////////////////////
// Mp4Array

/** 一个简单的数组模板类. */
template <class T> 
class Mp4Array
{
public:
	Mp4Array();
	virtual ~Mp4Array();

// Operations -------------------------------------------------
public:
	inline int GetCount() { return fCount; }	///< 返回元素的个数
	inline int MaxSize() { return fMaxCount; }	///< 

public:
	inline bool ValidIndex(const int index);
	void Resize();
	void Add(const T& item);
	void Clear();

	T& operator[](const int index) { 
		if (!ValidIndex(index)) {
			return fEmptyItem;
		}
		return fItems[index];
	} 

// Data Members -----------------------------------------------
protected:
	T  fEmptyItem;		///< 
	T* fItems;			///< 元素列表
	int fCount;			///< 有效元素数量
	int fMaxCount;		///< 最大元素数量
};

template <class T>
Mp4Array<T>::Mp4Array()
{
	fItems = NULL;
	fCount = 0;
	fMaxCount = 0;
}

template <class T>
Mp4Array<T>::~Mp4Array()
{
	if (fItems) {
		delete[] fItems;
		fItems = NULL;
	}
}

/** 检查指定的索引值是否有效. */
template <class T>
inline bool Mp4Array<T>::ValidIndex(const int index) {
	if (index < 0 || index >= fCount) {
		return false;
	}
	return true;
}

/** 重新分配数组存储空间. */
template <class T>
void Mp4Array<T>::Resize()
{
	fMaxCount = (fItems == NULL) ? 2 : fMaxCount * 2;
	T* items = new T[fMaxCount];
	if (fItems) {
		for (int i = 0; i < fCount; i++) {
			items[i] = fItems[i];
		}
		delete[] fItems;
	}
	
	fItems = items;
}

/** 添加一个元素. */
template <class T>
void Mp4Array<T>::Add(const T& item) 
{
	if (fMaxCount <= fCount) {
		Resize();
	}
	
	if (fItems && fMaxCount > fCount) {
		fItems[fCount++] = item;
	}
}

/** 清空这个数组. */
template <class T>
void Mp4Array<T>::Clear() 
{
	if (fItems) {
		delete[] fItems;
		fItems = NULL;
	}
	
	fCount = 0;
	fMaxCount = 0;
}

//______________________________________________________________________________
///////////////////////////////////////////////////////////////////////////////
// Mp4Property

class Mp4File;
class Mp4Property;
typedef SmartPtr<Mp4Property> Mp4PropertyPtr;
typedef Mp4Array<Mp4PropertyPtr> Mp4PropertyArray;

/** 代表一个 MP4 atom 的基本属性. */
class Mp4Property : public BaseObject
{
public:
	Mp4Property(Mp4PropertyType type, int size = 0, LPCSTR name = NULL);
	virtual ~Mp4Property();
	DECLARE_BASEREF_METHODS(Mp4Property);

// Attributes -------------------------------------------------
public:
	/** 返回这个属性的类型. */
	Mp4PropertyType GetType() { return fType; }
	
	/** 返回这个属性的名称. */
	LPCSTR GetName() const { return fName ? fName : ""; }

	/** 返回这个属性所占用的空间, 单位为字节. 返回 0 表示不确定. */
	virtual int GetSize();

// Operations -------------------------------------------------
public:
	virtual INT64 GetIntValue();
	void SetIntValue(INT64 value);

	float GetFloatValue();
	void SetFloatValue(float value);

	void SetBytesValue(const BYTE* bytes, UINT count);
	LPCSTR GetBytes() const;

	void SetStringValue(LPCSTR value);
	int GetStringValue(char* buf, UINT bufLen);

	virtual void SetExpectSize(int count);
	virtual LRESULT Read(Mp4File* file);
	virtual LRESULT Write(Mp4File* file);

// Data Members -----------------------------------------------
protected:
	Mp4PropertyType fType;	///< 属性的类型
	char* fName;			///< 属性名称
	int fSize;				///< 值的长度.
	int fExpectSize;		///< 期望的大小或长度

	INT64 fIntValue;		///< 整型值
	float fFloatValue;		///< 浮点型值
	char* fBytesValue;		///< 字符串/字节/其他类型值
};

//______________________________________________________________________________
///////////////////////////////////////////////////////////////////////////////
// Mp4DescriptorProperty

/** MP4 Descriptor 类型. 主要用于 "iods,esds" atom 节点 */
enum Mp4DescriptorType {
	Mp4ODescrTag				= 0x01, 
	Mp4IODescrTag				= 0x02, 
	Mp4ESDescrTag				= 0x03, 
	Mp4DecConfigDescrTag		= 0x04, 
	Mp4DecSpecificDescrTag		= 0x05, 
	Mp4SLConfigDescrTag		 	= 0x06, 
	Mp4ContentIdDescrTag		= 0x07, 
	Mp4SupplContentIdDescrTag 	= 0x08, 
	Mp4IPIPtrDescrTag		 	= 0x09, 
	Mp4IPMPPtrDescrTag		 	= 0x0A, 
	Mp4IPMPDescrTag			 	= 0x0B, 
	Mp4RegistrationDescrTag	 	= 0x0D, 
	Mp4ESIDIncDescrTag			= 0x0E, 
	Mp4ESIDRefDescrTag			= 0x0F, 
	Mp4FileIODescrTag			= 0x10, 
	Mp4FileODescrTag			= 0x11, 
	Mp4ExtProfileLevelDescrTag 	= 0x13, 
	Mp4ExtDescrTagsStart		= 0x80, 
	Mp4ExtDescrTagsEnd			= 0xFE, 
};

class Mp4Descriptor;
typedef SmartPtr<Mp4Descriptor> Mp4DescriptorPtr;
typedef Mp4Array<Mp4DescriptorPtr> Mp4DescriptorArray;

/** MP4 Descriptor. 主要用于 "iods,esds" atom 节点. */
class Mp4Descriptor : public BaseObject
{
public:
	Mp4Descriptor(Mp4DescriptorType type);
	DECLARE_BASEREF_METHODS(Mp4Descriptor);

// Operations -------------------------------------------------
public:
	Mp4PropertyPtr GetProperty(LPCSTR name);
	UINT GetPropertyValue(LPCSTR name);

	BYTE GetSize();
	void SetSize(BYTE size) { fSize = size; }
	
	Mp4DescriptorType GetType() { return fType; }

	LRESULT Read(Mp4File* file);
	LRESULT Write(Mp4File* file);

	void WriteMpegLength(UINT length, UINT& count);
	void WriteInt(UINT value, UINT size, UINT& count);

// Data Members -----------------------------------------------
protected:
	BYTE fSize;							///< 这个 Descriptor 的数据的长度
	BYTE fBuffer[MAX_PATH];				///< 这个 Descriptor 的数据缓存区
	
	Mp4DescriptorType  fType;			///< 这个 Descriptor 的类型
	Mp4PropertyArray   fProperties;		///< 这个 Descriptor 的属性列表
	Mp4DescriptorArray fDescriptors;	///< 子 Descriptor 列表.
};

//______________________________________________________________________________
///////////////////////////////////////////////////////////////////////////////
// Mp4DescriptorProperty

/** 代表一个 Descriptor 类型的属性. */
class Mp4DescriptorProperty : public Mp4Property
{
public:
	Mp4DescriptorProperty(LPCSTR name);

// Operations -------------------------------------------------
public:
	LRESULT Read(Mp4File* file);
	LRESULT Write(Mp4File* file);
	Mp4PropertyPtr GetProperty(LPCSTR name);

	void AddDescriptor(Mp4Descriptor* descriptor);
	Mp4DescriptorPtr GetDescriptor(int index);

	int GetDescriptorCount() { return fDescriptors.GetCount(); }
	INT64 GetIntValue() { return fDescriptors.GetCount(); }
	int GetSize();

// Data Members -----------------------------------------------
protected:
	Mp4DescriptorArray fDescriptors;	///< 子 Descriptor 列表
};

//______________________________________________________________________________
///////////////////////////////////////////////////////////////////////////////
// Mp4ArrayProperty

/** MP4 数组属性, 仅用于 MP4 表格属性, 代表表格的一列. */
class Mp4ArrayProperty : public Mp4Property
{
public:
	Mp4ArrayProperty(LPCSTR name);

// Operations -------------------------------------------------
public:
	/** 返回数组的长度. */
	int GetCount() { return fProperties.GetCount(); }

	/** 清空整个数组. */
	void Clear() { fProperties.Clear(); }

	UINT GetValue(int index);
	void SetValue(int index, UINT value);
	void AddValue(UINT value);

// Data Members -----------------------------------------------
protected:
	Mp4Array<UINT> fProperties;		///< 数组属性
};

typedef SmartPtr<Mp4ArrayProperty> Mp4ArrayPropertyPtr;

//______________________________________________________________________________
///////////////////////////////////////////////////////////////////////////////
// Mp4SizeTableProperty

/** MP4 字节数组表, 仅用于 AvcC atom 节点. */
class Mp4SizeTableProperty : public Mp4Property
{
public:
	Mp4SizeTableProperty(LPCSTR name);
	virtual ~Mp4SizeTableProperty();

// Operations -------------------------------------------------
public:
	/** 返回表格中元素的个数. */
	int GetCount() { return fSizeArray.GetCount(); }

	void AddEntry(const BYTE* bytes, WORD length);
	BYTE* GetEntry(int index, WORD& length);

	LRESULT Read(Mp4File* file);
	LRESULT Write(Mp4File* file);
	int GetSize();

// Data Members -----------------------------------------------
protected:
	Mp4Array<WORD>  fSizeArray;		///< 字节数组长度表
	Mp4Array<BYTE*> fBytesArray;	///< 字节数组表
};

typedef SmartPtr<Mp4SizeTableProperty> Mp4SizeTablePropertyPtr;

//______________________________________________________________________________
///////////////////////////////////////////////////////////////////////////////
// Mp4TableProperty

/** MP4 表格属性. */
class Mp4TableProperty : public Mp4Property
{
public:
	Mp4TableProperty(LPCSTR name = NULL);

// Operations -------------------------------------------------
public:
	/** 返回表格的列数. */
	int GetColCount() { return fProperties.GetCount(); }

	/** 返回表格的行数. */
	int GetRowCount();

	Mp4ArrayProperty* GetColumn(int index);
	void AddColumn(LPCSTR name);

	LRESULT Read(Mp4File* file);
	LRESULT Write(Mp4File* file);

	int GetSize();

// Data Members -----------------------------------------------
protected:
	Mp4PropertyArray fProperties;	///< 属性列表.
};

typedef SmartPtr<Mp4TableProperty> Mp4TablePropertyPtr;

//______________________________________________________________________________
///////////////////////////////////////////////////////////////////////////////
// Mp4File

/** 代表一个 MP4 文件, 这个类主要封装了 MP4 文件的 I/O 操作. */
class Mp4File : public BaseObject
{
public:
	Mp4File(ENUM_MP4_WRITE_DESTYPE desType = ENUM_MP4_DESTYPE_LOCAL);
	virtual ~Mp4File();
	DECLARE_BASEREF_METHODS(Mp4File);

// Operations -------------------------------------------------
public:
	virtual INT64 GetPosition(FILE* file = NULL);
	virtual void SetPosition(INT64 position, FILE* file = NULL);

	virtual LRESULT Open(LPCSTR name, LPCSTR mode);
	virtual void Close();

	virtual UINT ReadBytes(BYTE* bytes, UINT numBytes, FILE* file = NULL);
	virtual UINT WriteBytes(BYTE* bytes, UINT numBytes, FILE* file = NULL);

public:
	UINT PeekBytes(BYTE* bytes, UINT numBytes, FILE* file = NULL);
	INT64 GetFileSize() { return fFileSize; }

	INT64 ReadInt(UINT size);
	UINT WriteInt(INT64 value, UINT size);

	INT64 ReadBits(UINT size);
	UINT WriteBits(INT64 value, UINT size);
	void StartReadBits();
	UINT ReadMpegLength();

	size_t SafeWrite (const void *ptr, size_t size, size_t nmemb, FILE * stream);

// Data Members -----------------------------------------------
protected:
	INT64 fFileSize;	///< 文件长度
	FILE* fFile;		///< 文件句柄
	bool fIsWriting;	///< 是否正在写

	INT64 fBitsBuffer;	///< 比特数据缓存区
	int fBitsCount;		///< 缓存的比特数.
	ENUM_MP4_WRITE_DESTYPE m_desType;//是用ftp方式写还是写到本地
	unsigned long m_ftpFD;
};

typedef SmartPtr<Mp4File> Mp4FilePtr;

//______________________________________________________________________________
///////////////////////////////////////////////////////////////////////////////
// 其他工具方法

/** 把 4 个 ASCII 字符型式的 atom 类型字符串转换成 UINT 型式的 atom 类型. */
inline UINT ATOMID(const char* s) {
	if (s == NULL || *s == '\0') {
		return 0;
	}
	return (s[0] << 24) | (s[1] << 16) | (s[2] << 8) | s[3];
}

/** 返回 MP4 文件形式的当前时间. */
inline INT64 Mp4GetTimestamp() {
	INT64 ret = 0;
	//ret = time(NULL);
	ret += 2082844800;
	return ret;	// MP4 start date is 1/1/1904
	// 208284480 is (((1970 - 1904) * 365) + 17) * 24 * 60 * 60
}

/** 分配指定的大小的缓存区. */
inline void* Mp4Malloc(size_t size) {
	if (size == 0) {
		return NULL;
	}

	//void* p = malloc(size);
	
	//修改为使用new分配
	void* p = new unsigned char[size];

	return p;
}

/** 重新分配指定的大小的缓存区. */
//inline void* Mp4Realloc(void* p, size_t newSize) {
//	// workaround library bug
//	if (p == NULL && newSize == 0) {
//		return NULL;
//	}
//	p = realloc(p, newSize);
//	return p;
//}

/** 释放指定的缓存区. */
inline void Mp4Free(void* p) {
	if (p != NULL) {
		//free(p);
		//修改为new 分配
		unsigned char* ptr = (unsigned char*)(p);
		delete [] ptr;
	}
}

#endif // !defined(_NS_VISION_MP4_COMMON_H)
