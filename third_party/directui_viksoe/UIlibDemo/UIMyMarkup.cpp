#include "StdAfx.h"
#include "UIMyMarkup.h"

#ifndef TRACE
#define TRACE
#endif

///////////////////////////////////////////////////////////////////////////////////////
//
//
//

CMyMarkupNode::CMyMarkupNode()
	: m_pOwner(NULL)
{
}

CMyMarkupNode::CMyMarkupNode(CMyMarkup* pOwner, int iPos)
	: m_pOwner(pOwner)
	, m_iPos(iPos)
	, m_nAttributes(0)
{
}

CMyMarkupNode CMyMarkupNode::GetSibling()
{
	if (m_pOwner == NULL)
		return CMyMarkupNode();
	ULONG iPos = m_pOwner->m_pElements[m_iPos].iNext;
	if (iPos == 0)
		return CMyMarkupNode();
	return CMyMarkupNode(m_pOwner, iPos);
}

bool CMyMarkupNode::HasSiblings() const
{
	if (m_pOwner == NULL)
		return false;
	ULONG iPos = m_pOwner->m_pElements[m_iPos].iNext;
	return iPos > 0;
}

CMyMarkupNode CMyMarkupNode::GetChild()
{
	if (m_pOwner == NULL)
		return CMyMarkupNode();
	ULONG iPos = m_pOwner->m_pElements[m_iPos].iChild;
	if (iPos == 0)
		return CMyMarkupNode();
	return CMyMarkupNode(m_pOwner, iPos);
}

CMyMarkupNode CMyMarkupNode::GetChild(LPCTSTR pstrName)
{
	if (m_pOwner == NULL)
		return CMyMarkupNode();
	ULONG iPos = m_pOwner->m_pElements[m_iPos].iChild;
	while (iPos != 0)
	{
		if (_tcscmp(m_pOwner->m_pstrXML + m_pOwner->m_pElements[iPos].iStart, pstrName) == 0)
		{
			return CMyMarkupNode(m_pOwner, iPos);
		}
		iPos = m_pOwner->m_pElements[iPos].iNext;
	}
	return CMyMarkupNode();
}

bool CMyMarkupNode::HasChildren() const
{
	if (m_pOwner == NULL)
		return false;
	return m_pOwner->m_pElements[m_iPos].iChild != 0;
}

CMyMarkupNode CMyMarkupNode::GetParent()
{
	if (m_pOwner == NULL)
		return CMyMarkupNode();
	ULONG iPos = m_pOwner->m_pElements[m_iPos].iParent;
	if (iPos == 0)
		return CMyMarkupNode();
	return CMyMarkupNode(m_pOwner, iPos);
}

bool CMyMarkupNode::IsValid() const
{
	return m_pOwner != NULL;
}

LPCTSTR CMyMarkupNode::GetName() const
{
	if (m_pOwner == NULL)
		return NULL;
	return m_pOwner->m_pstrXML + m_pOwner->m_pElements[m_iPos].iStart;
}

LPCTSTR CMyMarkupNode::GetValue() const
{
	if (m_pOwner == NULL)
		return NULL;
	return m_pOwner->m_pstrXML + m_pOwner->m_pElements[m_iPos].iData;
}

LPCTSTR CMyMarkupNode::GetAttributeName(int iIndex)
{
	if (m_pOwner == NULL)
		return NULL;
	if (m_nAttributes == 0)
		_MapAttributes();
	if (iIndex < 0 || iIndex >= m_nAttributes)
		return _T("");
	return m_pOwner->m_pstrXML + m_aAttributes[iIndex].iName;
}

LPCTSTR CMyMarkupNode::GetAttributeValue(int iIndex)
{
	if (m_pOwner == NULL)
		return NULL;
	if (m_nAttributes == 0)
		_MapAttributes();
	if (iIndex < 0 || iIndex >= m_nAttributes)
		return _T("");
	return m_pOwner->m_pstrXML + m_aAttributes[iIndex].iValue;
}

LPCTSTR CMyMarkupNode::GetAttributeValue(LPCTSTR pstrName)
{
	if (m_pOwner == NULL)
		return NULL;
	if (m_nAttributes == 0)
		_MapAttributes();
	for (int i = 0; i < m_nAttributes; i++)
	{
		if (_tcscmp(m_pOwner->m_pstrXML + m_aAttributes[i].iName, pstrName) == 0)
			return m_pOwner->m_pstrXML + m_aAttributes[i].iValue;
	}
	return _T("");
}

