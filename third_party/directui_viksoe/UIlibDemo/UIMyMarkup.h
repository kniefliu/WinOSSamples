#ifndef __MY_MARKUP_H__
#define __MY_MARKUP_H__

class CMyMarkup;
class CMyMarkupNode;

class CMyMarkup
{
	friend CMyMarkupNode;
public:
	CMyMarkup(LPCTSTR pstrXML = NULL);
	~CMyMarkup();

	bool Load(LPCTSTR pstrXML);
	bool LoadFromFile(LPCTSTR pstrFilename);
	void Release();
	bool IsValid() const;

	void SetPerserveWhitespace(bool bPreserve = true);
	void GetLastErrorMessage(LPTSTR pstrMessage, SIZE_T cchMax) const;
	void GetLastErrorLocation(LPTSTR pstrSource, SIZE_T cchMax) const;

	CMyMarkupNode GetRoot();

private:
	typedef struct tagXMLELEMENT
	{
		ULONG iStart;
		ULONG iChild;
		ULONG iNext;
		ULONG iParent;
		ULONG iData;
	} XMLELEMENT;

	LPTSTR m_pstrXML;
	XMLELEMENT* m_pElements;
	ULONG m_nElements;
	ULONG m_nReservedElements;
	TCHAR m_szErrorMsg[100];
	TCHAR m_szErrorXML[50];
	bool m_bPreserveWhitespace;

private:
	bool _Parse();
	bool _Parse(LPTSTR& pstrText, ULONG iParent);
	XMLELEMENT *_ReserveElement();
	inline void _SkipWhiteSpace(LPTSTR& pstr) const;
	inline void _SkipWhiteSpace(LPCTSTR& pstr) const;
	inline void _SkipIdentifier(LPTSTR& pstr) const;
	inline void _SkipIdentifier(LPCTSTR& pstr) const;
	bool _ParseData(LPTSTR& pstrText, LPTSTR& pstrData, char cEnd);
	void _ParseMetaChar(LPTSTR& pstrText, LPTSTR& pstrDest);
	bool _ParseAttributes(LPTSTR& pstrText);
	bool _Failed(LPCTSTR pstrError, LPCTSTR pstrLocation = NULL);
};

class CMyMarkupNode
{
	friend CMyMarkup;
private:
	CMyMarkupNode();
	CMyMarkupNode(CMyMarkup* pOwner, int iPos);

public:
	bool IsValid() const;

	CMyMarkupNode GetParent();
	CMyMarkupNode GetSibling();
	CMyMarkupNode GetChild();
	CMyMarkupNode GetChild(LPCTSTR pstrName);

	bool HasSiblings() const;
	bool HasChildren() const;
	LPCTSTR GetName() const;
	LPCTSTR GetValue() const;

	bool HasAttibutes();
	bool HasAttibute(LPCTSTR pstrName);
	int GetAttibuteCount();
	LPCTSTR GetAttributeName(int iIndex);
	LPCTSTR GetAttributeValue(int iIndex);
	LPCTSTR GetAttributeValue(LPCTSTR pstrName);
	bool GetAttributeValue(int iIndex, LPTSTR pstrValue, SIZE_T cchMax);
	bool GetAttributeValue(LPCTSTR pstrName, LPTSTR pstrValue, SIZE_T cchMax);

private:
	void _MapAttributes();

	enum { MAX_XML_ATTRIBUTES = 20 };

	typedef struct 
	{
		ULONG iName;
		ULONG iValue;
	}XMLAttribute;

	int m_iPos;
	int m_nAttributes;
	XMLAttribute m_aAttributes[MAX_XML_ATTRIBUTES];
	CMyMarkup* m_pOwner;
};

#endif//__MY_MARKUP_H__
