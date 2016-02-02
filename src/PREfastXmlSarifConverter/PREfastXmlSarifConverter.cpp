// PREfastXmlSarifConverter.cpp : Defines the entry point for the console application.
//

#include <fstream>
#include "SarifFormat.h"
#include "DefectXmlFormat.h"

extern "C" {__declspec(dllexport) BSTR __stdcall ConvertToSarif(BSTR bstrFilePath); }
extern "C" {__declspec(dllexport) BSTR __stdcall ConvertToSarifFromXml(BSTR bstrXmlText); }

HRESULT __stdcall ConvertToSarifHelperFromText(BSTR bstrXmlText, BSTR* pbstrSarifText);
HRESULT __stdcall ConvertToSarifHelperFromFile(BSTR bstrInputFile, BSTR bstrOutputFile, BSTR* pbstrSarifText);

class XmlToSarifConverter
{
private:
	bool ReadSFA(IXmlReader *pReader, XmlSfa &sfa)
	{
		while (!pReader->IsEOF())
		{
			XmlNodeType nodeType;
			HRESULT hr = pReader->Read(&nodeType);
			if (hr != S_OK)
				break;

			if (nodeType == XmlNodeType_Element)
			{
				LPCWSTR wszPrefix, wszLocalName, wszValue;
				UINT cchPrefix, cchLocalName, cchValue;

				pReader->GetPrefix(&wszPrefix, &cchPrefix);
				pReader->GetLocalName(&wszLocalName, &cchLocalName);
				pReader->GetValue(&wszValue, &cchValue);

				if (wcscmp(wszLocalName, L"FILEPATH") == 0)
				{
					HRESULT hr = pReader->Read(&nodeType);
					pReader->GetValue(&wszValue, &cchValue);
					sfa.SetFilePath(wszValue);
				}
				else if (wcscmp(wszLocalName, L"FILENAME") == 0)
				{
					HRESULT hr = pReader->Read(&nodeType);
					pReader->GetValue(&wszValue, &cchValue);
					sfa.SetFileName(wszValue);
				}
				else if (wcscmp(wszLocalName, L"COLUMN") == 0)
				{
					HRESULT hr = pReader->Read(&nodeType);
					pReader->GetValue(&wszValue, &cchValue);
					sfa.SetColumnNo(wszValue);

				}
				else if (wcscmp(wszLocalName, L"LINE") == 0)
				{
					HRESULT hr = pReader->Read(&nodeType);
					pReader->GetValue(&wszValue, &cchValue);
					sfa.SetLineNo(wszValue);
				}
				pReader->Read(&nodeType); // Covers an end element tag
			}
			else if (nodeType == XmlNodeType_EndElement)
			{
				break;
			}
		}

		return true;
	}

	bool ReadPath(IXmlReader *pReader, std::vector<XmlSfa> &path)
	{
		while (!pReader->IsEOF())
		{
			XmlNodeType nodeType;
			HRESULT hr = pReader->Read(&nodeType);
			if (hr != S_OK)
				break;

			if (nodeType == XmlNodeType_Element)
			{
				LPCWSTR wszPrefix, wszLocalName, wszValue;
				UINT cchPrefix, cchLocalName, cchValue;

				pReader->GetPrefix(&wszPrefix, &cchPrefix);
				pReader->GetLocalName(&wszLocalName, &cchLocalName);
				pReader->GetValue(&wszValue, &cchValue);

				if (wcscmp(wszLocalName, L"SFA") == 0)
				{
					XmlSfa sfa;
					ReadSFA(pReader, sfa);
					path.push_back(sfa);
				}
			}
			else if (nodeType == XmlNodeType_EndElement)
			{
				break;
			}
		}

		return true;
	}

	bool ReadDefect(IXmlReader *pReader, XmlDefect &defect)
	{
		while (!pReader->IsEOF())
		{
			XmlNodeType nodeType;
			HRESULT hr = pReader->Read(&nodeType);
			if (hr != S_OK)
				break;
			if (nodeType == XmlNodeType_Whitespace)
				continue;

			if (nodeType == XmlNodeType_Element)
			{
				LPCWSTR wszPrefix, wszLocalName, wszValue;
				UINT cchPrefix, cchLocalName, cchValue;

				pReader->GetPrefix(&wszPrefix, &cchPrefix);
				pReader->GetLocalName(&wszLocalName, &cchLocalName);
				pReader->GetValue(&wszValue, &cchValue);

				if (wcscmp(wszLocalName, L"SFA") == 0)
				{
					ReadSFA(pReader, defect.m_sfa);
				}
				else if (wcscmp(wszLocalName, L"PATH") == 0)
				{
					ReadPath(pReader, defect.m_path);
				}
				// probability and rank
				else if (wcscmp(wszLocalName, L"DEFECTCODE") == 0)
				{
					HRESULT hr = pReader->Read(&nodeType);
					pReader->GetValue(&wszValue, &cchValue);
					defect.SetDefectCode(wszValue);
				}
				else if (wcscmp(wszLocalName, L"DESCRIPTION") == 0)
				{
					HRESULT hr = pReader->Read(&nodeType);
					pReader->GetValue(&wszValue, &cchValue);
					defect.SetDescription(wszValue);
				}
				else if (wcscmp(wszLocalName, L"FUNCTION") == 0)
				{
					HRESULT hr = pReader->Read(&nodeType);
					pReader->GetValue(&wszValue, &cchValue);
					defect.SetFunction(wszValue);
				}
				else if (wcscmp(wszLocalName, L"DECORATED") == 0)
				{
					HRESULT hr = pReader->Read(&nodeType);
					pReader->GetValue(&wszValue, &cchValue);
					defect.SetDecorated(wszValue);
				}
				else if (wcscmp(wszLocalName, L"FUNCLINE") == 0)
				{
					HRESULT hr = pReader->Read(&nodeType);
					pReader->GetValue(&wszValue, &cchValue);
					defect.SetFunctionLine(wszValue);
				}
				pReader->Read(&nodeType);
			}
			else if (nodeType == XmlNodeType_EndElement)
			{
				break;
			}
		}

		return true;
	}

