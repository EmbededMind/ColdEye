#pragma once

#include <vector>
#include <math.h>
#include "MyListUI.h"
#include "File\RecordFileInfo.h"

#include "UIlib.h"
using namespace DuiLib;

class CVideoListUI :public CListUI
{
	DECLARE_DUICONTROL(CVideoListUI)

public:
	enum{ SCROLL_TIMERID = 10 };

	struct NodeData
	{
		int id;
		int _level;
		bool _expand;
		bool _type;
		CDuiString _text;
		CMyListUI * _pListElement;
	};

	class Node
	{
		typedef std::vector<Node*> Children;
		Children	_children;
		Node*		_parent;
		NodeData	_data;

	private:
		void set_parent(Node* parent) { _parent = parent; }	//���ø��ڵ�

	public:
		Node() :_parent(NULL) {}
		explicit Node(NodeData t) :_data(t), _parent(NULL) {}
		Node(NodeData t, Node *parent) :_data(t), _parent(parent) {}

		~Node()
		{
			for (int i = 0; i < num_children(); i++)
				delete _children[i];
		}

		NodeData& data() { return _data; }							//��ȡ�ڵ�����
		int num_children() const { return _children.size(); }		//�ӽڵ����
		Node* child(int i) { return _children[i]; }					//��ȡ�ӽڵ�
		Node* parent() { return _parent; }							//��ȡ���ڵ�
		bool has_children() const { return num_children() > 0; };	//�ж��Ƿ�����ӽڵ�

		void add_child(Node*child)									//����ӽڵ�
		{
			child->set_parent(this);
			_children.push_back(child);
		}

		void remove_child(Node* child)	//�Ƴ��ӽڵ�
		{
			Children::iterator iter = _children.begin();
			for (; iter < _children.end(); ++iter)
			{
				if (*iter == child)
				{
					_children.erase(iter);
					return;
				}
			}
		}

		Node* get_last_child()	//��ȡ���һ���ӽڵ�
		{
			if (has_children())
			{
				return child(num_children() - 1)->get_last_child();
			}
			else
				return this;
		}
	};



public:
	CVideoListUI() : _root(NULL), m_dwDelayDeltaY(0), m_dwDelayNum(0), m_dwDelayLeft(0)
	{
		//SetItemShowHtml(true);
		_root = new Node;
		_root->data()._level = -1;
		_root->data()._expand = true;
	}

	~CVideoListUI() { if (_root) delete _root; };

	bool Add(CControlUI* pControl) //��ӿؼ�
	{
		if (!pControl) return false;
		if (_tcscmp(pControl->GetClass(), _T("ListLabelElementUI")) != 0) return false;
		return	CListUI::Add(pControl);
	}

	bool AddAt(CControlUI* pControl, int iIndex) //��ӿؼ���ָ����λ��
	{
		if (!pControl) return false;
		if (_tcscmp(pControl->GetClass(), _T("ListLabelElementUI")) != 0) return false;
		return	CListUI::AddAt(pControl,iIndex);
	}

	bool Remove(CControlUI* pControl) //�Ƴ��ؼ�
	{
		if (!pControl) return false;
		if (_tcscmp(pControl->GetClass(), _T("ListLabelElementUI")) != 0) return false;

		return CListUI::Remove(pControl);
		//if (reinterpret_cast<Node*>(static_cast<CListLabelElementUI*>(pControl->GetInterface(_T("ListLabelElement")))->GetTag()) == NULL)
		//	return CListUI::Remove(pControl);
		//else
		//	return RemoveNode(reinterpret_cast<Node*>(static_cast<CListLabelElementUI*>(pControl->GetInterface(_T("ListLabelElement")))->GetTag()));
	}

	bool RemoveAt(int iIndex) //�Ƴ�ָ���Ľڵ�
	{
		CControlUI* pControl = GetItemAt(iIndex);
		if (!pControl) return false;
		if (_tcscmp(pControl->GetClass(), _T("ListLabelElementUI")) != 0) return false;

		if (reinterpret_cast<Node*>(static_cast<CListLabelElementUI*>(pControl->GetInterface(_T("ListLabelElement")))->GetTag()) == NULL)
			return CListUI::RemoveAt(iIndex);
		else
			return RemoveNode(reinterpret_cast<Node*>(static_cast<CListLabelElementUI*>(pControl->GetInterface(_T("ListLabelElement")))->GetTag()));
	}