bool CMyMarkupNode::GetAttributeValue(int iIndex, LPTSTR pstrValue, SIZE_T cchMax)
{
	if (m_pOwner == NULL)
		return false;
	if (m_nAttributes == 0)
		_MapAttributes();
	if (iIndex < 0 || iIndex >= m_nAttributes)
		return false;
	_tcsncpy(pstrValue, m_pOwner->m_pstrXML + m_aAttributes[iIndex].iValue, cchMax);
	return false;
}

bool CMyMarkupNode::GetAttributeValue(LPCTSTR pstrName, LPTSTR pstrValue, SIZE_T cchMax)
{
	if (m_pOwner == NULL)
		return false;
	if (m_nAttributes == 0)
		_MapAttributes();
	for (int i = 0; i < m_nAttributes; i++)
	{
		if (_tcscmp(m_pOwner->m_pstrXML + m_aAttributes[i].iName, pstrName) == 0)
		{
			_tcsncpy(pstrValue, m_pOwner->m_pstrXML + m_aAttributes[i].iValue, cchMax);
			return true;
		}
	}
	return false;
}

int CMyMarkupNode::GetAttibuteCount()
{
	if (m_pOwner == NULL)
		return 0;
	if (m_nAttributes == 0)
		_MapAttributes();
	return m_nAttributes;
}

bool CMyMarkupNode::HasAttibutes()
{
	if (m_pOwner == NULL)
		return false;
	if (m_nAttributes == 0)
		_MapAttributes();
	return m_nAttributes > 0;
}

bool CMyMarkupNode::HasAttibute(LPCTSTR pstrName)
{
	if (m_pOwner == NULL)
		return false;
	if (m_nAttributes == 0)
		_MapAttributes();
	for (int i = 0; i < m_nAttributes; i++)
	{
		if (_tcscmp(m_pOwner->m_pstrXML + m_aAttributes[i].iName, pstrName) == 0)
			return true;
	}
	return false;
}

void CMyMarkupNode::_MapAttributes()
{
	m_nAttributes = 0;
	LPCTSTR pstr = m_pOwner->m_pstrXML + m_pOwner->m_pElements[m_iPos].iStart;
	LPCTSTR pstrEnd = m_pOwner->m_pstrXML + m_pOwner->m_pElements[m_iPos].iData;
	pstr += _tcslen(pstr) + 1;
	while (pstr < pstrEnd)
	{
		m_pOwner->_SkipWhiteSpace(pstr);
		m_aAttributes[m_nAttributes].iName = pstr - m_pOwner->m_pstrXML;
		pstr += _tcslen(pstr) + 1;
		if (*pstr++ != '\"' && *pstr++ != '\'')
			return;
		m_aAttributes[m_nAttributes++].iValue = pstr - m_pOwner->m_pstrXML;
		if (m_nAttributes >= MAX_XML_ATTRIBUTES)
			return;
		pstr += _tcslen(pstr) + 1;
	}
}


///////////////////////////////////////////////////////////////////////////////////////
//
//
//

CMyMarkup::CMyMarkup(LPCTSTR pstrXML)
	: m_pstrXML(NULL)
	, m_pElements(NULL)
	, m_nElements(0)
	, m_bPreserveWhitespace(false)
{
	if (pstrXML != NULL)
		Load(pstrXML);
}

CMyMarkup::~CMyMarkup()
{
	Release();
}

bool CMyMarkup::IsValid() const
{
	return m_pElements != NULL;
}

void CMyMarkup::SetPerserveWhitespace(bool bPreserve)
{
	m_bPreserveWhitespace = bPreserve;
}

bool CMyMarkup::Load(LPCTSTR pstrXML)
{
	Release();
	SIZE_T cbLen = (_tcslen(pstrXML) + 1) * sizeof(TCHAR);
	m_pstrXML = static_cast<LPTSTR>(malloc(cbLen));
	::CopyMemory(m_pstrXML, pstrXML, cbLen);
	bool bRes = _Parse();
	if (!bRes)
		Release();
	return bRes;
}