	bool InternalLoadXmlDefects(IStream *pInStream, std::deque<XmlDefect> &defectList)
	{
		IXmlReader *pReader = NULL;
		HRESULT hr; 
		if (FAILED(hr = CreateXmlReader(__uuidof(IXmlReader), (void**)&pReader, NULL)))
		{
			wprintf(L"Error creating xml reader, error is %08.8lx", hr);
			return false;
		}

		IXmlReaderInput *pReaderInput = NULL;
		pReader->SetInput(pInStream);

		pReader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit);

		while (!pReader->IsEOF())
		{
			XmlNodeType nodeType;
			HRESULT hr = pReader->Read(&nodeType);
			if (hr != S_OK)
				break;
			if (nodeType == XmlNodeType_Whitespace)
				continue;

			if (nodeType == XmlNodeType_Element)
			{

				LPCWSTR wszPrefix, wszLocalName, wszValue;
				UINT cchPrefix, cchLocalName, cchValue;

				pReader->GetPrefix(&wszPrefix, &cchPrefix);
				pReader->GetLocalName(&wszLocalName, &cchLocalName);
				pReader->GetValue(&wszValue, &cchValue);

				if (wcscmp(wszLocalName, L"DEFECT") == 0)
				{
					XmlDefect defect;
					if (ReadDefect(pReader, defect))
						defectList.push_back(defect);
				}
			}
		}
		return true;
	}

public:

	static bool LoadXmlDefectFromString(const std::wstring xmlText, std::deque<XmlDefect> &defectList)
	{
		_bstr_t inpString = xmlText.c_str();
		IStream *pInStream = SHCreateMemStream( (BYTE*) inpString.operator char *(), inpString.length());
		
		XmlToSarifConverter converter;
		return converter.InternalLoadXmlDefects(pInStream, defectList);
	}

	static bool LoadXmlDefects(const std::wstring &xmlFile, std::deque<XmlDefect> &defectList)
	{
		IStream *pInFileStream = NULL;
		HRESULT hr;
		if (FAILED(hr = SHCreateStreamOnFile(xmlFile.c_str(), STGM_READ, &pInFileStream)))
		{
			wprintf(L"Error creating file reader, error is %08.8lx", hr);
			return false;
		}

		XmlToSarifConverter converter;
		return converter.InternalLoadXmlDefects(pInFileStream, defectList);
		
	}

};

#if BUILD_CONSOLE
int main(int argc, char * argv[])
{
	if (argc != 2 && argc != 3)
	{
		std::wcout << "Usage: PREfastSarifConverter.exe <XmlFilename> [output.sarif] \n";
		return -1;
	}

	BSTR bstrInputFile = _bstr_t(argv[1]);
	BSTR bstrOutputFile = NULL;
	if (argc == 3)
	{
		bstrOutputFile = _bstr_t(argv[2]);
	}

	ConvertToSarifHelperFromFile(bstrInputFile, bstrOutputFile, NULL);
}
#endif

extern "C"
{
	__declspec(dllexport) BSTR __stdcall ConvertToSarif(BSTR bstrFilePath)
	{
		BSTR bstrResult;
		ConvertToSarifHelperFromFile(bstrFilePath, NULL, &bstrResult);
		return bstrResult;
	}

	__declspec(dllexport) BSTR __stdcall ConvertToSarifFromXml(BSTR bstrXmlText)
	{
		BSTR bstrResult;
		ConvertToSarifHelperFromText(bstrXmlText, &bstrResult);
		return bstrResult;
	}
}

