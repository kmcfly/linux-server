
#if !defined(_NS_VISION_MP4_COMMON_H)
#define _NS_VISION_MP4_COMMON_H
#include "mp4define.h"

///////////////////////////////////////////////////////////////////////////////
// Mp4ErrorCode

/** MP4 ģ����󷵻���. */
enum Mp4ErrorCode
{
	MP4_S_OK				= 0,	///< ִ�гɹ�
	MP4_ERR_FAILED			= -1,	///< һ���Դ���
	MP4_ERR_ALREADY_OPEN	= -10,	///< �ļ��Ѿ���
	MP4_ERR_OPEN			= -11,	///< �ļ��򿪴���
	MP4_ERR_NOT_OPEN		= -12,	///< �ļ���û�д�
	MP4_ERR_NULL_ATOM		= -13,	///< �����յ� atom
	MP4_ERR_NULL_PROPERTY	= -14,	///< �����յ�����
	MP4_ERR_WRITE			= -15,	///< ����д����
	MP4_ERR_READ			= -16,	///< ����������
	MP4_ERR_NULL_FILE		= -17,	///< �������ļ�ָ��
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

/** MP4 Atom ��������. */
enum Mp4PropertyType
{
	IntegerProperty			= 1,
	FloatProperty			= 2,
	BitsProperty			= 3,
	BytesProperty			= 4,
	StringProperty			= 5,
	TableProperty			= 6,
	DescriptorProperty		= 7,
	IntegerArrayProperty	= 8,	///< ������������
	SizeTableProperty		= 9,	
};

//дmp4�ļ�Ŀ������
#ifndef      ENUM_MP4_WRITE_DESTYPE_DEFINED
#define      ENUM_MP4_WRITE_DESTYPE_DEFINED
typedef enum _mp4_enum_write_destype_
{
	ENUM_MP4_DESTYPE_LOCAL,//д������
	ENUM_MP4_DESTYPE_FTP   //д��fty
}ENUM_MP4_WRITE_DESTYPE;
#endif
//______________________________________________________________________________
///////////////////////////////////////////////////////////////////////////////
// Mp4Array

/** һ���򵥵�����ģ����. */
template <class T> 
class Mp4Array
{
public:
	Mp4Array();
	virtual ~Mp4Array();

// Operations -------------------------------------------------
public:
	inline int GetCount() { return fCount; }	///< ����Ԫ�صĸ���
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
	T* fItems;			///< Ԫ���б�
	int fCount;			///< ��ЧԪ������
	int fMaxCount;		///< ���Ԫ������
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

/** ���ָ��������ֵ�Ƿ���Ч. */
template <class T>
inline bool Mp4Array<T>::ValidIndex(const int index) {
	if (index < 0 || index >= fCount) {
		return false;
	}
	return true;
}

/** ���·�������洢�ռ�. */
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

/** ���һ��Ԫ��. */
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

/** ����������. */
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

/** ����һ�� MP4 atom �Ļ�������. */
class Mp4Property : public BaseObject
{
public:
	Mp4Property(Mp4PropertyType type, int size = 0, LPCSTR name = NULL);
	virtual ~Mp4Property();
	DECLARE_BASEREF_METHODS(Mp4Property);

// Attributes -------------------------------------------------
public:
	/** ����������Ե�����. */
	Mp4PropertyType GetType() { return fType; }
	
	/** ����������Ե�����. */
	LPCSTR GetName() const { return fName ? fName : ""; }

	/** �������������ռ�õĿռ�, ��λΪ�ֽ�. ���� 0 ��ʾ��ȷ��. */
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
	Mp4PropertyType fType;	///< ���Ե�����
	char* fName;			///< ��������
	int fSize;				///< ֵ�ĳ���.
	int fExpectSize;		///< �����Ĵ�С�򳤶�

	INT64 fIntValue;		///< ����ֵ
	float fFloatValue;		///< ������ֵ
	char* fBytesValue;		///< �ַ���/�ֽ�/��������ֵ
};

//______________________________________________________________________________
///////////////////////////////////////////////////////////////////////////////
// Mp4DescriptorProperty

/** MP4 Descriptor ����. ��Ҫ���� "iods,esds" atom �ڵ� */
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

/** MP4 Descriptor. ��Ҫ���� "iods,esds" atom �ڵ�. */
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
	BYTE fSize;							///< ��� Descriptor �����ݵĳ���
	BYTE fBuffer[MAX_PATH];				///< ��� Descriptor �����ݻ�����
	
	Mp4DescriptorType  fType;			///< ��� Descriptor ������
	Mp4PropertyArray   fProperties;		///< ��� Descriptor �������б�
	Mp4DescriptorArray fDescriptors;	///< �� Descriptor �б�.
};

//______________________________________________________________________________
///////////////////////////////////////////////////////////////////////////////
// Mp4DescriptorProperty

/** ����һ�� Descriptor ���͵�����. */
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
	Mp4DescriptorArray fDescriptors;	///< �� Descriptor �б�
};

//______________________________________________________________________________
///////////////////////////////////////////////////////////////////////////////
// Mp4ArrayProperty