	void RemoveAll() //�Ƴ����еĽڵ�
	{
		CListUI::RemoveAll();
		for (int i = 0; i < _root->num_children(); ++i)
		{
			Node* child = _root->child(i);
			RemoveNode(child);
		}
		delete _root;
		_root = new Node;
		_root->data()._level = -1;
		_root->data()._expand = true;
		_root->data()._pListElement = NULL;
	}

	void SetVisible(bool bVisible = true) //�����Ƿ�ɼ�
	{
		if (m_bVisible == bVisible) return;
		CControlUI::SetVisible(bVisible);
	}

	void SetInternVisible(bool bVisible = true)
	{
		CControlUI::SetInternVisible(bVisible);
	}

	void DoEvent(TEventUI& event);

	Node* GetRoot() { return _root; }

	Node* AddNode(LPCTSTR text, int id,CRecordFileInfo* pInfo, Node* parent = NULL ) //��ӽڵ�
	{
		if (!parent) 
			parent = _root;

		CMyListUI* pListElement = new CMyListUI(pInfo);
		Node* node = new Node;
		node->data().id = id;
		node->data()._level = parent->data()._level + 1;
		node->data()._expand = false;
		node->data()._text = text;
		node->data()._pListElement = pListElement;

		if (parent != _root) {
			if (!(parent->data()._expand && parent->data()._pListElement->IsVisible()))
				pListElement->SetInternVisible(false);
		}
		pListElement->SetText(node->data()._text);
		pListElement->SetTag((UINT_PTR)node);
		//pListElement->SetFixedHeight(65);
		int index = 0;

		if (parent == _root)
		{
			index = 0;
		}
		else
		{
			index = 1;
		}
		if (!CListUI::AddAt(pListElement, index)) {
			delete pListElement;
			delete node;
			node = NULL;
		}
		parent->add_child(node);
		return node;
	}

	bool RemoveNode(Node* node)	//�Ƴ��ڵ�
	{
		if (!node || node == _root)return false;
		for (int i = node->num_children() - 1; i >= 0; --i)
		{
			Node* child = node->child(i);
			RemoveNode(child);
		}
		CVideoListUI::Remove(node->data()._pListElement);
		node->parent()->remove_child(node);
		delete node;
		return true;
	}

	void ExpandNode(Node* node, bool expand) //
	{
		if (!node || node == _root) return;

		if (node->data()._expand == expand) return;
		node->data()._expand = expand;
		node->data()._pListElement->SetText(node->data()._text);

		if (!node->data()._pListElement->IsVisible()) return;
		if (!node->has_children()) return;

		Node* begin = node->child(0);
		Node* end = node->get_last_child();
		int nFirst = begin->data()._pListElement->GetIndex();
		int nLast = end->data()._pListElement->GetIndex();
		for (int i = nLast; i <= nFirst; ++i) {
			CControlUI* control = GetItemAt(i);
			if (_tcscmp(control->GetClass(), _T("ListLabelElementUI")) == 0) {

				Node* local_parent = ((CVideoListUI::Node*)control->GetTag())->parent();
				control->SetInternVisible(local_parent->data()._expand && local_parent->data()._pListElement->IsVisible());
			}
		}

		NeedUpdate();
	}

	SIZE GetExpanderSizeX(Node* node) const
	{
		if (!node || node == _root) return CDuiSize();
		if (node->data()._level >= 3) return CDuiSize();

		SIZE szExpander = { 0 };
		szExpander.cx = 6 + 24 * node->data()._level - 4/*�ʵ��Ŵ�һ��*/;
		szExpander.cy = szExpander.cx + 16 + 8/*�ʵ��Ŵ�һ��*/;
		return szExpander;
	}

	Node* AddHeadNode(CString sText, int nID, CRecordFileInfo* pInfo);
	Node* AddChildNode(CString sText, CVideoListUI::Node *HeadNode, int nID, CRecordFileInfo* pInfo);
	void AddRecordFile(CRecordFileInfo* pInfo);
	void AddItem(CRecordFileInfo* pInfo);
	void RefreshList();
	void DeleteRecordFile(CRecordFileInfo* pInfo);

	void CompareBeginTime(CTime refTime, CTime tbegin, CTime tend, CRecordFileInfo * pInfo, CVideoListUI::Node* pNode);
	int IsNewDay(CRecordFileInfo *HeadInfo,CRecordFileInfo * pInfo);

	void SetListElementHeight(float);
private:
	Node* _root;
	int m_listElementHight;
	LONG m_dwDelayDeltaY;
	DWORD m_dwDelayNum;
	DWORD m_dwDelayLeft;

public:
	int mhintNumber;
};