bool CMyMarkup::LoadFromFile(LPCTSTR pstrFilename)
{
	Release();
	HANDLE hFile = ::CreateFile(pstrFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return _Failed(_T("Error opening file"));
	DWORD dwSize = ::GetFileSize(hFile, NULL);
	if (dwSize == 0)
		return _Failed(_T("File is empty"));
	DWORD dwRead = 0;
#ifdef _UNICODE
#else
	m_pstrXML = static_cast<LPTSTR>(malloc(dwSize + 1));
	::ReadFile(hFile, m_pstrXML, dwSize, &dwRead, NULL);
	::CloseHandle(hFile);
	m_pstrXML[dwSize] = '\0';
#endif
	if (dwRead != dwSize)
	{
		Release();
		return _Failed(_T("Could not read file"));
	}
	bool bRes = _Parse();
	if (!bRes)
		Release();
	return bRes;
}

void CMyMarkup::Release()
{
	if (m_pstrXML != NULL)
		free(m_pstrXML);
	if (m_pElements != NULL)
		free(m_pElements);
	m_pstrXML = NULL;
	m_pElements = NULL;
	m_nElements = 0;
}

void CMyMarkup::GetLastErrorMessage(LPTSTR pstrMessage, SIZE_T cchMax) const
{
	_tcsncpy(pstrMessage, m_szErrorMsg, cchMax);
}

void CMyMarkup::GetLastErrorLocation(LPTSTR pstrSource, SIZE_T cchMax) const
{
	_tcsncpy(pstrSource, m_szErrorXML, cchMax);
}

CMyMarkupNode CMyMarkup::GetRoot()
{
	if (m_nElements == 0)
		return CMyMarkupNode();
	return CMyMarkupNode(this, 1);
}

bool CMyMarkup::_Parse()
{
	_ReserveElement();
	::ZeroMemory(m_szErrorMsg, sizeof(m_szErrorMsg));
	::ZeroMemory(m_szErrorXML, sizeof(m_szErrorXML));
	LPTSTR pstrXML = m_pstrXML;
	return _Parse(pstrXML, 0);
}

bool CMyMarkup::_Parse(LPTSTR& pstrText, ULONG iParent)
{
	ULONG iPrevious = 0;
	for (;;)
	{
		if (*pstrText == '\0' && iParent <= 1)
			return true;
		if (*pstrText != '<')
			return _Failed(_T("Expected start tag"), pstrText);
		if (pstrText[1] == '/')
			return true;
		*pstrText++ = '\0';
		if (*pstrText == '!' || *pstrText == '?') 
		{
			TCHAR chEnd = *pstrText == '!' ? '-' : '?';
			while (*pstrText != '\0' && !(*pstrText == chEnd && *(pstrText + 1) == '>'))
				pstrText = ::CharNext(pstrText);
			if (*pstrText != '\0')
				pstrText += 2;
			_SkipWhiteSpace(pstrText);
			continue;
		}
		XMLELEMENT* pE1 = _ReserveElement();
		ULONG iPos = pE1 - m_pElements;
		pE1->iStart = pstrText - m_pstrXML;
		pE1->iParent = iParent;
		pE1->iNext = pE1->iChild = 0;
		if (iPrevious != 0)
			m_pElements[iPrevious].iNext = iPos;
		else if (iParent > 0)
			m_pElements[iParent].iChild =iPos;
		iPrevious = iPos;

		LPTSTR pstrName = pstrText;
		_SkipIdentifier(pstrText);
		LPTSTR pstrNameEnd = pstrText;
		if (*pstrText == '\0')
			return _Failed(_T("Error parsing element name"), pstrText);

		if (!_ParseAttributes(pstrText))
			return false;
		_SkipWhiteSpace(pstrText);
		if (pstrText[0] == '/' && pstrText[1] == '>')
		{
			pE1->iData = pstrText - m_pstrXML;
			*pstrText = '\0';
			pstrText += 2;
		}
		else 
		{
			if (*pstrText != '>')
				return _Failed(_T("Expected start-tag closing"), pstrText);

			pE1->iData = ++pstrText - m_pstrXML;
			LPTSTR pstrDest = pstrText;
			if (!_ParseData(pstrText, pstrDest, '<'))
				return false;

			if (*pstrText == '\0' && iParent <= 1)
				return true;
			if (*pstrText != '<')
				return _Failed(_T("Expected end-tag start"), pstrText);
			if (pstrText[0] == '<' && pstrText[1] != '/')
			{
				if (!_Parse(pstrText, iPos))
					return false;
			}
			else if (pstrText[0] == '<' && pstrText[1] == '/')
			{
				*pstrDest = '\0';
				*pstrText = '\0';
				pstrText += 2;
				SIZE_T cchName = pstrNameEnd - pstrName;
				if (_tcsncmp(pstrText, pstrName, cchName) != 0)
					return _Failed(_T("Unmatched closing tag"), pstrText);
				if (pstrText[cchName] != '>')
					return _Failed(_T("Unmatched closing tag"), pstrText);
				pstrText += cchName + 1;
			}
		}
		*pstrNameEnd = '\0';
		_SkipWhiteSpace(pstrText);
	}
}

CMyMarkup::XMLELEMENT* CMyMarkup::_ReserveElement()
{
	if (m_nElements == 0)
		m_nReservedElements = 0;
	if (m_nElements >= m_nReservedElements)
	{
		m_nReservedElements += (m_nReservedElements / 2) + 500;
		m_pElements = static_cast<XMLELEMENT*>(realloc(m_pElements, m_nReservedElements * sizeof(XMLELEMENT)));
	}
	return &m_pElements[m_nElements++];
}

void CMyMarkup::_SkipWhiteSpace(LPCTSTR& pstr) const
{
	while (*pstr != '\0' && *pstr <= ' ') 
		pstr++;
}

void CMyMarkup::_SkipWhiteSpace(LPTSTR& pstr) const
{
	while (*pstr != '\0' && *pstr <= ' ') 
		pstr++;
}

void CMyMarkup::_SkipIdentifier(LPCTSTR& pstr) const
{
	while (*pstr != '\0' && (*pstr == '_' || *pstr == ':' || _istalnum(*pstr)))
		pstr++;
}

void CMyMarkup::_SkipIdentifier(LPTSTR& pstr) const
{
	while (*pstr != '\0' && (*pstr == '_' || *pstr == ':' || _istalnum(*pstr)))
		pstr++;
}

bool CMyMarkup::_ParseAttributes(LPTSTR& pstrText)
{
	if (*pstrText == '>')
		return true;
	*pstrText++ = '\0';
	_SkipWhiteSpace(pstrText);
	while (*pstrText != '\0' && *pstrText != '>' && *pstrText != '/')
	{
		_SkipIdentifier(pstrText);
		if (*pstrText != '=')
			return _Failed(_T("Error while parsing attributes"), pstrText);
		*pstrText++ = '\0';
		TCHAR chQuote = *pstrText++;
		if (chQuote != '\"' && chQuote != '\'')
			return _Failed(_T("Expected attribute value"), pstrText);
		LPTSTR pstrDest = pstrText;
		if (!_ParseData(pstrText, pstrDest, chQuote))
			return false;
		if (*pstrText == '\0')
			return _Failed(_T("Error while parsing attribute string"), pstrText);
		*pstrDest = '\0';
		*pstrText++ = '\0';
		_SkipWhiteSpace(pstrText);
	}
	return true;
}

bool CMyMarkup::_ParseData(LPTSTR& pstrText, LPTSTR& pstrDest, char cEnd)
{
	while (*pstrText != '\0' && *pstrText != cEnd)
	{
		if (*pstrText == '&')
		{
			_ParseMetaChar(++pstrText, pstrDest);
		}
		if (*pstrText == ' ')
		{
			*pstrDest++ = *pstrText++;
			if (!m_bPreserveWhitespace)
				_SkipWhiteSpace(pstrText);
		}
		else 
		{
			*pstrDest++ = *pstrText++;
#ifdef _MBCS
			if (::IsDBCSLeadByte(*(pstrText - 1)))
				*pstrDest++ = *pstrText++;
#endif
		}
	}
	LPTSTR pstrFill = pstrDest + 1;
	while (pstrFill < pstrText)
		*pstrFill++ = ' ';
	return true;
}

void CMyMarkup::_ParseMetaChar(LPTSTR& pstrText, LPTSTR& pstrDest)
{
	if (pstrText[0] == 'a' && pstrText[1] == 'm' && pstrText[2] == 'p' && pstrText[3] == ';')
	{
		*pstrDest++ = '&';
		pstrText += 4;
	}
	else if (pstrText[0] == 'l' && pstrText[1] == 't' && pstrText[2] == ';')
	{
		*pstrDest++ = '<';
		pstrText += 3;
	}
	else if (pstrText[0] == 'g' && pstrText[1] == 't' && pstrText[2] == ';')
	{
		*pstrDest++ = '>';
		pstrText += 3;
	}
	else if (pstrText[0] == 'q' && pstrText[1] == 'u' && pstrText[2] == 'o' && pstrText[3] == 't' && pstrText[4] == ';')
	{
		*pstrDest++ = '\"';
		pstrText += 5;
	}
	else if (pstrText[0] == 'a' && pstrText[1] == 'p' && pstrText[2] == 'o' && pstrText[3] == 's' && pstrText[4] == ';')
	{
		*pstrDest++ = '\'';
		pstrText += 5;
	}
	else
	{
		*pstrDest++ = '&';
	}
}

bool CMyMarkup::_Failed(LPCTSTR pstrError, LPCTSTR pstrLocation)
{
	TRACE("XML Error: %s", pstrError);
	TRACE(pstrLocation);
	_tcsncpy(m_szErrorMsg, pstrError, (sizeof(m_szErrorMsg) / sizeof(m_szErrorMsg[0])) - 1 );
	_tcsncpy(m_szErrorXML, pstrLocation != NULL ? pstrLocation : _T(""), lengthof(m_szErrorXML) - 1);
	return false;
}
