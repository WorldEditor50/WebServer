

#ifndef __ARGUMENT_PARSER_H__
#define __ARGUMENT_PARSER_H__

#include <string>
#include <map>

namespace CWSLib
{
	typedef enum tagEnArgType
	{
		EN_ARG_TYPE_NO_ARG = 0,// �����ò������������(�磺--name)
		EN_ARG_TYPE_REQUIRE_ARG = 1,// �����ò�������������(�磺--name Bob)
		EN_ARG_TYPE_OPTIONAL_ARG = 2// �����ò�����ɴ�����Ҳ�ɲ������� (��--name��--name Bob����)
	}EN_ARG_TYPE;

	struct COption
	{
		std::string strLongArgName;//���������� --name $name$,��strLongArgNameΪ"name"
		char cShortArgName;//�̲������� -n $name$,��cShortArgNameΪ'n',�������ַ�����NULL
		EN_ARG_TYPE enArgType;
		std::string strComments;//ע��
		std::string strHelpMsg;//������Ϣ
	};

	class ArgumentParser
	{
	public:
		void addDefaultArg(const COption& option);
		void addDefaultArgs(COption* optionList, int size);
		void parse(int argc, char** argv);	// May throw std::exception if argument not in format
		std::map<std::string, std::string>& getArgValue();

	private:
		bool checkShortName(const std::string& shortName, std::string& outName);
		bool checkLongName(const std::string& fullName, std::string& outName);
		void printErrorInfo();

	private:
		std::map<std::string, COption> m_defaultArgs;
		std::map<std::string, std::string> m_argumentMap;
		std::map<char, std::string> m_shortIndex;
	};
}

#endif // !__ARGUMENT_PARSER_H__

