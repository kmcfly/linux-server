// MyList.h: interface for the CMyList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYLIST_H__4C05531C_6FE3_41E9_A220_3C5CA2386075__INCLUDED_)
#define AFX_MYLIST_H__4C05531C_6FE3_41E9_A220_3C5CA2386075__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef void* POS;

template <class T>
class CMyList  
{
typedef struct _node
{
	T  node;
	_node *pNext;
	_node *pPrev;
}
NODE;
public:
	POS			AddTail(const T &node);
	POS			AddHead(const T &node);
	T			RemoveTail();
	T			RemoveHead();
	T			GetTail() const;
	T&			GetTail();
	T			GetHead() const;
	T&			GetHead();
	T			GetNext(POS &pos) const;
	T&			GetNext(POS &pos);
	T			GetPrev(POS &pos) const;
	T&			GetPrev(POS &pos);
	POS			GetHeadPosition() const;
	POS			GetTailPosition() const;
	T			GetAt(POS pos) const;
	T&			GetAt(POS pos);

	POS			GetLastInsertPosition() const;
	POS			GetLastReadPosition() const;
	T			GetLastInsert() const;
	T&			GetLastInsert();
	T			GetLastRead() const;
	T&			GetLastRead();

	T			NextNode(POS pos) const;
	T&			NextNode(POS pos);
	T			prevNode(POS pos) const;
	T&			prevNode(POS pos);
	POS			InsertBefore(POS pos, const T &node);
	POS			InsertAfter(POS pos,const T &node);
	void		SetAt(POS pos,T newnode);
	POS			Find(T node, POS startAfter);
	void		RemoveAll();
	int			GetCount() const;
	bool		IsEmpty() const;
	void		RemoveAt(POS pos);
	CMyList();
	virtual ~CMyList();
private:
	NODE *m_pHead;
	NODE *m_pTail;
	int	m_Count;
	POS	m_lastInsertPos;	//表示最近插入的节点
	mutable POS m_lastReadPos;		//标识最近被访问的节点
};

#endif // !defined(AFX_MYLIST_H__4C05531C_6FE3_41E9_A220_3C5CA2386075__INCLUDED_)

