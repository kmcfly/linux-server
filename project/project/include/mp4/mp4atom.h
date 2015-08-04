
#if !defined(_NS_VISION_MP4_ATOM_H)
#define _NS_VISION_MP4_ATOM_H

#include "mp4common.h"

//______________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////
// Mp4Atom

class Mp4Atom;
typedef SmartPtr<Mp4Atom> Mp4AtomPtr;
typedef Mp4Array<Mp4AtomPtr> Mp4AtomArray;

/** ���� MP4 �ļ��� ATOM �ڵ�. */
class Mp4Atom : public BaseObject
{
public:
	Mp4Atom();
	Mp4Atom(LPCSTR type,ENUM_MP4_WRITE_DESTYPE desType = ENUM_MP4_DESTYPE_LOCAL);
	virtual ~Mp4Atom();
	DECLARE_BASEREF_METHODS(Mp4Atom);

// Attributes -------------------------------------------------
public:
	void SetSize(INT64 size);
	int GetSize();
	
	void SetStart(INT64 start);
	INT64 GetStart() { return fStart; }

	Mp4AtomPtr GetParent() { return fParentAtom; }
	void SetParent(Mp4Atom* parent) { fParentAtom = parent; }

	LPCSTR GetType() const { return fType; }
	int GetChildAtomCount() { return fChildAtoms.GetCount(); }
	int GetPropertyCount() { return fProperties.GetCount(); }

	ENUM_MP4_WRITE_DESTYPE GetDesType(){return m_desType;}

// Operations -------------------------------------------------
public:
	LRESULT Write(Mp4File* file);
	LRESULT Read(Mp4File* file);

	Mp4PropertyPtr GetProperty(int index);
	Mp4PropertyPtr FindProperty(LPCSTR name);
	Mp4ArrayPropertyPtr GetTableProperty(LPCSTR name, LPCSTR col);	

	void SetStringProperty(LPCSTR name, LPCSTR value);
	int GetStringProperty(LPCSTR name, char* buf, UINT bufLen);

	void SetIntProperty(LPCSTR name, INT64 value);
	INT64 GetIntProperty(LPCSTR name);

	void SetFloatProperty(LPCSTR name, float value);
	float GetFloatProperty(LPCSTR name);

	Mp4AtomPtr AddChildAtom(LPCSTR name ,ENUM_MP4_WRITE_DESTYPE desType = ENUM_MP4_DESTYPE_LOCAL, int index = -1);
	Mp4AtomPtr GetChildAtom(LPCSTR name);
	Mp4AtomPtr GetChildAtom(int index);
	Mp4AtomPtr FindAtom( LPCSTR name );

	void ClearChildAtoms();

	LRESULT BeginWrite( Mp4File* file );
	LRESULT FinishWrite( Mp4File* file );

	virtual LRESULT Init(BYTE version = 0);
	void Clear();

// Implementation ---------------------------------------------
protected:
	void AddVersionAndFlags();
	virtual LRESULT AddProperties(BYTE version);
	Mp4PropertyPtr AddProperty(Mp4PropertyType type, int size = 0, LPCSTR name = NULL);

	void Skip( Mp4File* file );
	virtual void Reset();

	LRESULT ReadProperties(Mp4File* file);
	LRESULT WriteProperties(Mp4File* file);
	LRESULT ReadChildAtoms(Mp4File* file);
	LRESULT WriteChildAtoms(Mp4File* file);

// Data Members -----------------------------------------------
protected:
	INT64 fStart;		///< ����ڵ����ļ��еĿ�ʼλ��
	INT64 fEnd;			///< ����ڵ����ļ��еĽ��λ��
	INT64 fSize;		///< ����ڵ��ܹ��Ĵ�С, ����ͷ��
	char  fType[5];		///< ����ڵ������, 4���ֽ�
	BYTE  fDepth;		///< ����ڵ�����
	bool  fExpectChild;	///< �Ƿ��������ӽڵ�

	Mp4PropertyArray fProperties;	///< �����б�
	Mp4AtomArray	 fChildAtoms;	///< �ӽڵ��б�
	Mp4AtomPtr		 fParentAtom;	///< ����ڵ�ĸ��ڵ�
	ENUM_MP4_WRITE_DESTYPE m_desType;
};

//______________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////
// AvcCAtom

/** ��װ�˶� AvcC atom �Ĳ���. */
class Mp4AvcCAtom
{
public:
	Mp4AvcCAtom(Mp4Atom* avcC);
	
	// Operations -------------------------------------------------
public:
	void GetProfileLevel(BYTE *pProfile, BYTE *pLevel);
	void SetProfileLevel (BYTE pProfile, BYTE pLevel);
	
	void SetProfileCompatibility(BYTE compatibility);
	BYTE GetProfileCompatibility();	
	UINT GetLengthSize();
	
	bool AddSequenceParameters(const BYTE* pSequence, WORD sequenceLen);
	bool AddPictureParameters(const BYTE* pPicture, WORD pictureLen);
	BYTE* GetPictureParameters(int index, WORD& length);
	BYTE* GetSequenceParameters(int index, WORD& length);
	UINT GetPictureSetCount();
	UINT GeSequenceSetCount();
	
protected:
	Mp4SizeTablePropertyPtr GetSizeTable(LPCSTR name);
	
	// Data Members -----------------------------------------------
protected:
	Mp4AtomPtr fAvcC;
};


#endif // !defined(_NS_VISION_MP4_ATOM_H)
