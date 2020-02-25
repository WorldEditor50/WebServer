

#ifndef __BASE_COMMAND_H__
#define __BASE_COMMAND_H__

template<typename CommandType>
void ParseCommand(const char* content)
{
	CommandType object;
	object.parse(content);
}

#endif // !__BASE_COMMAND_H__