/** MP4 ��������, ������ MP4 �������, �������һ��. */
class Mp4ArrayProperty : public Mp4Property
{
public:
	Mp4ArrayProperty(LPCSTR name);

// Operations -------------------------------------------------
public:
	/** ��������ĳ���. */
	int GetCount() { return fProperties.GetCount(); }

	/** �����������. */
	void Clear() { fProperties.Clear(); }

	UINT GetValue(int index);
	void SetValue(int index, UINT value);
	void AddValue(UINT value);

// Data Members -----------------------------------------------
protected:
	Mp4Array<UINT> fProperties;		///< ��������
};

typedef SmartPtr<Mp4ArrayProperty> Mp4ArrayPropertyPtr;

//______________________________________________________________________________
///////////////////////////////////////////////////////////////////////////////
// Mp4SizeTableProperty

/** MP4 �ֽ������, ������ AvcC atom �ڵ�. */
class Mp4SizeTableProperty : public Mp4Property
{
public:
	Mp4SizeTableProperty(LPCSTR name);
	virtual ~Mp4SizeTableProperty();

// Operations -------------------------------------------------
public:
	/** ���ر����Ԫ�صĸ���. */
	int GetCount() { return fSizeArray.GetCount(); }

	void AddEntry(const BYTE* bytes, WORD length);
	BYTE* GetEntry(int index, WORD& length);

	LRESULT Read(Mp4File* file);
	LRESULT Write(Mp4File* file);
	int GetSize();

// Data Members -----------------------------------------------
protected:
	Mp4Array<WORD>  fSizeArray;		///< �ֽ����鳤�ȱ�
	Mp4Array<BYTE*> fBytesArray;	///< �ֽ������
};

typedef SmartPtr<Mp4SizeTableProperty> Mp4SizeTablePropertyPtr;

//______________________________________________________________________________
///////////////////////////////////////////////////////////////////////////////
// Mp4TableProperty

/** MP4 �������. */
class Mp4TableProperty : public Mp4Property
{
public:
	Mp4TableProperty(LPCSTR name = NULL);

// Operations -------------------------------------------------
public:
	/** ���ر�������. */
	int GetColCount() { return fProperties.GetCount(); }

	/** ���ر�������. */
	int GetRowCount();

	Mp4ArrayProperty* GetColumn(int index);
	void AddColumn(LPCSTR name);

	LRESULT Read(Mp4File* file);
	LRESULT Write(Mp4File* file);

	int GetSize();

// Data Members -----------------------------------------------
protected:
	Mp4PropertyArray fProperties;	///< �����б�.
};

typedef SmartPtr<Mp4TableProperty> Mp4TablePropertyPtr;

//______________________________________________________________________________
///////////////////////////////////////////////////////////////////////////////
// Mp4File

/** ����һ�� MP4 �ļ�, �������Ҫ��װ�� MP4 �ļ��� I/O ����. */
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
	INT64 fFileSize;	///< �ļ�����
	FILE* fFile;		///< �ļ����
	bool fIsWriting;	///< �Ƿ�����д

	INT64 fBitsBuffer;	///< �������ݻ�����
	int fBitsCount;		///< ����ı�����.
	ENUM_MP4_WRITE_DESTYPE m_desType;//����ftp��ʽд����д������
	unsigned long m_ftpFD;
};

typedef SmartPtr<Mp4File> Mp4FilePtr;

//______________________________________________________________________________
///////////////////////////////////////////////////////////////////////////////
// �������߷���

/** �� 4 �� ASCII �ַ���ʽ�� atom �����ַ���ת���� UINT ��ʽ�� atom ����. */
inline UINT ATOMID(const char* s) {
	if (s == NULL || *s == '\0') {
		return 0;
	}
	return (s[0] << 24) | (s[1] << 16) | (s[2] << 8) | s[3];
}

/** ���� MP4 �ļ���ʽ�ĵ�ǰʱ��. */
inline INT64 Mp4GetTimestamp() {
	INT64 ret = 0;
	//ret = time(NULL);
	ret += 2082844800;
	return ret;	// MP4 start date is 1/1/1904
	// 208284480 is (((1970 - 1904) * 365) + 17) * 24 * 60 * 60
}

/** ����ָ���Ĵ�С�Ļ�����. */
inline void* Mp4Malloc(size_t size) {
	if (size == 0) {
		return NULL;
	}

	//void* p = malloc(size);
	
	//�޸�Ϊʹ��new����
	void* p = new unsigned char[size];

	return p;
}

/** ���·���ָ���Ĵ�С�Ļ�����. */
//inline void* Mp4Realloc(void* p, size_t newSize) {
//	// workaround library bug
//	if (p == NULL && newSize == 0) {
//		return NULL;
//	}
//	p = realloc(p, newSize);
//	return p;
//}

/** �ͷ�ָ���Ļ�����. */
inline void Mp4Free(void* p) {
	if (p != NULL) {
		//free(p);
		//�޸�Ϊnew ����
		unsigned char* ptr = (unsigned char*)(p);
		delete [] ptr;
	}
}

#endif // !defined(_NS_VISION_MP4_COMMON_H)
