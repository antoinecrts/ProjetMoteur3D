#include "GLError.h"
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

void GLError::clearError()
{
	glGetError();
}

GLenum GLError::getLastErrorType()
{
	return glGetError();
}

string GLError::getLastErrorBase(string userMarkUp)
{
		GLenum catchError = glGetError();
		string errorMessage = userMarkUp;
		switch (catchError)
		{
		case GL_INVALID_ENUM :
			errorMessage += " - An unacceptable value is specified for an enumerated argument ";
			break;
		case GL_INVALID_VALUE :
			errorMessage += " - A numeric argument is out of range ";
			break;
		case GL_INVALID_OPERATION :
			errorMessage += " - The specified operation is not allowed in the current state ";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION :
			errorMessage += " - The framebuffer object is not complete ";
			break;
		case GL_OUT_OF_MEMORY :
			errorMessage += " - There is not enough memory left to execute the command ";
			break;
		case GL_STACK_UNDERFLOW :
			errorMessage += " - An attempt has been made to perform an operation that would cause an internal stack to underflow ";
			break;
		case GL_STACK_OVERFLOW :
			errorMessage += " - An attempt has been made to perform an operation that would cause an internal stack to overflow ";
			break;
		case GL_NO_ERROR :
		default :
			break;
		}
		return errorMessage;
}

void GLError::displayErrorLog()
{
	GLint maxMsgLen = 0;
	glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH, &maxMsgLen);
	GLuint numMsgs = 250;
	std::vector<GLchar> msgData(numMsgs * maxMsgLen);
	std::vector<GLenum> sources(numMsgs);
	std::vector<GLenum> types(numMsgs);
	std::vector<GLenum> severities(numMsgs);
	std::vector<GLuint> ids(numMsgs);
	std::vector<GLsizei> lengths(numMsgs);

	GLuint numFound = glGetDebugMessageLog(numMsgs, msgData.size(), &sources[0], &types[0], &ids[0], &severities[0], &lengths[0], &msgData[0]);

	sources.resize(numFound);
	types.resize(numFound);
	severities.resize(numFound);
	ids.resize(numFound);
	lengths.resize(numFound);

	std::vector<std::string> messages;
	messages.reserve(numFound);

	std::vector<GLchar>::iterator currPos = msgData.begin();
	for(size_t msg = 0; msg < lengths.size(); ++msg)
	{
		messages.push_back(std::string(currPos, currPos + lengths[msg] - 1));
		currPos = currPos + lengths[msg];
	}

	LOG_TRACE  << "Debug log : " << messages.size() << " message(s) " << std::endl;
	for(int i = 0; i < messages.size() ; i++)
	{

		LOG_TRACE <<" - " << messages[i] << std::endl;
	}
}

void APIENTRY DebugMessageCallback
(
		GLenum source,
		GLenum type,
		GLuint id_msg,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		void* userParam
)
{
	//std::stringstream msg_str;
	LOG_TRACE << "Message from ";
	switch(source)
	{
	case GL_DEBUG_SOURCE_API:
		LOG_TRACE << "OpenGL API :: ";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		LOG_TRACE << "Window-system API :: ";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		LOG_TRACE << "Shader compiler :: ";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		LOG_TRACE << "Third Party :: ";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		LOG_TRACE << "User application :: ";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		LOG_TRACE << "Other :: ";
		break;
	default:
		LOG_TRACE << "None ::";
		break;

	}

	LOG_TRACE << " Severity = " ;
	switch(severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		LOG_TRACE << "High";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		LOG_TRACE << "Medium";
		break;
	case GL_DEBUG_SEVERITY_LOW:
		LOG_TRACE << "Low";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		LOG_TRACE << "Notification";
		break;
	default:
		std::cout << "Undefined";
		break;
	}
	std::cout << endl ;
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		LOG_TRACE << "> Error ::";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		LOG_TRACE << "> Deprecated behavior ::";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		LOG_TRACE << "> Undefined behavior ::";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		LOG_TRACE << "> Portability warning ::";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		LOG_TRACE << "> Performance warning ::";
		break;
	case GL_DEBUG_TYPE_MARKER:
		LOG_TRACE << "> Command stream annotation ::";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		LOG_TRACE << "> Group pushing ::";
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		LOG_TRACE << "> Group poping ::";
		break;
	case GL_DEBUG_TYPE_OTHER:
		LOG_TRACE << "> Other ::";
		break;
	default:
		LOG_TRACE << "> Undefined ::";
		break;
	}

	LOG_TRACE << message <<  endl;

}

void GLError::enableCallback()
{
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
//	glDebugMessageCallback(DebugMessageCallback,NULL);

}

void GLError::disableCallback()
{

	glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDisable(GL_DEBUG_OUTPUT);
}


void GLError::sendDebugNotification(string message, bool testToSend)
{
	if (testToSend) 
		glDebugMessageInsert
		(
			GL_DEBUG_SOURCE_APPLICATION,
			GL_DEBUG_TYPE_OTHER,
			0,
			GL_DEBUG_SEVERITY_NOTIFICATION,
			message.size(),
			message.c_str()
		);
}