HRESULT __stdcall Convert(const std::deque<XmlDefect> defectList, BSTR bstrOutputFile, BSTR* pbstrSarifText)
{
	SarifIssueLog issueLog;
	issueLog.SetVersion(L"0.4");

	// Set ToolInfo
	SarifToolInfo toolInfo;
	toolInfo.SetName(L"PREfast");
	toolInfo.SetFullName(L"PREfast Code Analysis");
	toolInfo.SetVersion(L"14.0.0");

	// Set RunInfo
	SarifRunInfo runInfo;
	runInfo.SetCommandLineArguments(L"");

	std::wstring uriCmdLineSrc = L"";
	SarifFileReference fr1;
	fr1.SetURI(uriCmdLineSrc);
	runInfo.AddAnalysisTarget(fr1);

	SarifRunLog runLog;
	runLog.SetToolInfo(toolInfo);
	runLog.SetRunInfo(runInfo);

	for (const XmlDefect &defect : defectList)
	{
		SarifRegion region;
		region.SetStartColumn(defect.m_sfa.GetColumnNo());
		region.SetStartLine(defect.m_sfa.GetLineNo());

		SarifPhysicalLocationComponent plc;
		plc.SetURI(uriCmdLineSrc);

		std::wstring uriIssueFile = GetDefectUri(defect.m_sfa);
		SarifPhysicalLocationComponent ifc;
		SarifLocation location;

		if (uriIssueFile != uriCmdLineSrc)
		{
			ifc.SetURI(uriIssueFile);
			if (region.IsValid())
				ifc.SetRegion(region);
			location.AddIssueFileComponent(ifc);
		}

		if (uriCmdLineSrc.length() > 0)
		{
			if (region.IsValid())
				plc.SetRegion(region);
		}
		location.AddAnalysisTargetComponent(plc);

		location.SetFullyQualifiedLogicalName(defect.GetFunction());
		location.AddLogicalLocationComponent(defect.GetFunction(), L"method");
		location.AddProperty(L"decorated", defect.GetDecorated());
		location.AddProperty(L"funcline", defect.GetFunctionLine());

		// Issue
		SarifIssue issue;
		issue.SetRuleId(defect.GetDefectCode());
		issue.SetFullMessage(defect.GetDescription());
		issue.AddLocation(location);

		if (wcslen(defect.GetProbability()) > 0)
			issue.AddProperty(L"probability", defect.GetProbability());

		if (wcslen(defect.GetRank()) > 0)
		{
			issue.AddProperty(L"rank", defect.GetRank());
		}

		if (defect.m_category.size() > 0)
		{
			for (auto mit : defect.m_category)
			{
				std::wstring key;
				GetXmlToSarifMapping(std::wstring(mit.first), key);
				issue.AddProperty(key, std::wstring(mit.second));
			}
		}

		if (defect.m_additionalInfo.size() > 0)
		{
			for (auto mit : defect.m_additionalInfo)
			{
				std::wstring key;
				GetXmlToSarifMapping(std::wstring(mit.first), key);
				issue.AddProperty(key, std::wstring(mit.second));
			}
		}

		if (defect.m_path.size() > 0)
		{
			SarifExecutionFlowEntries entries;
			for (const XmlSfa &sfa : defect.m_path)
			{
				SarifPhysicalLocationComponent fileLocation;

				fileLocation.SetURI(GetDefectUri(sfa));

				SarifRegion region;
				region.SetStartColumn(sfa.GetColumnNo());
				region.SetStartLine(sfa.GetLineNo());
				if (region.IsValid())
					fileLocation.SetRegion(region);

				SarifExecutionFlowEntry entry;
				entry.AddPhysicalLocationComponent(fileLocation);

				const XmlKeyEvent &keyEvent = sfa.GetKeyEvent();

				if (keyEvent.IsValid())
				{
					entry.AddProperty(L"id", keyEvent.GetId());
					entry.AddProperty(L"kind", keyEvent.GetKind());
					entry.AddProperty(L"importance", keyEvent.GetImportance());
					entry.SetMessage(keyEvent.GetMessage());
				}

				entries.AddExecutionFlowEntry(entry);
			}
			issue.AddExecutionFlow(entries);
		}
		runLog.AddIssue(issue);
	}
	issueLog.AddRunLog(runLog);

	std::wstring out = json::Serialize(issueLog.m_values);

	if (pbstrSarifText != NULL)
	{
		*pbstrSarifText = SysAllocString(out.c_str());
	}
	else if (bstrOutputFile != NULL)
	{
		std::wstring filepath = bstrOutputFile;
		std::wofstream ofs(filepath);
		if (ofs.is_open())
		{
			ofs.write(out.c_str(), out.size());
			if (ofs.fail())
			{
				ofs.close();
				return E_FAIL;
			}
			ofs.close();
		}
	}
	else
	{
		wprintf_s(L"%s", out.c_str());
	}
	return S_OK;
}

HRESULT __stdcall ConvertToSarifHelperFromText(BSTR bstrXmlText, BSTR* pbstrSarifText)
{
	std::wstring xmlText = bstrXmlText;
	std::deque<XmlDefect> defectList;
	XmlToSarifConverter::LoadXmlDefectFromString(xmlText, defectList);

	return Convert(defectList, NULL, pbstrSarifText);
}

HRESULT __stdcall ConvertToSarifHelperFromFile(BSTR bstrInputFile, BSTR bstrOutputFile, BSTR* pbstrSarifText)
{
	std::wstring filename = bstrInputFile;
	std::deque<XmlDefect> defectList;
	XmlToSarifConverter::LoadXmlDefects(filename, defectList);

	return Convert(defectList, bstrOutputFile, pbstrSarifText);
}